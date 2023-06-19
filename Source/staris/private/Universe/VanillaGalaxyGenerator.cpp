// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/VanillaGalaxyGenerator.h"

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
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		VanillaContext->CurrentGalaxy = &Data;
		
		FStringTableConstPtr StringTable = FStringTableRegistry::Get().FindStringTable("StarNames");
		if (StringTable.IsValid())
		{
			StringTable->EnumerateSourceStrings([&](const FString& InKey, const FString& InSourceString) -> bool
			{
				VanillaContext->AvailableStarNames.Add(InKey);
				return true;
			});
		}
		
		auto Seed = Settings->Seed * SubSeed;
		
		FRandomStream Random(Seed);

		Data.Id = GenerateId(Seed, VanillaContext->UsedIDs);
		Data.Systems.SetNum(Settings->SystemCount);

		for (auto& System : Data.Systems)
		{
			GenerateSystem(System, Random.FRand() * MAX_int32, SettingsManager, Context);
		}
	}
}

void UVanillaGalaxyGenerator::GenerateSystem(FSystemMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		VanillaContext->CurrentSystem = &Data;
		
		auto Seed = Settings->Seed * SubSeed;
		
		FRandomStream Random(Seed);

		VanillaContext->IsBlackHole = Random.FRand() < 0.1;

		if (!VanillaContext->AvailableStarNames.IsEmpty())
		{
			auto UsedStarName = GetRandomArrayItem(VanillaContext->AvailableStarNames, Random);
			VanillaContext->AvailableStarNames.Remove(UsedStarName);
			Data.Title = FText::FromStringTable("StarNames", UsedStarName, EStringTableLoadingPolicy::FindOrFullyLoad).ToString();
		}

		Data.Id = GenerateId(Seed, VanillaContext->UsedIDs);
		Data.Stars.SetNum(1);
		Data.Planets.SetNum(VanillaContext->IsBlackHole ? 0 : Random.RandRange(3, 7));

		float Angle = Random.FRand();
		float Dist = Random.FRand();

		Dist += FMath::Cos(Dist * 3.14) * 0.1;

		Data.Location = FVector(FMath::Sin(3.14 * 2 * Angle) * 50000 * Dist, FMath::Cos(3.14 * 2 * Angle) * 50000 * Dist, Random.FRand() * 1000 - 500) * 5;
		
		VanillaContext->CurrentStarIndex = 0;
		for (auto& Star : Data.Stars)
		{
			GenerateStar(Star, Random.FRand() * MAX_int32, SettingsManager, Context);
			VanillaContext->CurrentStarIndex++;
		}

		VanillaContext->CurrentPlanetIndex = 0;
		for (auto& Planet : Data.Planets)
		{
			GeneratePlanet(Planet, Random.FRand() * MAX_int32, SettingsManager, Context);
			VanillaContext->CurrentPlanetIndex++;
		}

		if (!Data.Planets.IsEmpty())
		{
			for (int32 i = 1; i < Data.Planets.Num(); i++)
			{
				for (int32 j = i - 1; j >= 0; --j)
				{
					if (Data.Planets[j + 1].OrbitDistance < Data.Planets[j].OrbitDistance)
					{
						Swap(Data.Planets[j + 1], Data.Planets[j]);
					}
				}
			}
			
			for (int32 i = 0; i < 10; i++)
			{
				TArray<float> Offsets;
				Offsets.SetNumZeroed(Data.Planets.Num());

				for (int32 j = 0; j < Data.Planets.Num() - 1; j++)
				{
					Offsets[j] +=  Data.Planets[j + 1].OrbitDistance - Data.Planets[j].OrbitDistance;
				}

				for (int32 j = 1; j < Data.Planets.Num(); j++)
				{
					Offsets[j] -=  Data.Planets[j].OrbitDistance - Data.Planets[j - 1].OrbitDistance;
				}

				for (int32 j = 0; j < Data.Planets.Num(); j++)
				{
					Data.Planets[j].OrbitDistance += Offsets[j] * 0.05;
				}
			}
		}
	}
}

void UVanillaGalaxyGenerator::GenerateStar(FStarMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		VanillaContext->CurrentStar = &Data;
		
		auto Seed = Settings->Seed * SubSeed;
		
		const static TSet<FName> StarTypes = {
			FStarMetaData::TYPE_Blue,
			FStarMetaData::TYPE_Red,
			FStarMetaData::TYPE_Yellow
		};

		FRandomStream Random(Seed);

		if (VanillaContext->CurrentSystem)
		{
			if (VanillaContext->CurrentSystem->Stars.Num() > 1 || true)
			{
				Data.Title = VanillaContext->CurrentSystem->Title + "-" + GetLetterName(VanillaContext->CurrentStarIndex);
			}
			else
			{
				Data.Title = VanillaContext->CurrentSystem->Title;
			}
		}
		Data.Id = GenerateId(Seed, VanillaContext->UsedIDs);

		if (VanillaContext->IsBlackHole) Data.Type = StarTypeDatabase->GetOrCreateRecord(FStarMetaData::TYPE_Black_Hole);
		else Data.Type = StarTypeDatabase->GetOrCreateRecord(GetRandomSetItem(StarTypes, Random));

		Data.Location = FVector::Zero();
		Data.Scale = Random.FRandRange(0.5, 1.0);
	}
}

void UVanillaGalaxyGenerator::GeneratePlanet(FPlanetMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	const float OrbitRange = 2000;
	const float OrbitOffset = 300;
	
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto VanillaContext = Context->GetOrCreateComponent<UVanillaGalaxyGeneratorContext>();
		VanillaContext->CurrentPlanet = &Data;
		
		auto Seed = Settings->Seed * SubSeed;
		
		const static TSet<FName> PlanetBiomes = {
			FPlanetMetaData::BIOME_Desert,
			FPlanetMetaData::BIOME_Forest,
			FPlanetMetaData::BIOME_Arctic
		};

		FRandomStream Random(Seed);

		Data.Id = GenerateId(Seed, VanillaContext->UsedIDs);
		Data.Biome = GetRandomSetItem(PlanetBiomes, Random);
		Data.Regions.SetNum(Random.RandRange(10, 20));
		Data.Scale = Random.FRandRange(0.5, 1.0);
		Data.OrbitOffset = Random.FRand();
		Data.OrbitPoint = FVector::Zero();
		Data.OrbitSpeed = Random.FRandRange(0.5, 2.0);
		
		const float Orbit = Random.FRandRange(0, OrbitRange);
		Data.OrbitDistance = OrbitOffset + Orbit;

		const int32 TemperatureRange = Random.RandRange(20, 100);
		const int32 TemperatureOffset = (1 - (Orbit / OrbitRange)) * 800 - 400;
		Data.TemperatureMin = TemperatureOffset - TemperatureRange / 2;
		Data.TemperatureMax = TemperatureOffset + TemperatureRange / 2;

		for (auto& Region : Data.Regions)
		{
			GeneratePlanetRegion(Region, Random.FRand() * MAX_int32, SettingsManager, Context);
		}
	}
}

void UVanillaGalaxyGenerator::GeneratePlanetRegion(FPlanetRegionMetadata& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto Seed = Settings->Seed * SubSeed;
		
		const static TSet<FName> RegionTypes = {
			FPlanetRegionMetadata::Type_Agrarian,
			FPlanetRegionMetadata::Type_City,
			FPlanetRegionMetadata::Type_Industrial
		};
	
		FRandomStream Random(Seed);

		for (const auto& Type : RegionTypes)
		{
			if (Random.FRand() > 0.5) Data.AllowedTypes.Add(Type);
		}
	}
}
