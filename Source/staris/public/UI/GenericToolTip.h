// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolTip.h"
#include "GenericToolTip.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UGenericToolTip : public UToolTip
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void AddLine(const FText& Line);
};
