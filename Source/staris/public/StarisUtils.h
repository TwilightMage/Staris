// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StarisUtils.generated.h"

class UEmpire;
class UPlanet;
/**
 * 
 */
UCLASS()
class STARIS_API UStarisUtils : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void FillPlanetPopulation(UPlanet* Planet, UEmpire* Empire);
};
