// Fill out your copyright notice in the Description page of Project Settings.


#include "Empire/Empire.h"

#include "StarisStatics.h"
#include "Game/StarisController.h"
#include "Game/StarisGameInstance.h"
#include "Game/StarisPlayerController.h"
#include "UI/ContextMenu.h"
#include "Universe/Planet.h"
#include "Universe/Star.h"
#include "Universe/System.h"

bool UEmpire::IsAssignedToPlayer() const
{
	for (auto Controller : OwningControllers)
	{
		if (Cast<APlayerController>(Controller))
		{
			return true;
		}
	}

	return false;
}

void UEmpire::TakeSystem(ASystem* System)
{
	if (auto OldEmpire = System->OwningEmpire.Get())
	{
		OldEmpire->OwnedSystems.Remove(System);
		OldEmpire->SystemChanged.Broadcast(System, false);

		UE_LOG(LogStaris, Log, TEXT("System %s was lost by empire \"%s\""), *System->Id.ToString(), *OldEmpire->Title);
	}

	System->OwningEmpire = this;
	
	OwnedSystems.Add(System);
	SystemChanged.Broadcast(System, true);

	UE_LOG(LogStaris, Log, TEXT("System %s was taken by empire \"%s\""), *System->Id.ToString(), *Title);
}

TArray<UContextMenuItem*> UEmpire::CreateContextActions(IFocusable* HoveredFocusable, IFocusable* SelectedFocusable)
{
	TArray<UContextMenuItem*> Items;
	
	if (auto Star = Cast<UStar>(HoveredFocusable))
	{
		if (auto System = Star->GetSystem())
		{
			if (System->GetOwningEmpire() == this)
			{
				Items.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Empire", "Rename System", "Rename System"), FText(), nullptr, nullptr));
				Items.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Empire", "Rename Star", "Rename Star"), FText(), nullptr, nullptr));
			}
			else
			{
				if (UStarisGameInstance::DebugToolsEnabled)
				{
					Items.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Empire", "(DEBUG) Take System", "(DEBUG) Take System"), FText(), FContextMenuActionDelegate::CreateLambda([this, System]()
					{
						TakeSystem(System);
					}), nullptr));
				}
			}
		}
	}

	return Items;
}

void UEmpire::ClearControllerList()
{
	OwningControllers.Empty();
}

void UEmpire::ControllerAssigned(IStarisController* Controller)
{
	OwningControllers.Add(Controller);
	
	UE_LOG(LogStaris, Log, TEXT("Empire \"%s\" Assigned to %s %s"), *Title, Controller->IsPlayer() ? TEXT("player") : TEXT("bot"), *Controller->GetControllerName());
}

void UEmpire::ControllerRemoved(IStarisController* Controller)
{
	OwningControllers.Remove(Controller);
}

void UEmpire::SystemTaken(ASystem* System)
{
	OwnedSystems.Add(System);
}

void UEmpire::SystemLost(ASystem* System)
{
	OwnedSystems.Remove(System);
}
