// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "LabelExtension.h"
#include "ResourceLabelExtension.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UResourceLabelExtension : public ULabelExtension
{
	GENERATED_BODY()

public:
	virtual bool TestTarget(ILabeled* Labeled) override;
};
