// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Labeled.generated.h"

class USceneLabel;

UINTERFACE()
class ULabeled : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARIS_API ILabeled
{
	GENERATED_BODY()

public:
	virtual USceneLabel* CreateLabel() = 0;
	virtual void SetupLabel(USceneLabel* Label) = 0;
	virtual FVector GetLabelLocation() const = 0;

	int32 DisplayCounter = 0;
};
