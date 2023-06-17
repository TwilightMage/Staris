// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Colony.generated.h"

class URace;
/**
 * 
 */
UCLASS(EditInlineNew)
class STARIS_API UColony : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	int32 getTotalPopulationAmount() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<URace*, int32> Population;
};
