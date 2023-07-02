// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisPlayerController.h"

#include "Empire/Empire.h"
#include "Focusable.h"
#include "Components/AudioComponent.h"
#include "Game/BuildMode.h"
#include "Game/GlobalPlaneConsumer.h"
#include "Game/StarisPlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/ContextMenu.h"
#include "UI/ToolTip.h"
#include "Universe/Planet.h"
#include "Universe/StarisInstancedStaticMesh.h"
#include "Universe/System.h"


// Sets default values
AStarisPlayerController::AStarisPlayerController()
{
	MusicPlayer = CreateDefaultSubobject<UAudioComponent>("Music Player");
	MusicPlayer->SetupAttachment(GetRootComponent());

	MusicPlayer->OnAudioFinishedNative.AddWeakLambda(this, [this](UAudioComponent*)
	{
		PlayNextMusic();
	});
	
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
	bEnableTouchEvents = false;
}

void AStarisPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		SwitchBuildMode(true);
		
		UKismetSystemLibrary::ExecuteConsoleCommand(this, "stat fps");
		UKismetSystemLibrary::ExecuteConsoleCommand(this, "stat unit");
		UKismetSystemLibrary::ExecuteConsoleCommand(this, "t.maxfps 300");

		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);

		InputComponent->BindAction("AssignOrder", IE_Pressed, this, &AStarisPlayerController::AssignOrderPressed);
		InputComponent->BindAction("AssignOrder", IE_Released, this, &AStarisPlayerController::AssignOrderReleased);

		PlayNextMusic();
	}
}

void AStarisPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsLocalPlayerController())
	{
		if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
		{
			if (LocalPlayer && LocalPlayer->ViewportClient)
			{
				LocalPlayer->ViewportClient->GetMousePosition(CachedMousePosition);
			}
		}
			
		FHitResult Hit;
		GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, Hit);

		IFocusable* NewTargetedFocusable = nullptr;

		TargetedType = None;
		
		if (auto Component = Hit.Component.Get())
		{
			LocationUnderMouse = Hit.Location;
			HasLocationUnderMouse = true;
			
			if (auto Focusable = Cast<IFocusable>(Component))
			{
				NewTargetedFocusable = Focusable;
			}
			else if (auto StarisInstancedMesh = Cast<UStarisInstancedStaticMesh>(Component))
			{
				if (auto InstanceRef = StarisInstancedMesh->TryGetObjectRef(Hit.Item))
				{
					if (auto FocusableInstance = Cast<IFocusable>(InstanceRef->Object))
					{
						NewTargetedFocusable = FocusableInstance;
						TargetedType = Object;
					}
				}
			}
		}
		else
		{
			HasLocationUnderMouse = false;
		}

		if (GlobalPlaneConsumers.Num() > 0)
		{
			FVector WorldOrigin;
			FVector WorldDirection;
			if (UGameplayStatics::DeprojectScreenToWorld(this, CachedMousePosition, WorldOrigin, WorldDirection) == true)
			{
				float T;
				FVector HitLocation;
				if (UKismetMathLibrary::LinePlaneIntersection(WorldOrigin, WorldOrigin + WorldDirection * 10000000, FPlane(FVector::Zero(), FVector::UpVector), T, HitLocation))
				{
					if (HasLocationUnderMouse)
					{
						if (FVector::Distance(WorldOrigin, HitLocation) < FVector::Distance(WorldOrigin, LocationUnderMouse))
						{
							LocationUnderMouse = HitLocation;
							NewTargetedFocusable = nullptr;
							TargetedType = GlobalPlane;
						}
					}
					else
					{
						HasLocationUnderMouse = true;
						LocationUnderMouse = HitLocation;
						TargetedType = GlobalPlane;
					}
				}
			}
		}

		if (NewTargetedFocusable != FocusableUnderMouse)
		{
			if (CurrentToolTip)
			{
				CurrentToolTip->RemoveFromParent();
			}
			
			FocusableUnderMouse = NewTargetedFocusable;

			if (FocusableUnderMouse)
			{
				CurrentToolTip = NewObject<UToolTip>(this, ToolTipClass);
				CurrentToolTip->SetOwningPlayer(this);
				CurrentToolTip->AddToPlayerScreen(1001);
					
				FocusableUnderMouse->SetupToolTip(CurrentToolTip);
			}
		}

		int32 ViewportWidth;
		int32 ViewportHeight;
		GetViewportSize(ViewportWidth, ViewportHeight);

		if (CurrentToolTip)
		{
			FVector2D DesiredToolTipSize = CurrentToolTip->GetDesiredSize();

			CurrentToolTip->SetPositionInViewport(FVector2D(FMath::Min(CachedMousePosition.X + 6, ViewportWidth - DesiredToolTipSize.X), FMath::Min(CachedMousePosition.Y + 15, ViewportHeight - DesiredToolTipSize.Y)));
		}

		if (CurrentContextMenu && HasContextMenuAnchor)
		{
			FVector2D DesiredContextMenuSize = CurrentContextMenu->GetDesiredSize();
			
			FVector2D ScreenPosition;
			UGameplayStatics::ProjectWorldToScreen(this, ContextMenuAnchor, ScreenPosition);
			CurrentContextMenu->SetPositionInViewport(FVector2D(
				FMath::Clamp(ScreenPosition.X, 0, ViewportWidth - DesiredContextMenuSize.X),
				FMath::Clamp(ScreenPosition.Y, 0, ViewportHeight - DesiredContextMenuSize.Y)
				));
		}
	}
}

void AStarisPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (OwnedEmpire)
	{
		if (auto StarisPlayerPawn = Cast<AStarisPlayerPawn>(InPawn))
		{
			StarisPlayerPawn->MoveToLocation(OwnedEmpire->Capital->GetLocation());
		}
	}
}

UEmpire* AStarisPlayerController::GetEmpire() const
{
	return OwnedEmpire;
}

void AStarisPlayerController::AssignEmpire(UEmpire* Empire)
{
	if (OwnedEmpire)
	{
		OwnedEmpire->OnSystemChanged.RemoveAll(this);

		NotifyControllerRemoved(OwnedEmpire);

		OwnedEmpire = nullptr;
	}
	
	OwnedEmpire = Empire;

	if (OwnedEmpire)
	{
		NotifyControllerAssigned(OwnedEmpire);
		
		OwnedEmpire->OnSystemChanged.AddUObject(this, &AStarisPlayerController::EmpireSystemChanged);
		
		if (auto StarisPlayerPawn = Cast<AStarisPlayerPawn>(GetPawn()))
		{
			StarisPlayerPawn->MoveToLocation(OwnedEmpire->Capital->GetLocation());
		}	
	}
}

void AStarisPlayerController::Click()
{
	if (HasContextMenuAnchor)
	{
		HasContextMenuAnchor = false;
	}

	if (CurrentContextMenu)
	{
		CurrentContextMenu->RemoveFromParent();
		CurrentContextMenu = nullptr;
	}

	if (TargetedType == Object && FocusableUnderMouse)
	{
		SelectFocusable(FocusableUnderMouse);
	}
	else if (TargetedType == GlobalPlane)
	{
		for (const auto Consumer : GlobalPlaneConsumers)
		{
			Consumer->GlobalPlaneClicked(LocationUnderMouse);
		}
	}
}

