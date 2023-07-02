// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GalaxyBuildingTypeProperties.h"
#include "VanillaGalaxyBuildingTypeProperties.generated.h"

UCLASS(EditInlineNew)
class STARIS_API UVanillaGalaxyBuildingTypeProperties : public UGalaxyBuildingTypeProperties
{
	GENERATED_BODY()

public:
	TMap<UCompositeRecord*, int32> GetBuildRequirements() const
	{
		TMap<UCompositeRecord*, int32> Result;
		
		if (const auto Resources = ResourceTypeDatabase.Get())
		{
			for (auto& Pair : BuildRequirements)
			{
				Result.Add(Resources->GetOrCreateRecord(Pair.Key), Pair.Value);
			}
		}

		return Result;
	}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, int32> BuildRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BaseBuildTime;

	TWeakObjectPtr<UCompositeDatabase> ResourceTypeDatabase;
};
