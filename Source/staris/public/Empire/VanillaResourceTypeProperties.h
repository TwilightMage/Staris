// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "ResourceTypeProperties.h"
#include "VanillaResourceTypeProperties.generated.h"

UCLASS(EditInlineNew)
class STARIS_API UVanillaResourceTypeProperties : public UResourceTypeProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsMineral;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition=bIsMineral, EditConditionHides=true))
	FRuntimeFloatCurve MineralDensityPerLayerCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition=bIsMineral, EditConditionHides=true))
	FRuntimeFloatCurve MineralDensityPerTemperatureCurve;
};
