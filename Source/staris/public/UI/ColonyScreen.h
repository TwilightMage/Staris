// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ColonyScreen.generated.h"

class UColony;
/**
 * 
 */
UCLASS()
class STARIS_API UColonyScreen : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Setup(UColony* Colony);
};
