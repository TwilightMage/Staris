// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceProvider.h"
#include "Universe/Planet.h"
#include "UObject/Object.h"
#include "Colony.generated.h"

class UPlanet;
class URace;
class UBuilding;

USTRUCT(BlueprintType)
struct FColonyStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FName> Flags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UCompositeRecord*, float> ResourceBalance;
};

UCLASS(EditInlineNew)
class STARIS_API UColony : public UObject, public IResourceProvider
{
	GENERATED_BODY()

public:
	UColony();
	
	virtual TMap<UCompositeRecord*, int32> GetResourceBalance() const override;
	
	UFUNCTION(BlueprintPure)
	int32 getTotalPopulationAmount() const;

	void MonthPassed();

	UBuilding* AddBuilding(UCompositeRecord* BuildingType);

	UPlanet* GetPlanet() const { return Cast<UPlanet>(GetOuter()); }

	const FColonyStats& GetStats() const { return Stats; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<URace*, int32> Population;
	
private:
	void RecalculateStats();

	UPROPERTY()
	TArray<UBuilding*> Buildings;
	
	TMap<UCompositeRecord*, int32> Balance;

	FColonyStats Stats;
};
