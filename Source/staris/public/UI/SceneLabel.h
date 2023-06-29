// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SceneLabel.generated.h"

DECLARE_MULTICAST_DELEGATE(FLabelClicked);

UCLASS()
class STARIS_API USceneLabel : public UUserWidget
{
	GENERATED_BODY()

public:
	FLabelClicked OnLabelClicked;

protected:
	UFUNCTION(BlueprintCallable)
	void Clicked() { OnLabelClicked.Broadcast(); }
};
