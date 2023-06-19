// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "BuildingShip.generated.h"


UCLASS(Blueprintable)
class STARIS_API UBuildingShip : public UShip
{
	GENERATED_BODY()

public:
	UBuildingShip();
};
