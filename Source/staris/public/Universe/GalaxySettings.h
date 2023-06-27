// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GalaxySettings.generated.h"

struct FJsonObjectWrapper;
class USettingsPanel;
/**
 * 
 */
UCLASS(Abstract, BlueprintType)
class STARIS_API UGalaxySettings : public UObject
{
	GENERATED_BODY()

public:
	virtual void FillSettingsPanel(USettingsPanel* SettingsPanel, const TSharedPtr<FJsonObject>& JsonContainer) {}
};
