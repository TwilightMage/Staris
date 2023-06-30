// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneLabel.h"
#include "Blueprint/UserWidget.h"

#include "LabelExtension.generated.h"

class USceneLabel;
class ILabeled;

UCLASS(Abstract)
class STARIS_API ULabelExtension : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual bool TestTarget(ILabeled* Labeled) { return false; }

	UFUNCTION(BlueprintImplementableEvent)
	void Setup(const TScriptInterface<ILabeled>& Labeled);

	UFUNCTION(BlueprintPure)
	USceneLabel* GetLabel() const { return Cast<USceneLabel>(GetOuter()); }
};
