// Fill out your copyright notice in the Description page of Project Settings.


#include "Empire.h"

#include "StarisStatics.h"
#include "Game/StarisController.h"
#include "Game/StarisPlayerController.h"
#include "Universe/System.h"

FText UEmpire::GetTitle() const
{
	return NSLOCTEXT("-", "-", "Great Foxik Empire");
}

void UEmpire::AssignController(IStarisController* Controller)
{
	if (!Controller) return;
	if (OwningControllers.Contains(Controller)) return;
	
	OwningControllers.Add(Controller);
	Controller->OnEmpireAssigned(this);
	UE_LOG(LogStaris, Log, TEXT("Empire \"%s\" Assigned to %s %s"), *GetTitle().ToString(), Controller->IsPlayer() ? TEXT("player") : TEXT("bot"), *Controller->GetControllerName());
}

void UEmpire::RemoveController(IStarisController* Controller)
{
	if (!Controller) return;
	
	OwningControllers.Remove(Controller);
	Controller->OnEmpireAssigned(nullptr);
}

bool UEmpire::TryAssignPlayer(AStarisPlayerController* Player)
{
	for (auto Controller : OwningControllers)
	{
		if (Cast<APlayerController>(Controller))
		{
			return false;
		}
	}

	for (int32 i = OwningControllers.Num() - 1; i >= 0; i--)
	{
		RemoveController(OwningControllers[i]);
	}

	AssignController(Player);

	return true;
}

void UEmpire::TakeSystem(ASystem* System)
{
	if (auto OldEmpire = System->OwningEmpire.Get())
	{
		OldEmpire->OwnedSystems.Remove(System);
		OldEmpire->SystemChanged.Broadcast(System, false);

		UE_LOG(LogStaris, Log, TEXT("System %s was lost by empire \"%s\""), *System->Id.ToString(), *OldEmpire->GetTitle().ToString());
	}

	System->OwningEmpire = this;
	
	OwnedSystems.Add(System);
	SystemChanged.Broadcast(System, true);

	UE_LOG(LogStaris, Log, TEXT("System %s was taken by empire \"%s\""), *System->Id.ToString(), *GetTitle().ToString());
}

void UEmpire::SystemTaken(ASystem* System)
{
	OwnedSystems.Add(System);
}

void UEmpire::SystemLost(ASystem* System)
{
	OwnedSystems.Remove(System);
}
