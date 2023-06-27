// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StarisGameSettings.generated.h"

UCLASS()
class STARIS_API AStarisGameSettings : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScrollSensitivity = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinZoomDistance = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxZoomDistance = 100000;
};
