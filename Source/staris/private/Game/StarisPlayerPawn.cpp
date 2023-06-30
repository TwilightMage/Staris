// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisPlayerPawn.h"

#include "StarisStatics.h"
#include "Camera/CameraComponent.h"
#include "Components/SphereComponent.h"
#include "Game/StarisGameSettings.h"
#include "Game/StarisPlayerController.h"
#include "UI/LabelExtension.h"
#include "UI/SceneLabel.h"
#include "UI/TitleLabel.h"
#include "Universe/Galaxy.h"
#include "Universe/GalaxySettingsManager.h"
#include "..\..\public\Universe\LabeledProxy.h"
#include "Universe/Star.h"
#include "Universe/StarisInstancedStaticMesh.h"
#include "Universe/System.h"
#include "Universe/VanillaGalaxySettings.h"


AStarisPlayerPawn::AStarisPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootComponent);

	CameraArm = CreateDefaultSubobject<USceneComponent>("Camera Arm");
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->SetRelativeRotation(FRotator(-45, 0, 0));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(CameraArm);
	CameraComponent->SetRelativeLocation(FVector(-1000, 0, 0));

	ProximitySphere = CreateDefaultSubobject<USphereComponent>("Proximity Sphere");
	ProximitySphere->SetupAttachment(CameraComponent);
	ProximitySphere->SetSphereRadius(5000);
	ProximitySphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::ProximitySphereBeginOverlap);
	ProximitySphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::ProximitySphereEndOverlap);
}

void AStarisPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	Galaxy = GetActorOfClass<AGalaxy>(this);
	GameSettings = GetActorOfClass<AStarisGameSettings>(this);

	RootComponent->SetWorldLocation(RootComponent->GetComponentLocation() * FVector(1, 1, 0));
	DesiredLocation = RootComponent->GetComponentLocation();

	DesiredYaw = RootComponent->GetRelativeRotation().Yaw;
	DesiredPitch = CameraArm->GetRelativeRotation().Pitch;

	//DesiredCamDist = -CameraComponent->GetRelativeLocation().X;
	DesiredCamDist = GameSettings->MinZoomDistance;
}

void AStarisPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto Settings = GetActorOfClass<AGalaxySettingsManager>(this);
	auto VanillaSettings = Settings->GetSettings<UVanillaGalaxySettings>();
	
	float MovementScale = FMath::Lerp(1.0f, 2000.0f, UnLerp(DesiredCamDist, GameSettings->MinZoomDistance, GameSettings->MaxZoomDistance));
	DesiredLocation += (RootComponent->GetForwardVector() * Movement.X * MovementScale + RootComponent->GetRightVector() * Movement.Y * MovementScale) * DeltaTime * 200;
	DesiredLocation.Z = 0;
	if (DesiredLocation.Size() > VanillaSettings->GalaxyRadius)
	{
		DesiredLocation = DesiredLocation.GetUnsafeNormal() * VanillaSettings->GalaxyRadius;
	}
	if (FVector::Distance(DesiredLocation, RootComponent->GetComponentLocation()) < KINDA_SMALL_NUMBER)
	{
		RootComponent->SetWorldLocation(DesiredLocation);
	}
	else
	{
		RootComponent->SetWorldLocation(FMath::Lerp(RootComponent->GetComponentLocation(), DesiredLocation, DeltaTime * 10));
	}

	auto ActualRotation = FRotator(CameraArm->GetRelativeRotation().Pitch, RootComponent->GetComponentRotation().Yaw, 0);
	auto DesiredRotation = FRotator(DesiredPitch, DesiredYaw, 0);
	FRotator NewRotation;
	if (1 - ActualRotation.Vector().Dot(DesiredRotation.Vector()) < SMALL_NUMBER)
	{
		NewRotation = DesiredRotation;
	}
	else
	{
		NewRotation = FRotator(FQuat::Slerp(ActualRotation.Quaternion(), DesiredRotation.Quaternion(), DeltaTime * 20));
	}
	RootComponent->SetRelativeRotation(FRotator(0, NewRotation.Yaw, 0));
	CameraArm->SetRelativeRotation(FRotator(NewRotation.Pitch, 0, 0));

	if (FMath::Abs(-DesiredCamDist - CameraComponent->GetRelativeLocation().X) < 0.1f)
	{
		CameraComponent->SetRelativeLocation(FVector(-DesiredCamDist, 0, 0));
	}
	else
	{
		CameraComponent->SetRelativeLocation(FVector(FMath::Lerp(CameraComponent->GetRelativeLocation().X, -DesiredCamDist, DeltaTime * 10), 0, 0));	
	}

	//if (Galaxy)
	//{
	//	PlanetsVisible = CameraComponent->GetComponentLocation().Z < 15000;
	//	
	//	if (PlanetsVisible != PlanetsVisiblePrev)
	//	{
	//		Galaxy->SetPlanetsVisible(PlanetsVisible);
	//		PlanetsVisiblePrev = PlanetsVisible;
	//	}
	//}

	Galaxy->SetStarsScale(FMath::Lerp(1.0f, 50.0f, UnLerp((float)CameraComponent->GetComponentLocation().Z, GameSettings->MinZoomDistance, GameSettings->MaxZoomDistance)));

	if (auto StarisPlayerController = Cast<AStarisPlayerController>(GetController()))
	{
		for (auto Pair : Labels)
		{
			FVector2D ScreenLocation;
			
			if (UGameplayStatics::ProjectWorldToScreen(StarisPlayerController, Pair.Key->GetLabelLocation(), ScreenLocation))
			{
				Pair.Value->SetPositionInViewport(ScreenLocation);
			}
		}
	}
}

void AStarisPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindKey(FInputChord(FKey("MiddleMouseButton")), IE_Pressed, this, &AStarisPlayerPawn::RotateCamera_Pressed);
	PlayerInputComponent->BindKey(FInputChord(FKey("MiddleMouseButton")), IE_Released, this, &AStarisPlayerPawn::RotateCamera_Released);

	PlayerInputComponent->BindKey(FInputChord(FKey("LeftMouseButton")), IE_Pressed, this, &AStarisPlayerPawn::Click);
	PlayerInputComponent->BindKey(FInputChord(FKey("RightMouseButton")), IE_Pressed, this, &AStarisPlayerPawn::OpenContextMenu);

	PlayerInputComponent->BindAction("PauseGameTime", IE_Pressed, this, &AStarisPlayerPawn::PauseGameTime);
	
	PlayerInputComponent->BindAxis("MovementForward", this, &AStarisPlayerPawn::MovementForward);
	PlayerInputComponent->BindAxis("MovementRight", this, &AStarisPlayerPawn::MovementRight);

	PlayerInputComponent->BindVectorAxis("Mouse2D", this, &AStarisPlayerPawn::Mouse2D);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AStarisPlayerPawn::CameraZoom);
}

void AStarisPlayerPawn::MoveToTarget(USceneComponent* Target, bool Track)
{
	MoveToLocation(Target->GetComponentLocation());
}

void AStarisPlayerPawn::MoveToLocation(const FVector& Location)
{
	DesiredLocation = FVector(Location.X, Location.Y, 0);

	if (!HasActorBegunPlay())
	{
		RootComponent->SetWorldLocation(DesiredLocation);
	}
}

void AStarisPlayerPawn::SetActiveLabelExtension(const TSubclassOf<ULabelExtension>& LabelExtensionClass)
{
	const auto ExtensionCDO = LabelExtensionClass.GetDefaultObject();
	
	for (const auto Labeled : Labels)
	{
		if (ExtensionCDO && ExtensionCDO->TestTarget(Labeled.Key))
		{
			Labeled.Value->SetLabelExtension(LabelExtensionClass);
		}
		else
		{
			Labeled.Value->SetLabelExtension(nullptr);
		}
	}

	ActiveLabelExtension = LabelExtensionClass;
}

