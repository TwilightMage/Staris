// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "EmpireGenerator.generated.h"

class URace;
class AGalaxy;
class UEmpire;
class USettingsPanel;
struct FJsonObjectWrapper;

/**
 * 
 */
UCLASS(Abstract)
class STARIS_API UEmpireGenerator : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	UEmpire* Generate(AGalaxy* Galaxy);

	virtual UEmpire* Generate_Implementation(AGalaxy* Galaxy) { return nullptr; }

	virtual void FillSettingsPanel(USettingsPanel* SettingsPanel, const TSharedPtr<FJsonObject>& Json) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<URace*> EmpireRaces;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString EmpireTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SystemTitle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString CapitalTitle;
};
