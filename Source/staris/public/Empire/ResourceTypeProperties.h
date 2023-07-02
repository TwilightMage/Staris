// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Universe/CompositeDatabase.h"
#include "ResourceTypeProperties.generated.h"

class UEmpire;
/**
 * 
 */
UCLASS(Abstract)
class STARIS_API UResourceTypeProperties : public UCompositeRecordComponent
{
	GENERATED_BODY()

public:
	virtual void FailedToFulfill(UEmpire* Empire, int32 AmountFailed) {}
};
