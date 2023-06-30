// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "SceneLabel.generated.h"

class ILabeled;
class ULabelExtension;

DECLARE_MULTICAST_DELEGATE(FLabelClicked);

UCLASS()
class STARIS_API USceneLabel : public UUserWidget
{
	GENERATED_BODY()

public:
	FLabelClicked OnLabelClicked;

	UFUNCTION(BlueprintCallable)
	void SetLabelExtension(const TSubclassOf<ULabelExtension>& ExtensionClass);

	UFUNCTION(BlueprintPure)
	TScriptInterface<ILabeled> GetLabeled() const;
	
	ILabeled* Labeled;

protected:
	UFUNCTION(BlueprintCallable)
	void Clicked() { OnLabelClicked.Broadcast(); }

	UFUNCTION(BlueprintImplementableEvent)
	void PutExtension(ULabelExtension* Extension);
};
