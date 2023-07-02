// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EmpireGenerator.h"
#include "UnitedEmpireGenerator.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UUnitedEmpireGenerator : public UEmpireGenerator
{
	GENERATED_BODY()

public:
	virtual UEmpire* Generate_Implementation(AGalaxy* Galaxy) override;

	virtual void FillSettingsPanel(USettingsPanel* SettingsPanel, const TSharedPtr<FJsonObject>& Json) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalPops;
};
