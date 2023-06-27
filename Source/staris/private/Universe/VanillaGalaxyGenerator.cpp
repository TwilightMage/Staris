// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/VanillaGalaxyGenerator.h"

#include "StarisStatics.h"
#include "Empire/VanillaResourceTypeProperties.h"
#include "Game/StarisGameInstance.h"
#include "Internationalization/StringTableCore.h"
#include "Internationalization/StringTableRegistry.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/GalaxySettingsManager.h"
#include "Universe/LetterNames.h"
#include "Universe/VanillaGalaxyGeneratorContext.h"
#include "Universe/VanillaGalaxySettings.h"

FName GenerateId(int32 Seed, TSet<FName>& UsedIDs)
{
	const static FString CharSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	FRandomStream Random(Seed);
	FString Result;
	
	do
	{
		Result = "";
		for (int32 i = 0; i < 6; i++)
		{
			Result += CharSet[Random.RandRange(0, CharSet.Len() - 1)];
		}	
	}
	while (UsedIDs.Contains(FName(Result)));

	UsedIDs.Add(FName(Result));
	return FName(Result);
}

template<typename T>
T GetRandomSetItem(const TSet<T>& Container, FRandomStream& Random) { return Container[FSetElementId::FromInteger(Random.RandRange(0, Container.Num() - 1))]; }

template<typename T>
T GetRandomArrayItem(const TArray<T>& Container, FRandomStream& Random) { return Container[Random.RandRange(0, Container.Num() - 1)]; }

void UVanillaGalaxyGenerator::GenerateGalaxy(FGalaxyMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	if (const auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		// Fetch and setup vanilla generator context
		const auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		VanillaContext->CurrentGalaxy = &Data;

		// Fill pool with star names
		const FStringTableConstPtr StringTable = FStringTableRegistry::Get().FindStringTable("StarNames");
		if (StringTable.IsValid())
		{
			StringTable->EnumerateSourceStrings([&](const FString& InKey, const FString& InSourceString) -> bool
			{
				VanillaContext->AvailableStarNames.Add(InKey);
				return true;
			});
		}

		// Cache mineral resource types
		for (auto ResourceType : GameInstance->GetResourceTypeDatabase()->GetAllRecords())
		{
			auto VanillaResourceProps = ResourceType.Value->GetOrCreateComponent<UVanillaResourceTypeProperties>();
			if (!VanillaResourceProps->MineralDensityPerLayer.IsEmpty())
			{
				VanillaContext->MineralResourceTypes.Add({
					ResourceType.Value,
					VanillaResourceProps
				});
			}
		}

		// Setup star type distribution
		for (auto& DistributionEntry : Settings->StarTypeDistribution)
		{
			VanillaContext->StarTypeDistribution.AddDistribution(GameInstance->GetStarTypeDatabase()->GetOrCreateRecord(DistributionEntry.Key), DistributionEntry.Value);
		}

		// Setup star count per system distribution
		VanillaContext->StarCountDistribution = FRandomDistribution(Settings->StarCountDistribution);

		// Setup random stream for current galaxy
		const auto Seed = Settings->Seed * SubSeed;
		FRandomStream Random(Seed);

		// Fill in properties
		Data.Seed = SubSeed;
		Data.Id = GenerateId(Seed, VanillaContext->UsedIDs);
		Data.Systems.SetNum(Settings->SystemCount);

		// Generate systems
		for (auto& System : Data.Systems)
		{
			GenerateSystem(System, Random.FRand() * MAX_int32, SettingsManager, Context);
		}

		VanillaContext->CurrentGalaxy = nullptr;
	}
}