void AStarisPlayerController::OpenContextMenu()
{
	if (HasLocationUnderMouse)
	{
		ContextMenuAnchor = LocationUnderMouse;
		HasContextMenuAnchor = true;
	}
	else
	{
		HasContextMenuAnchor = false;
	}
	
	if (CurrentContextMenu)
	{
		CurrentContextMenu->RemoveFromParent();
		CurrentContextMenu = nullptr;
	}

	TArray<UContextMenuItem*> Items;
	
	if (OwnedEmpire)
	{
		const auto ItemSet = OwnedEmpire->CreateContextActions(FocusableUnderMouse, SelectedFocusable);
		if (!ItemSet.IsEmpty())
		{
			UContextMenuSeparator::AddSeparatorIfNeeded(Items);
			Items.Append(ItemSet);
		}
	}

	if (FocusableUnderMouse)
	{
		const auto ItemSet = FocusableUnderMouse->CreateContextActionsHovered(SelectedFocusable);
		if (!ItemSet.IsEmpty())
		{
			UContextMenuSeparator::AddSeparatorIfNeeded(Items);
			Items.Append(ItemSet);
		}
	}

	if (SelectedFocusable)
	{
		const auto ItemSet = SelectedFocusable->CreateContextActionsSelected(FocusableUnderMouse);
		if (!ItemSet.IsEmpty())
		{
			UContextMenuSeparator::AddSeparatorIfNeeded(Items);
			Items.Append(ItemSet);
		}
	}

	if (!Items.IsEmpty())
	{
		CurrentContextMenu = NewObject<UContextMenu>(this, ContextMenuClass);
		CurrentContextMenu->SetOwningPlayer(this);
		CurrentContextMenu->AddToPlayerScreen(1000);
	
		FVector2D MousePos;
		GetMousePosition(MousePos.X, MousePos.Y);
		CurrentContextMenu->SetPositionInViewport(MousePos);

		for (auto Item : Items)
		{
			if (auto Action = Cast<UContextMenuAction>(Item))
			{
				Action->OwningContextMenu = CurrentContextMenu;
			}
				
			CurrentContextMenu->AddItem(Item);
		}
	}
}

void AStarisPlayerController::SelectFocusable(IFocusable* NewFocusable)
{
	if (SelectedFocusable == NewFocusable) return;
	
	if (SelectedFocusable)
	{
		SelectedFocusable->OnDeselected();
	}
	
	SelectedFocusable = NewFocusable;

	SelectedFocusableChanged.Broadcast(SelectedFocusable);
	SelectedFocusableChanged_K2.Broadcast(Cast<UObject>(SelectedFocusable));

	if (SelectedFocusable)
	{
		SelectedFocusable->OnSelected();
	}
}

void AStarisPlayerController::SelectFocusable_K2(const TScriptInterface<IFocusable>& NewFocusable)
{
	SelectFocusable(NewFocusable.GetInterface());
}

void AStarisPlayerController::RequireGlobalPlane(IGlobalPlaneConsumer* GlobalPlaneConsumer)
{
	GlobalPlaneConsumers.Add(GlobalPlaneConsumer);
}

void AStarisPlayerController::FreeGlobalPlane(IGlobalPlaneConsumer* GlobalPlaneConsumer)
{
	GlobalPlaneConsumers.Remove(GlobalPlaneConsumer);
}

void AStarisPlayerController::SwitchBuildMode(bool NewState)
{
	if (NewState && !BuildMode)
	{
		BuildMode = NewObject<UBuildMode>(this);
		RequireGlobalPlane(BuildMode);
	}
	else if (!NewState && BuildMode)
	{
		FreeGlobalPlane(BuildMode);
		BuildMode = nullptr;
	}
}

void AStarisPlayerController::PlayNextMusic()
{
	if (!MusicPlaySet.IsEmpty())
	{
		USoundBase* Music;
		if (MusicPlaySet.Num() > 1)
		{
			auto MusicToPlay = MusicPlaySet;
			MusicToPlay.Remove(MusicPlayer->GetSound());
			Music = MusicToPlay[FMath::Rand() % MusicToPlay.Num()];
		}
		else
		{
			Music = MusicPlaySet[0];
		}
		MusicPlayer->SetSound(Music);
		MusicPlayer->Play();
	}
}

void AStarisPlayerController::EmpireSystemChanged(USystem* System, bool TakenOrLost)
{
	//UGameplayStatics::PlaySound2D(this, NotificationSound);
	if (TakenOrLost)
	{
		PlayAdvisorPhrase(ADVISOR_SystemTaken);
	}
	else
	{
		PlayAdvisorPhrase(ADVISOR_SystemLost);
	}
}

void AStarisPlayerController::PlayAdvisorPhrase(const FName& Key)
{
	if (const auto Phrase = AdvisorPhrases.FindRef(Key))
	{
		UGameplayStatics::PlaySound2D(this, Phrase);
	}
}
