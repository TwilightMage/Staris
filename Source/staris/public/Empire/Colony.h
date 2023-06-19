// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceProvider.h"
#include "UObject/Object.h"
#include "Colony.generated.h"

class URace;
/**
 * 
 */
UCLASS(EditInlineNew)
class STARIS_API UColony : public UObject, public IResourceProvider
{
	GENERATED_BODY()

public:
	virtual TMap<UCompositeRecord*, int32> GetResourceBalance() const override;
	
	UFUNCTION(BlueprintPure)
	int32 getTotalPopulationAmount() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<URace*, int32> Population;
};
