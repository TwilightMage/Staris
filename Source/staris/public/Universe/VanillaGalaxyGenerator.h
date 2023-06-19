// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GalaxyGenerator.h"
#include "UObject/Object.h"

#include "VanillaGalaxyGenerator.generated.h"

class UCompositeDatabase;
/**
 * 
 */
UCLASS()
class STARIS_API UVanillaGalaxyGenerator : public UGalaxyGenerator
{
	GENERATED_BODY()

public:
	virtual void GenerateGalaxy(FGalaxyMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const override;
	virtual void GenerateSystem(FSystemMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const override;
	virtual void GenerateStar(FStarMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const override;
	virtual void GeneratePlanet(FPlanetMetaData& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const override;
	virtual void GeneratePlanetRegion(FPlanetRegionMetadata& Data, int32 SubSeed, AGalaxySettingsManager* SettingsManager, UCompositeRecord* Context) const override;
};
