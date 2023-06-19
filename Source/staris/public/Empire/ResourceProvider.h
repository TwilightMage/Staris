// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ResourceProvider.generated.h"

class UCompositeRecord;
class UEmpire;
// This class does not need to be modified.
UINTERFACE()
class UResourceProvider : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARIS_API IResourceProvider
{
	GENERATED_BODY()

	virtual TMap<UCompositeRecord*, int32> GetResourceBalance() const { return {}; }
public:
};
