// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

template<typename ItemType>
struct FRandomDistribution
{
	FRandomDistribution()
	{}
	
	FRandomDistribution(const TMap<ItemType, float>& Distributions)
		: Distributions(Distributions)
	{
		RecalculateTotalDistribution();
	}
	
	void AddDistribution(const ItemType& Item, float Distribution)
	{
		Distributions.Add(Item, Distribution);

		RecalculateTotalDistribution();
	}

	void RemoveDistribution(const ItemType& Item)
	{
		Distributions.Remove(Item);

		RecalculateTotalDistribution();
	}

	ItemType PickRandom()
	{
		float Selection = FMath::FRand();

		float SelectionDistribution = TotalDistribution * Selection;
		for (auto& DistributionEntry : Distributions)
		{
			if (SelectionDistribution <= DistributionEntry.Value) return DistributionEntry.Key;
			SelectionDistribution -= DistributionEntry.Value;
		}

		return ItemType();
	}
	
	ItemType PickRandomFromStream(FRandomStream& Stream)
	{
		float Selection = Stream.FRand();

		float SelectionDistribution = TotalDistribution * Selection;
		for (auto& DistributionEntry : Distributions)
		{
			if (SelectionDistribution <= DistributionEntry.Value) return DistributionEntry.Key;
			SelectionDistribution -= DistributionEntry.Value;
		}

		return ItemType();
	}

private:
	void RecalculateTotalDistribution()
	{
		TotalDistribution = 0;
		for (auto& DistributionEntry : Distributions)
		{
			TotalDistribution += DistributionEntry.Value;
		}
	}
	
	float TotalDistribution = 0;
	TMap<ItemType, float> Distributions;
};
