// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/VanillaGalaxyGenerator.h"

#include "Universe/CompositeDatabase.h"
#include "Universe/GalaxySettingsManager.h"
#include "Universe/VanillaGalaxySettings.h"

FName GenerateName(int32 Seed)
{
	const static FString CharSet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	FRandomStream Random(Seed);

	FString Result;

	for (int32 i = 0; i < 6; i++)
	{
		Result += CharSet[Random.RandRange(0, CharSet.Len() - 1)];
	}

	return FName(Result);
}

template<typename T>
T GetRandomSetItem(const TSet<T>& Container, FRandomStream& Random) { return Container[FSetElementId::FromInteger(Random.RandRange(0, Container.Num() - 1))]; }

template<typename T>
T GetRandomArrayItem(const TArray<T>& Container, FRandomStream& Random) { return Container[Random.RandRange(0, Container.Num() - 1)]; }

void UVanillaGalaxyGenerator::GenerateGalaxy(FGalaxyMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto Seed = Settings->Seed * SubSeed;
		
		FRandomStream Random(Seed);

		Data.Id = GenerateName(Seed);
		Data.Systems.SetNum(Settings->SystemCount);

		for (auto& System : Data.Systems)
		{
			GenerateSystem(System, Random.FRand() * MAX_int32, SettingsManager);
		}
	}
}

void UVanillaGalaxyGenerator::GenerateSystem(FSystemMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto Seed = Settings->Seed * SubSeed;
		
		FRandomStream Random(Seed);

		bool BlackHole = Random.FRand() < 0.1;

		Data.Id = GenerateName(Seed);
		Data.Stars.SetNum(1);
		Data.Planets.SetNum(BlackHole ? 0 : Random.RandRange(3, 7));

		float Angle = Random.FRand();
		float Dist = Random.FRand();

		Dist += FMath::Cos(Dist * 3.14) * 0.1;

		Data.Location = FVector(FMath::Sin(3.14 * 2 * Angle) * 50000 * Dist, FMath::Cos(3.14 * 2 * Angle) * 50000 * Dist, Random.FRand() * 1000 - 500) * 5;

		if (BlackHole)
		{
			Data.Stars[0].Type = StarTypeDatabase->GetOrCreateRecord(FStarMetaData::TYPE_Black_Hole);
		}
		for (auto& Star : Data.Stars)
		{
			GenerateStar(Star, Random.FRand() * MAX_int32, SettingsManager);
		}
		
		for (auto& Planet : Data.Planets)
		{
			GeneratePlanet(Planet, Random.FRand() * MAX_int32, SettingsManager);
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

void UVanillaGalaxyGenerator::GenerateStar(FStarMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager) const
{
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto Seed = Settings->Seed * SubSeed;
		
		const static TSet<FName> StarTypes = {
			FStarMetaData::TYPE_Blue,
			FStarMetaData::TYPE_Red,
			FStarMetaData::TYPE_Yellow
		};

		FRandomStream Random(Seed);

		Data.Id = GenerateName(Seed);
		
		if (!Data.Type) Data.Type = StarTypeDatabase->GetOrCreateRecord(GetRandomSetItem(StarTypes, Random));

		Data.Location = FVector::Zero();
		Data.Scale = Random.FRandRange(0.5, 1.0);
	}
}

void UVanillaGalaxyGenerator::GeneratePlanet(FPlanetMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager) const
{
	const float OrbitRange = 2000;
	const float OrbitOffset = 300;
	
	if (auto Settings = SettingsManager->GetSettings<UVanillaGalaxySettings>())
	{
		auto Seed = Settings->Seed * SubSeed;
		
		const static TSet<FName> PlanetBiomes = {
			FPlanetMetaData::BIOME_Desert,
			FPlanetMetaData::BIOME_Forest,
			FPlanetMetaData::BIOME_Arctic
		};

		FRandomStream Random(Seed);

		Data.Id = GenerateName(Seed);
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
			GeneratePlanetRegion(Region, Random.FRand() * MAX_int32, SettingsManager);
		}
	}
}

void UVanillaGalaxyGenerator::GeneratePlanetRegion(FPlanetRegionMetadata& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager) const
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
