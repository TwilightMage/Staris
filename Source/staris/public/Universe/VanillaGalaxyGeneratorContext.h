// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompositeDatabase.h"
#include "RandomDistribution.h"
#include "VanillaGalaxyGeneratorContext.generated.h"

struct FPlanetMetaData;
struct FStarMetaData;
struct FSystemMetaData;
struct FGalaxyMetaData;
class UVanillaResourceTypeProperties;

UCLASS()
class STARIS_API UVanillaGalaxyGeneratorContext : public UCompositeRecordComponent
{
	GENERATED_BODY()

public:
	struct S1
	{
		UCompositeRecord* Record;
		UVanillaResourceTypeProperties* VanillaProps;
	};

	struct S2
	{
		UCompositeRecord* Record;
		FRichCurve* LayerDensityCurve = nullptr;
		FRichCurve* TemperatureDensityCurve = nullptr;
		float Density;
	};
	
	// Game
	TArray<FString> AvailableStarNames;
	TSet<FName> UsedIDs;

	// Galaxy
	FGalaxyMetaData* CurrentGalaxy = nullptr;
	TArray<S1> MineralResourceTypes;

	// System
	FSystemMetaData* CurrentSystem = nullptr;
	FRandomDistribution<int32> StarCountDistribution;

	// Star
	FStarMetaData* CurrentStar = nullptr;
	int32 CurrentStarIndex = 0;
	bool IsBlackHole = false;
	FRandomDistribution<UCompositeRecord*> StarTypeDistribution;

	// Planet
	FPlanetMetaData* CurrentPlanet = nullptr;
	int32 CurrentPlanetIndex = 0;
	TArray<S2> PlanetMineralDistribution;

	// Layer
	int32 CurrentPlanetLayerIndex = 0;
};
