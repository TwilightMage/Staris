// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Titled.generated.h"

UINTERFACE(NotBlueprintable)
class UTitled : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARIS_API ITitled
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual FText GetTitle() const = 0;

	UFUNCTION(BlueprintCallable)
	virtual UTexture2D* GetIcon() const { return nullptr; }
};
