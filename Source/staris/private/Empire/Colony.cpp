// Fill out your copyright notice in the Description page of Project Settings.


#include "Empire/Colony.h"

#include "StarisStatics.h"
#include "Empire/Building.h"
#include "Empire/VanillaBuildingTypeProperties.h"
#include "Game/StarisGameInstance.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/Galaxy.h"

UColony::UColony()
{
	static auto AddBuildingCommand = IConsoleManager::Get().RegisterConsoleCommand(TEXT("Staris.CreatePlanetBuilding"), TEXT("<Planet Id> <Building Id> - Add building to the colony on planet, if colony present"), FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (Args.Num() >= 2)
		{
			if (auto Galaxy = GetActorOfClass<AGalaxy>(World))
			{
				if (auto Planet = Cast<UPlanet>(Galaxy->GetObjectById(FName(Args[0]))))
				{
					if (auto Colony = Planet->GetColony())
					{
						if (auto GameInstance = Cast<UStarisGameInstance>(UGameplayStatics::GetGameInstance(World)))
						{
							if (auto BuildingType = GameInstance->GetBuildingTypeDatabase()->GetRecord(FName(Args[1])))
							{
								Colony->AddBuilding(BuildingType);
							}
						}
					}
				}
			}
		}
	}));
}

TMap<UCompositeRecord*, int32> UColony::GetResourceBalance() const
{
	return {};
}

int32 UColony::getTotalPopulationAmount() const
{
	int32 Result = 0;
	for (const auto& Pop : Population)
	{
		Result += Pop.Value;
	}

	return Result;
}

void UColony::MonthPassed()
{
	
}

UBuilding* UColony::AddBuilding(UCompositeRecord* BuildingType)
{
	auto Building = NewObject<UBuilding>();
	Building->BuildingType = BuildingType;

	Buildings.Add(Building);
	RecalculateStats();

	return Building;
}

void UColony::RecalculateStats()
{
	Stats = FColonyStats();

	TArray<UVanillaBuildingTypeProperties*> Types;

	for (auto Building : Buildings)
	{
		auto Type = Building->BuildingType->GetOrCreateComponent<UVanillaBuildingTypeProperties>();
		Types.Add(Type);

		Stats.Flags.Append(Type->AddFlags);
	}

	for (auto Type : Types)
	{
		for (auto& RemoveFlag : Type->RemoveFlags)
		{
			Stats.Flags.Remove(RemoveFlag);
		}
	}

	for (auto Type : Types)
	{
		for (auto& ResourceBalance : Type->PreMultiplyAddIncomeResources)
		{
			Stats.ResourceBalance.FindOrAdd(ResourceBalance.Key) += ResourceBalance.Value;
		}
	}

	for (auto Type : Types)
	{
		for (auto& ResourceBalance : Type->MultiplyIncomeResources)
		{
			Stats.ResourceBalance.FindOrAdd(ResourceBalance.Key) *= ResourceBalance.Value;
		}
	}

	for (auto Type : Types)
	{
		for (auto& ResourceBalance : Type->PostMultiplyAddIncomeResources)
		{
			Stats.ResourceBalance.FindOrAdd(ResourceBalance.Key) += ResourceBalance.Value;
		}
	}

	TMap<UCompositeRecord*, float> Outcome;
	for (auto Type : Types)
	{
		for (auto& ResourceBalance : Type->PreMultiplyAddOutcomeResources)
		{
			Outcome.FindOrAdd(ResourceBalance.Key) += ResourceBalance.Value;
		}
	}

	for (auto Type : Types)
	{
		for (auto& ResourceBalance : Type->MultiplyOutcomeResources)
		{
			Outcome.FindOrAdd(ResourceBalance.Key) *= ResourceBalance.Value;
		}
	}

	for (auto Type : Types)
	{
		for (auto& ResourceBalance : Type->PostMultiplyAddOutcomeResources)
		{
			Outcome.FindOrAdd(ResourceBalance.Key) += ResourceBalance.Value;
		}
	}
	
	for (auto& ResourceBalance : Outcome)
	{
		Stats.ResourceBalance.FindOrAdd(ResourceBalance.Key) -= ResourceBalance.Value;
	}
}
