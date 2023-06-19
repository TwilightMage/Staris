// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisPlayerController.h"

#include "Empire/Empire.h"
#include "Focusable.h"
#include "Components/AudioComponent.h"
#include "Game/StarisPlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "UI/ContextMenu.h"
#include "UI/ToolTip.h"
#include "Universe/Planet.h"
#include "Universe/System.h"


// Sets default values
AStarisPlayerController::AStarisPlayerController()
{
	MusicPlayer = CreateDefaultSubobject<UAudioComponent>("Music Player");
	MusicPlayer->SetupAttachment(GetRootComponent());

	MusicPlayer->OnAudioFinishedNative.AddWeakLambda(this, [this](UAudioComponent*)
	{
		PlayNextSound();
	});
	
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
	bEnableTouchEvents = false;
}

void AStarisPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());

	PlayNextSound();
}

void AStarisPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult Hit;
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, Hit);

	IFocusable* NewTargetedFocusable = nullptr;
	
	if (auto Component = Hit.Component.Get())
	{
		LocationUnderMouse = Hit.Location;
		HasLocationUnderMouse = true;
		
		if (auto Focusable = Cast<IFocusable>(Component))
		{
			NewTargetedFocusable = Focusable;
		}
	}
	else
	{
		HasLocationUnderMouse = false;
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
			CurrentToolTip = FocusableUnderMouse->CreateToolTip();
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
		float MouseX;
		float MouseY;
		GetMousePosition(MouseX, MouseY);

		FVector2D DesiredToolTipSize = CurrentToolTip->GetDesiredSize();

		CurrentToolTip->SetPositionInViewport(FVector2D(FMath::Min(MouseX + 6, ViewportWidth - DesiredToolTipSize.X), FMath::Min(MouseY + 15, ViewportHeight - DesiredToolTipSize.Y)));
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

void AStarisPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (OwnedEmpire)
	{
		if (auto StarisPlayerPawn = Cast<AStarisPlayerPawn>(InPawn))
		{
			StarisPlayerPawn->MoveToTarget(OwnedEmpire->GetOwnedSystems()[0]->GetRootComponent(), false);
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
		OwnedEmpire->SystemChanged.RemoveAll(this);

		NotifyControllerRemoved(OwnedEmpire);

		OwnedEmpire = nullptr;
	}
	
	OwnedEmpire = Empire;

	if (OwnedEmpire)
	{
		NotifyControllerAssigned(OwnedEmpire);
		
		OwnedEmpire->SystemChanged.AddUObject(this, &AStarisPlayerController::EmpireSystemChanged);
		
		if (auto StarisPlayerPawn = Cast<AStarisPlayerPawn>(GetPawn()))
		{
			StarisPlayerPawn->MoveToTarget(OwnedEmpire->Capital, false);
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

	TArray<TArray<UContextMenuItem*>> Items;
	
	if (OwnedEmpire)
	{
		const auto ItemSet = OwnedEmpire->CreateContextActions(FocusableUnderMouse, SelectedFocusable);
		if (!ItemSet.IsEmpty()) Items.Add(ItemSet);
	}

	if (FocusableUnderMouse)
	{
		const auto ItemSet = FocusableUnderMouse->CreateContextActionsHovered(SelectedFocusable);
		if (!ItemSet.IsEmpty()) Items.Add(ItemSet);
	}

	if (SelectedFocusable)
	{
		const auto ItemSet = SelectedFocusable->CreateContextActionsSelected(FocusableUnderMouse);
		if (!ItemSet.IsEmpty()) Items.Add(ItemSet);
	}

	if (!Items.IsEmpty())
	{
		CurrentContextMenu = NewObject<UContextMenu>(this, ContextMenuClass);
		CurrentContextMenu->SetOwningPlayer(this);
		CurrentContextMenu->AddToPlayerScreen(1000);
	
		FVector2D MousePos;
		GetMousePosition(MousePos.X, MousePos.Y);
		CurrentContextMenu->SetPositionInViewport(MousePos);

		for (int32 i = 0; i < Items.Num(); i++)
		{
			if (i > 0) CurrentContextMenu->AddSeparator();

			for (auto& Item : Items[i])
			{
				if (auto Action = Cast<UContextMenuAction>(Item))
				{
					Action->OwningContextMenu = CurrentContextMenu;
				}
				
				CurrentContextMenu->AddItem(Item);
			}
		}
	}
}

void AStarisPlayerController::PlayNextSound()
{
	if (!MusicPlaySet.IsEmpty())
	{
		MusicPlayer->SetSound(MusicPlaySet[FMath::Rand() % MusicPlaySet.Num()]);
		MusicPlayer->Play();
	}
}

void AStarisPlayerController::EmpireSystemChanged(ASystem* System, bool TakenOrLost)
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
