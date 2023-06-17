// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisPlayerController.h"

#include "Empire/Empire.h"
#include "Focusable.h"
#include "Game/StarisPlayerPawn.h"
#include "UI/ToolTip.h"
#include "Universe/System.h"


// Sets default values
AStarisPlayerController::AStarisPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
}

void AStarisPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FHitResult Hit;
	GetHitResultUnderCursorByChannel(TraceTypeQuery1, false, Hit);

	IFocusable* NewTargetedFocusable = nullptr;
	
	if (auto Component = Hit.Component.Get())
	{
		if (auto Focusable = Cast<IFocusable>(Component))
		{
			NewTargetedFocusable = Focusable;
		}
	}

	if (NewTargetedFocusable != TargetedFocusable)
	{
		if (CurrentToolTip)
		{
			CurrentToolTip->RemoveFromParent();
		}
		
		TargetedFocusable = NewTargetedFocusable;

		if (TargetedFocusable)
		{
			CurrentToolTip = TargetedFocusable->CreateToolTip();
			CurrentToolTip->SetOwningPlayer(this);
			CurrentToolTip->AddToPlayerScreen(1000);
				
			TargetedFocusable->SetupToolTip(CurrentToolTip);
		}
	}

	FVector2D MousePos;
	GetMousePosition(MousePos.X, MousePos.Y);

	if (CurrentToolTip)
	{
		CurrentToolTip->SetPositionInViewport(MousePos + FVector2D(15, 15));
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

void AStarisPlayerController::OnEmpireAssigned(UEmpire* Empire)
{
	OwnedEmpire = Empire;

	if (auto StarisPlayerPawn = Cast<AStarisPlayerPawn>(GetPawn()))
	{
		StarisPlayerPawn->MoveToTarget(OwnedEmpire->GetOwnedSystems()[0]->GetRootComponent(), false);
	}
}
