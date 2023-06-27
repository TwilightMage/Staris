// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ResourcePanel.generated.h"

class UCompositeRecord;

UCLASS()
class STARIS_API UResourcePanel : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	
	UFUNCTION(BlueprintImplementableEvent)
	void Update(const TMap<UCompositeRecord*, FResourceAmountState>& State);
};
