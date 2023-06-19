// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypeProperties.h"
#include "VanillaResourceTypeProperties.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UVanillaResourceTypeProperties : public UResourceTypeProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;
};
