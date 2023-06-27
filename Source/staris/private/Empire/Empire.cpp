// Fill out your copyright notice in the Description page of Project Settings.


#include "Empire/Empire.h"

#include "StarisStatics.h"
#include "Empire/Colony.h"
#include "Empire/EmpirePlanetKnowledge.h"
#include "Game/StarisController.h"
#include "Game/StarisGameInstance.h"
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

void UEmpire::TakeSystem(USystem* System)
{
	if (auto OldEmpire = System->OwningEmpire.Get())
	{
		OldEmpire->OwnedSystems.Remove(System);
		OldEmpire->OnSystemChanged.Broadcast(System, false);

		UE_LOG(LogStaris, Log, TEXT("System %s was lost by empire \"%s\""), *System->Id.ToString(), *OldEmpire->Title);
	}

	System->OwningEmpire = this;
	
	OwnedSystems.Add(System);
	OnSystemChanged.Broadcast(System, true);

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

UEmpirePlanetKnowledge* UEmpire::GetPlanetKnowledge(UPlanet* Planet) const
{
	return PlanetKnowledgeDatabase.FindRef(Planet);
}

UEmpirePlanetKnowledge* UEmpire::GetOrCreatePlanetKnowledge(UPlanet* Planet)
{
	auto& Knowledge = PlanetKnowledgeDatabase.FindOrAdd(Planet);
	if (!Knowledge)
	{
		Knowledge = NewObject<UEmpirePlanetKnowledge>(this);
		Knowledge->Setup(Planet);

		OnPlanetKnowledgeAdded.Broadcast(Knowledge);
		OnPlanetKnowledgeAdded_K2.Broadcast(Knowledge);
	}
	return Knowledge;
}

void UEmpire::MonthPassed()
{
	TMap<UColony*, TMap<UCompositeRecord*, int32>> ColonySpends;
	BalanceUpdateData BalanceUpdate;
	
	for (auto System : OwnedSystems)
	{
		for (auto Planet : System->GetPlanets())
		{
			if (auto Colony = Planet->GetColony())
			{
				auto ColonyBalance = Colony->GetResourceBalance();
				for (auto& ColonyBalanceEntry : ColonyBalance)
				{
					auto& BalanceUpdateEntry = BalanceUpdate.FindOrAdd(ColonyBalanceEntry.Key);
					BalanceUpdateEntry.Balance += ColonyBalanceEntry.Value;
					if (ColonyBalanceEntry.Value > 0)
					{
						BalanceUpdateEntry.Amount = CurrentBalance.FindOrAdd(ColonyBalanceEntry.Key) += ColonyBalanceEntry.Value;
					}
					else if (ColonyBalanceEntry.Value < 0)
					{
						ColonySpends.FindOrAdd(Colony).Add(ColonyBalanceEntry);
					}
				}
			}
		}
	}

	for (auto& ColonySpend : ColonySpends)
	{
		for (auto& Spend : ColonySpend.Value)
		{
			auto& BalanceEntryAmount = CurrentBalance.FindOrAdd(Spend.Key);
			if (BalanceEntryAmount + Spend.Value < 0)
			{
				//Spend.Value - *BalanceEntry; // failed to fulfill
				BalanceEntryAmount = 0;
			}
			else
			{
				BalanceEntryAmount += Spend.Value;
			}
			BalanceUpdate.FindOrAdd(Spend.Key).Amount = BalanceEntryAmount;
		}
	}

	OnBalanceUpdated.Broadcast(BalanceUpdate);
	LatestBalanceUpdate = BalanceUpdate;
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

void UEmpire::SystemTaken(USystem* System)
{
	OwnedSystems.Add(System);
}

void UEmpire::SystemLost(USystem* System)
{
	OwnedSystems.Remove(System);
}
