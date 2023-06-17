// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GalaxyMetaData.h"
#include "UObject/Interface.h"

#include "GalaxyGenerator.generated.h"

class AGalaxySettingsManager;
class AStarisGameMode;
class UGalaxySettings;
class UCompositeDatabase;

UCLASS()
class STARIS_API UGalaxyGenerator : public UObject
{
	GENERATED_BODY()

	friend AStarisGameMode;

public:
	virtual void GenerateGalaxy(FGalaxyMetaData& Data, int32 SubSeed, AGalaxySettingsManager* Settings) const {};
	virtual void GenerateSystem(FSystemMetaData& Data, int32 SubSeed, AGalaxySettingsManager* Settings) const {};
	virtual void GenerateStar(FStarMetaData& Data, int32 SubSeed, AGalaxySettingsManager* Settings) const {};
	virtual void GeneratePlanet(FPlanetMetaData& Data, int32 SubSeed, AGalaxySettingsManager* Settings) const {};
	virtual void GeneratePlanetRegion(FPlanetRegionMetadata& Data, int32 SubSeed, AGalaxySettingsManager* Settings) const {};

	UPROPERTY()
	UCompositeDatabase* StarTypeDatabase;
};
