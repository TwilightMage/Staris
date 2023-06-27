// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Building.generated.h"

class UCompositeRecord;
struct FColonyStats;

UCLASS()
class STARIS_API UBuilding : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY()
	UCompositeRecord* BuildingType;
};
