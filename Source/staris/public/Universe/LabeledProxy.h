// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LabeledProxy.generated.h"

class ILabeled;
// This class does not need to be modified.
UINTERFACE()
class ULabeledProxy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARIS_API ILabeledProxy
{
	GENERATED_BODY()
public:

	virtual ILabeled* GetLabeled() const = 0;
};
