// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/FocusablePanel.h"
#include "UObject/Interface.h"
#include "Focusable.generated.h"

class UContextMenu;
class UToolTip;
class UContextMenuItem;

UINTERFACE(BlueprintType)
class UFocusable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARIS_API IFocusable
{
	GENERATED_BODY()

public:
	virtual void SetupToolTip(UToolTip* ToolTip) = 0;

	virtual TArray<UContextMenuItem*> CreateContextActionsHovered(IFocusable* Selected) { return {}; }
	virtual TArray<UContextMenuItem*> CreateContextActionsSelected(IFocusable* Hovered) { return {}; }

	virtual void OnSelected() {}
	virtual void OnDeselected() {}

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	TSubclassOf<UFocusablePanel> GetPanelWidgetClass();
};