void UVanillaGalaxyGenerator::GenerateSystem(FSystemMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	if (const auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		// Fetch and setup vanilla generator context
		const auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		VanillaContext->CurrentSystem = &Data;

		// Setup random stream for current system
		const auto Seed = Settings->Seed * SubSeed;
		FRandomStream Random(Seed);

		// Make current system black hole by a chance
		VanillaContext->IsBlackHole = Random.FRand() < Settings->BlackHoleChance;

		// Pick name for current system
		if (!VanillaContext->AvailableStarNames.IsEmpty())
		{
			auto UsedStarName = GetRandomArrayItem(VanillaContext->AvailableStarNames, Random);
			VanillaContext->AvailableStarNames.Remove(UsedStarName);
			Data.Title = FText::FromStringTable("StarNames", UsedStarName, EStringTableLoadingPolicy::FindOrFullyLoad).ToString();
		}

		// Fill in properties
		Data.Id = GenerateId(Seed, VanillaContext->UsedIDs);
		Data.Seed = SubSeed;
		Data.Stars.SetNum(VanillaContext->IsBlackHole ? 1 : VanillaContext->StarCountDistribution.PickRandomFromStream(Random));
		Data.Planets.SetNum(VanillaContext->IsBlackHole ? 0 : Random.RandRange(Settings->PlanetAmountRange.Min, Settings->PlanetAmountRange.Max));

		// Pick location for current system

		/*float Angle = Random.FRand();
		float Dist = Random.FRand();
		Dist += FMath::Cos(Dist * 3.14) * 0.1;
		Data.Location = FVector(FMath::Sin(3.14 * 2 * Angle) * 50000 * Dist, FMath::Cos(3.14 * 2 * Angle) * 50000 * Dist, Random.FRand() * 1000 - 500) * 5;*/
		
		if (Settings->SystemDistributionMap)
		{
			auto& Mip = Settings->SystemDistributionMap->GetPlatformData()->Mips[0];
			const FByteBulkData& RawImageData = Mip.BulkData;
			const FColor* FormatedImageData = static_cast<const FColor*>(RawImageData.LockReadOnly());
		
			bool PositionOk = false;
			while (true)
			{
				FVector PosAlpha = FVector(Random.FRand(), Random.FRand(), Random.FRand());
				FColor PixelColor = FormatedImageData[(int32)(Mip.SizeY * PosAlpha.Y) * Mip.SizeX + (int32)(Mip.SizeX * PosAlpha.X)];
				
				float Chance = PixelColor.R / (float)MAX_uint8;
				if (Random.FRand() < Chance)
				{
					Data.Location = FVector(
						-Settings->GalaxyRadius + Settings->GalaxyRadius * 2 * PosAlpha.X,
						-Settings->GalaxyRadius + Settings->GalaxyRadius * 2 * PosAlpha.Y,
						-Settings->GalaxyHeight / 2 + Settings->GalaxyHeight * PosAlpha.Z
					);
					
					break;
				}
			}

			RawImageData.Unlock();
		}
		else
		{
			Data.Location = FVector(
				-Settings->GalaxyRadius + Settings->GalaxyRadius * 2 * Random.FRand(),
				-Settings->GalaxyRadius + Settings->GalaxyRadius * 2 * Random.FRand(),
				-Settings->GalaxyHeight / 2 + Settings->GalaxyHeight * Random.FRand()
			);
		}

		//// Rearrange our planet orbits
		//if (!Data.Planets.IsEmpty())
		//{
		//	for (int32 i = 1; i < Data.Planets.Num(); i++)
		//	{
		//		for (int32 j = i - 1; j >= 0; --j)
		//		{
		//			if (Data.Planets[j + 1].OrbitDistance < Data.Planets[j].OrbitDistance)
		//			{
		//				Swap(Data.Planets[j + 1], Data.Planets[j]);
		//			}
		//		}
		//	}
		//	
		//	for (int32 i = 0; i < 10; i++)
		//	{
		//		TArray<float> Offsets;
		//		Offsets.SetNumZeroed(Data.Planets.Num());
		//
		//		for (int32 j = 0; j < Data.Planets.Num() - 1; j++)
		//		{
		//			Offsets[j] +=  Data.Planets[j + 1].OrbitDistance - Data.Planets[j].OrbitDistance;
		//		}
		//
		//		for (int32 j = 1; j < Data.Planets.Num(); j++)
		//		{
		//			Offsets[j] -=  Data.Planets[j].OrbitDistance - Data.Planets[j - 1].OrbitDistance;
		//		}
		//
		//		for (int32 j = 0; j < Data.Planets.Num(); j++)
		//		{
		//			Data.Planets[j].OrbitDistance += Offsets[j] * 0.05;
		//		}
		//	}
		//}

		// Generate stars in current system
		VanillaContext->CurrentStarIndex = 0;
		for (auto& Star : Data.Stars)
		{
			GenerateStar(Star, Random.FRand() * MAX_int32, SettingsManager, Context);
			VanillaContext->CurrentStarIndex++;
		}

		// Generate planets in current system
		VanillaContext->CurrentPlanetIndex = 0;
		for (auto& Planet : Data.Planets)
		{
			GeneratePlanet(Planet, Random.FRand() * MAX_int32, SettingsManager, Context);
			VanillaContext->CurrentPlanetIndex++;
		}

		VanillaContext->CurrentSystem = nullptr;
	}
}

