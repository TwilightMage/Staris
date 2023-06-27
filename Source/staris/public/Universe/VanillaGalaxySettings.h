// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GalaxySettings.h"
#include "GalaxyMetaData.h"

#include "VanillaGalaxySettings.generated.h"

USTRUCT(BlueprintType)
struct FMinMaxInt32
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Max;
};

USTRUCT(BlueprintType)
struct FMinMaxFloat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Min;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Max;
};

UCLASS(BlueprintType, EditInlineNew)
class STARIS_API UVanillaGalaxySettings : public UGalaxySettings
{
	GENERATED_BODY()

public:
	virtual void FillSettingsPanel(USettingsPanel* SettingsPanel, const TSharedPtr<FJsonObject>& Json) override;
	
	UFUNCTION(BlueprintCallable)
	static UTexture2D* LoadMaskFromComputer();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Galaxy)
	int32 Seed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Galaxy)
	int32 SystemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Galaxy)
	float GalaxyRadius = 50000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Galaxy)
	float GalaxyHeight = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Galaxy)
	UTexture2D* SystemDistributionMap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=System)
	TMap<int32, float> StarCountDistribution = {
		{1, 8},
		{2, 1.5},
		{3, 0.5}
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=System)
	FMinMaxInt32 PlanetAmountRange = {3, 9};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Planet)
	FMinMaxFloat PlanetOrbitRange = {300, 1500};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Planet)
	int32 TemperatureRandomSpread = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Planet)
	FMinMaxInt32 TemperatureGlobalRange = {-300, 500};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Planet)
	int32 LayerNum = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Planet)
	int32 LayerSize = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Star)
	TMap<FName, float> StarTypeDistribution = {
		{ FStarMetaData::TYPE_Red, 1 },
		{ FStarMetaData::TYPE_Blue, 1 },
		{ FStarMetaData::TYPE_Yellow, 1 },
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Star)
	float BlackHoleChance = 0.1;

	inline static UTexture2D* SavedMask;
private:
};