void AStarisPlayerPawn::ProximitySphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto InstancedMesh = Cast<UStarisInstancedStaticMesh>(OtherComp))
	{
		if (auto Instance = InstancedMesh->TryGetObjectRef(OtherBodyIndex))
		{
			if (auto Star = Cast<UStar>(Instance->Object))
			{
				SystemsInProximity.FindOrAdd(Star->GetSystem())++;
			}

			auto Labeled = Cast<ILabeled>(Instance->Object);
			if (!Labeled)
			{
				if (auto LabeledProxy = Cast<ILabeledProxy>(Instance->Object))
				{
					Labeled = LabeledProxy->GetLabeled();

					if (Labeled->DisplayCounter++ > 0)
					{
						Labeled = nullptr;
					}
				}
			}
			
			if (Labeled)
			{
				if (auto Titled = Cast<ITitled>(Labeled))
				{
					auto TitleLabel = NewObject<UTitleLabel>(this, TitleLabelClass);
					TitleLabel->SetOwningPlayer(Cast<AStarisPlayerController>(GetController()));
					TitleLabel->AddToPlayerScreen();
					TitleLabel->Labeled = Labeled;
					TitleLabel->Setup(Cast<UObject>(Titled));
					TitleLabel->SetPositionInViewport(FVector2D(100000, 100000));

					if (ActiveLabelExtension && ActiveLabelExtension.GetDefaultObject()->TestTarget(Labeled))
					{
						TitleLabel->SetLabelExtension(ActiveLabelExtension);
					}
					
					Labels.Add(Labeled, TitleLabel);

					if (auto Focusable = Cast<IFocusable>(Labeled))
					{
						TitleLabel->OnLabelClicked.AddWeakLambda(this, [this, Focusable]()
						{
							if (auto StarisPlayerController = Cast<AStarisPlayerController>(GetController()))
							{
								StarisPlayerController->SelectFocusable(Focusable);
							}
						});
					}
				}
			}
		}
	}
}

void AStarisPlayerPawn::ProximitySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (auto InstancedMesh = Cast<UStarisInstancedStaticMesh>(OtherComp))
    	{
    		if (auto Instance = InstancedMesh->TryGetObjectRef(OtherBodyIndex))
    		{
    			if (auto Star = Cast<UStar>(Instance->Object))
    			{
    				auto System = Star->GetSystem();
    				if (auto SystemCounter = SystemsInProximity.Find(System))
    				{
    					(*SystemCounter)--;
    					if (*SystemCounter <= 0)
    					{
    						SystemsInProximity.Remove(System);
    					}
    				}
    			}

    			auto Labeled = Cast<ILabeled>(Instance->Object);
    			if (!Labeled)
    			{
    				if (auto LabeledProxy = Cast<ILabeledProxy>(Instance->Object))
    				{
    					Labeled = LabeledProxy->GetLabeled();

    					if (--Labeled->DisplayCounter > 0)
    					{
    						Labeled = nullptr;
    					}
    				}
    			}
    			
    			if (Labeled)
    			{
    				Labels.FindRef(Labeled)->RemoveFromParent();
    				
    				Labels.Remove(Labeled);
    			}
    		}
    	}
}

void AStarisPlayerPawn::RotateCamera_Pressed()
{
	bIsRotatingCamera = true;
}

void AStarisPlayerPawn::RotateCamera_Released()
{
	bIsRotatingCamera = false;
}

void AStarisPlayerPawn::Click()
{
	if (auto StarisPlayerController = Cast<AStarisPlayerController>(GetController()))
	{
		StarisPlayerController->Click();
	}
}

void AStarisPlayerPawn::OpenContextMenu()
{
	if (auto StarisPlayerController = Cast<AStarisPlayerController>(GetController()))
	{
		StarisPlayerController->OpenContextMenu();
	}
}

void AStarisPlayerPawn::PauseGameTime()
{
	if (Galaxy)
	{
		Galaxy->TogglePauseTime();
	}
}

void AStarisPlayerPawn::MovementForward(float Axis)
{
	Movement.X = Axis;
}

void AStarisPlayerPawn::MovementRight(float Axis)
{
	Movement.Y = Axis;
}

void AStarisPlayerPawn::Mouse2D(FVector Vector)
{
	if (bIsRotatingCamera)
	{
		DesiredPitch = FMath::Clamp(DesiredPitch + Vector.Y * 3, -89, 89);
		DesiredYaw += Vector.X * 3;
	}
}

void AStarisPlayerPawn::CameraZoom(float Axis)
{
	if (Axis == 0) return;
	
	float Aspect = UnLerp(DesiredCamDist, GameSettings->MinZoomDistance, GameSettings->MaxZoomDistance);
	
	DesiredCamDist = FMath::Clamp(DesiredCamDist - Axis * 30 * GameSettings->ScrollSensitivity * FMath::Lerp(1.f, 1000.f, Aspect), GameSettings->MinZoomDistance, GameSettings->MaxZoomDistance);
}

