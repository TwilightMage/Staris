// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisPlayerPawn.h"

#include "StarisStatics.h"
#include "Camera/CameraComponent.h"
#include "Game/StarisPlayerController.h"
#include "Universe/Galaxy.h"
#include "Universe/Star.h"
#include "Universe/System.h"


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
	CameraComponent->SetRelativeLocation(FVector(-5000, 0, 0));
}

void AStarisPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	RootComponent->SetWorldLocation(RootComponent->GetComponentLocation() * FVector(1, 1, 0));
	DesiredLocation = RootComponent->GetComponentLocation();

	DesiredYaw = RootComponent->GetComponentRotation().Yaw;
	DesiredPitch = CameraArm->GetComponentRotation().Pitch;

	DesiredCamDist = -CameraComponent->GetComponentLocation().X;
}

void AStarisPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float MovementScale = FMath::Lerp(1.0f, 20.0f, (DesiredCamDist - MinCamDist) / (MaxCamDist - MinCamDist));
	DesiredLocation += (RootComponent->GetForwardVector() * Movement.X * MovementScale + RootComponent->GetRightVector() * Movement.Y * MovementScale) * DeltaTime * 2000;
	DesiredLocation.Z = 0;
	RootComponent->SetWorldLocation(FMath::Lerp(RootComponent->GetComponentLocation(), DesiredLocation, DeltaTime * 10));

	auto NewRotation = FRotator(FQuat::Slerp(FRotator(CameraArm->GetRelativeRotation().Pitch, RootComponent->GetComponentRotation().Yaw, 0).Quaternion(), FRotator(DesiredPitch, DesiredYaw, 0).Quaternion(), DeltaTime * 20));
	RootComponent->SetRelativeRotation(FRotator(0, NewRotation.Yaw, 0));
	CameraArm->SetRelativeRotation(FRotator(NewRotation.Pitch, 0, 0));

	CameraComponent->SetRelativeLocation(FVector(FMath::Lerp(CameraComponent->GetRelativeLocation().X, -DesiredCamDist, DeltaTime * 10), 0, 0));
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
	if (auto Galaxy = GetActorOfClass<AGalaxy>(this))
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
	
	float aspect = (DesiredCamDist - MinCamDist) / (MaxCamDist - MinCamDist);
	
	DesiredCamDist = FMath::Clamp(DesiredCamDist - Axis * 300 * FMath::Lerp(1, 10, aspect), MinCamDist, MaxCamDist);

	float StarScale = FMath::Lerp(1.0f, 4.0f, FMath::Clamp(aspect * 2, 0, 1));

	FVector Scale = FVector(StarScale);
	auto Galaxy = GetActorOfClass<AGalaxy>(this);
	for (auto System : Galaxy->GetSystems())
	{
		for (auto Star : System->GetStars())
		{
			Star->SetRelativeScale3D(Scale);
		}
	}
}

