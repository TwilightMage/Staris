// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StarTypeProperties.h"

#include "VanillaStarTypeProperties.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UVanillaStarTypeProperties : public UStarTypeProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMaterialInstance* MaterialInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;
};
