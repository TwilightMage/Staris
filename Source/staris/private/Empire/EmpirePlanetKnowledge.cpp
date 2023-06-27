// Copyright DragonCap. All rights reserved.


#include "Empire/EmpirePlanetKnowledge.h"

#include "StarisStatics.h"
#include "Universe/Planet.h"

bool UEmpirePlanetKnowledge::Setup(UPlanet* Planet)
{
	if (!TargetPlanet)
	{
		TargetPlanet = Planet;
		UnlockedTileIndices.SetNum(Planet->GetLayers().Num());

		return true;
	}

	return false;
}

void UEmpirePlanetKnowledge::UnlockTile(int32 Layer, int32 TileIndex)
{
	if (TargetPlanet && TargetPlanet->GetLayers().IsValidIndex(Layer) && TargetPlanet->GetLayers()[Layer].ResourceTiles.IsValidIndex(TileIndex))
	{
		bool Has = false;
		UnlockedTileIndices[Layer].Add(TileIndex, &Has);
		if (!Has)
		{
			ResourcesCached.FindOrAdd(TargetPlanet->GetLayers()[Layer].ResourceTiles[TileIndex])++;

			OnUpdated.Broadcast();
			OnUpdated_K2.Broadcast();
		}
	}
}

void UEmpirePlanetKnowledge::UnlockRandomTile(int32 Layers, const TArray<UCompositeRecord*>& WhitelistResources)
{
	if (Layers <= 0) return;
	
	auto& PlanetLayers = TargetPlanet->GetLayers();
	if (TargetPlanet)
	{
		TArray<TPair<int32, int32>> IndexPool;
		for (int32 i = 0; i < PlanetLayers.Num() && i < Layers; i++)
		{
			for (int32 j = 0; j < PlanetLayers[i].ResourceTiles.Num(); j++)
			{
				if (!UnlockedTileIndices[i].Contains(j))
				{
					IndexPool.Add({i, j});
				}
			}
		}

		if (IndexPool.IsEmpty()) return;

		const auto& SelectedIndex = GetRandomArrayItem(IndexPool, FMath::Rand());
		
		UnlockedTileIndices[SelectedIndex.Key].Add(SelectedIndex.Value);
		
		ResourcesCached.FindOrAdd(PlanetLayers[SelectedIndex.Key].ResourceTiles[SelectedIndex.Value])++;

		OnUpdated.Broadcast();
		OnUpdated_K2.Broadcast();
	}
}

void UEmpirePlanetKnowledge::UnlockLayer(int32 Layer)
{
	if (TargetPlanet && TargetPlanet->GetLayers().IsValidIndex(Layer))
	{
		bool Updated = false;
		
		for (int32 i = 0; i < TargetPlanet->GetLayers()[Layer].ResourceTiles.Num(); i++)
		{
			bool Has = false;
			UnlockedTileIndices[Layer].Add(i, &Has);
			if (!Has)
			{
				ResourcesCached.FindOrAdd(TargetPlanet->GetLayers()[Layer].ResourceTiles[i])++;
				Updated = true;
			}
		}

		if (Updated)
		{
			OnUpdated.Broadcast();
			OnUpdated_K2.Broadcast();
		}
	}
}
