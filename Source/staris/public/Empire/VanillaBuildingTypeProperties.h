// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "BuildingTypeProperties.h"
#include "Universe/CompositeDatabase.h"
#include "VanillaBuildingTypeProperties.generated.h"

/**
 * 
 */
UCLASS()
class STARIS_API UVanillaBuildingTypeProperties : public UBuildingTypeProperties
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SingleOfType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FName> AddFlags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSet<FName> RemoveFlags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UCompositeRecord*, float> PreMultiplyAddIncomeResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UCompositeRecord*, float> MultiplyIncomeResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UCompositeRecord*, float> PostMultiplyAddIncomeResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UCompositeRecord*, float> PreMultiplyAddOutcomeResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UCompositeRecord*, float> MultiplyOutcomeResources;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<UCompositeRecord*, float> PostMultiplyAddOutcomeResources;
};
