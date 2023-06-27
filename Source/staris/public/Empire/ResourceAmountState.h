// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "ResourceAmountState.generated.h"

USTRUCT(BlueprintType)
struct FResourceAmountState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Amount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Balance;
};