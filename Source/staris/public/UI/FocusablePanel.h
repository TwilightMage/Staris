// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FocusablePanel.generated.h"

class IFocusable;

DECLARE_MULTICAST_DELEGATE(FFocusablePanelCloseEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFocusablePanelCloseDynamicEvent);

UCLASS()
class STARIS_API UFocusablePanel : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void Setup(const TScriptInterface<IFocusable>& Target);

	FFocusablePanelCloseEvent OnClose;

	UPROPERTY(BlueprintAssignable, DisplayName="On Close")
	FFocusablePanelCloseDynamicEvent OnClose_K2;

protected:
	UFUNCTION(BlueprintCallable)
	void Close();
};
