// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GlobalPlaneConsumer.generated.h"

UINTERFACE(NotBlueprintable)
class UGlobalPlaneConsumer : public UInterface
{
	GENERATED_BODY()
};

class STARIS_API IGlobalPlaneConsumer
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void GlobalPlaneClicked(const FVector& Location) {}

	UFUNCTION(BlueprintCallable)
	virtual void GlobalPlaneTick(const FVector& MouseLocation) {}
};
