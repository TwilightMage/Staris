// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "StarisHUD.generated.h"

class IFocusable;
class UFocusablePanel;
class UPlanet;
class UResourcePanel;

UCLASS()
class STARIS_API AStarisHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OpenFocusablePanel(const TScriptInterface<IFocusable>& Target);
};
