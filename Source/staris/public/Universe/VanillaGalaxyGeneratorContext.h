// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CompositeDatabase.h"
#include "VanillaGalaxyGeneratorContext.generated.h"

struct FPlanetMetaData;
struct FStarMetaData;
struct FSystemMetaData;
struct FGalaxyMetaData;

UCLASS()
class STARIS_API UVanillaGalaxyGeneratorContext : public UCompositeRecordComponent
{
	GENERATED_BODY()

public:
	TArray<FString> AvailableStarNames;
	TSet<FName> UsedIDs;

	FGalaxyMetaData* CurrentGalaxy = nullptr;
	FSystemMetaData* CurrentSystem = nullptr;
	FStarMetaData* CurrentStar = nullptr;
	int32 CurrentStarIndex = 0;
	bool IsBlackHole = false;
	FPlanetMetaData* CurrentPlanet = nullptr;
	int32 CurrentPlanetIndex = 0;
};