void UVanillaGalaxyGenerator::GenerateStar(FStarMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		VanillaContext->CurrentStar = &Data;
		
		auto Seed = Settings->Seed * SubSeed;

		FRandomStream Random(Seed);

		if (VanillaContext->CurrentSystem)
		{
			if (VanillaContext->CurrentSystem->Stars.Num() > 1)
			{
				Data.Title = VanillaContext->CurrentSystem->Title + "-" + GetLetterName(VanillaContext->CurrentStarIndex);
			}
			else
			{
				Data.Title = VanillaContext->CurrentSystem->Title;
			}
		}
		
		Data.Id = GenerateId(Seed, VanillaContext->UsedIDs);
		Data.Seed = SubSeed;

		if (VanillaContext->IsBlackHole) Data.Type = GameInstance->GetStarTypeDatabase()->GetOrCreateRecord(FStarMetaData::TYPE_Black_Hole);
		else Data.Type = VanillaContext->StarTypeDistribution.PickRandomFromStream(Random);

		if (VanillaContext->CurrentSystem->Stars.Num() == 1)
		{
			Data.Location = FVector::Zero();
		}
		else
		{
			float Angle = (VanillaContext->CurrentStarIndex / (float)VanillaContext->CurrentSystem->Stars.Num()) * (3.14 * 2);
			Data.Location = FVector(FMath::Cos(Angle), FMath::Sin(Angle), 0) * 70;
		}
		Data.Scale = Random.FRandRange(0.5, 1.0);

		VanillaContext->CurrentStar = nullptr;
	}
}

void UVanillaGalaxyGenerator::GeneratePlanet(FPlanetMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		VanillaContext->CurrentPlanet = &Data;
		
		auto Seed = Settings->Seed * SubSeed;

		FRandomStream Random(Seed);

		Data.Id = GenerateId(Seed, VanillaContext->UsedIDs);
		Data.Seed = SubSeed;
		Data.Layers.SetNum(Settings->LayerNum);
		Data.Scale = Random.FRandRange(0.5, 1.0);
		Data.OrbitOffset = Random.FRand();
		Data.OrbitPoint = FVector::Zero();
		Data.OrbitSpeed = Random.FRandRange(0.5, 2.0);

		float OrbitAlphaStep = 1 / (float)Settings->PlanetAmountRange.Max;
		float OrbitAlpha = OrbitAlphaStep * VanillaContext->CurrentPlanetIndex + OrbitAlphaStep * (Random.FRand() * 0.2f - 0.1f);
		Data.OrbitDistance = FMath::Lerp(Settings->PlanetOrbitRange.Min, Settings->PlanetOrbitRange.Max, OrbitAlpha);

		const int32 TemperatureOffset = FMath::Lerp(Settings->TemperatureGlobalRange.Max, Settings->TemperatureGlobalRange.Min, OrbitAlpha);
		Data.Temperature = TemperatureOffset + Settings->TemperatureRandomSpread * (Random.FRand() * 2 - 1);

		VanillaContext->PlanetMineralDistribution = {};
		for (auto& Mineral : VanillaContext->MineralResourceTypes)
		{
			VanillaContext->PlanetMineralDistribution.Add({
				Mineral.Record,
				Mineral.VanillaProps,
				Random.FRand() * 2
			});
		}
		VanillaContext->PlanetMineralDistribution.Add({nullptr, nullptr, 1});

		VanillaContext->CurrentPlanetLayerIndex = 0;
		for (auto& PlanetLayer : Data.Layers)
		{
			GeneratePlanetLayer(PlanetLayer, Random.FRand() * MAX_int32, SettingsManager, Context);
			VanillaContext->CurrentPlanetLayerIndex++;
		}

		VanillaContext->CurrentPlanet = nullptr;
	}
}

void UVanillaGalaxyGenerator::GeneratePlanetLayer(FPlanetLayer& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		
		auto Seed = Settings->Seed * SubSeed;
		FRandomStream Random(Seed);

		int32 LayerSize = VanillaContext->CurrentPlanet->Scale * Settings->LayerSize;
		Data.ResourceTiles.Reserve(LayerSize);
		FRandomDistribution<UCompositeRecord*> LayerMineralDistribution;
		for (int32 i = 0; i < VanillaContext->PlanetMineralDistribution.Num(); i++)
		{
			auto& Mineral = VanillaContext->PlanetMineralDistribution[i];
			
			if (Mineral.VanillaProps && VanillaContext->CurrentPlanetLayerIndex >= Mineral.VanillaProps->MineralDensityPerLayer.Num()) continue;
			
			LayerMineralDistribution.AddDistribution(Mineral.Record, Mineral.Density * (Mineral.VanillaProps ? Mineral.VanillaProps->MineralDensityPerLayer[VanillaContext->CurrentPlanetLayerIndex] : 1));
		}

		for (int32 i = 0; i < LayerSize; i++)
		{
			if (auto Resource = LayerMineralDistribution.PickRandomFromStream(Random))
			{
				Data.ResourceTiles.Add(Resource);
			}
		}
	}
}
