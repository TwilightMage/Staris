// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/ToolTip.h"
#include "StarToolTip.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UStarToolTip : public UToolTip
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void SetupToolTip(
		const FText& StarTitle,
		const FText& SystemTitle,
		const FText& ClassTitle);
};
