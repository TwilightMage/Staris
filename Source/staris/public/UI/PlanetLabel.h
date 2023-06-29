// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SceneLabel.h"
#include "PlanetLabel.generated.h"

class UPlanet;
/**
 * 
 */
UCLASS()
class STARIS_API UPlanetLabel : public USceneLabel
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Setup(UPlanet* Planet);
};
