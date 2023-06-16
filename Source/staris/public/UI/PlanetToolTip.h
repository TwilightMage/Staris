// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolTip.h"
#include "PlanetToolTip.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UPlanetToolTip : public UToolTip
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetupToolTip(
		const FText& PlanetTitle,
		const FText& SystemTitle,
		int32 Population,
		const FText& BiomeTitle);
};
