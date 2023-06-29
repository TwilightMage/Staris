// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneLabel.h"
#include "TitleLabel.generated.h"

class ITitled;

UCLASS()
class STARIS_API UTitleLabel : public USceneLabel
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Setup(const TScriptInterface<ITitled>& Titled);
};
