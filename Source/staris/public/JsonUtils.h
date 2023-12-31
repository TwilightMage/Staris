﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "JsonUtils.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class STARIS_API UJsonUtils : public UObject
{
	GENERATED_BODY()

public:
	static TSharedPtr<FJsonObject> StringToJson(const FString& string);
};
