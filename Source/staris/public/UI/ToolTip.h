﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ToolTip.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UToolTip : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddLine(const FText& Line);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddSeparator();
};
