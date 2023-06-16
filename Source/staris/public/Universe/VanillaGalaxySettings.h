// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GalaxySettings.h"
#include "VanillaGalaxySettings.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew)
class STARIS_API UVanillaGalaxySettings : public UGalaxySettings
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Seed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SystemCount;
};
