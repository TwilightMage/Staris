// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EmpirePlanetKnowledge.generated.h"


class UPlanet;
class UCompositeRecord;

DECLARE_MULTICAST_DELEGATE(FEmpirePlanetKnowledgeUpdated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEmpirePlanetKnowledgeDynamicUpdated);

UCLASS(BlueprintType)
class STARIS_API UEmpirePlanetKnowledge : public UObject
{
	GENERATED_BODY()

public:
	bool Setup(UPlanet* Planet);
	
	UFUNCTION(BlueprintCallable)
	void UnlockTile(int32 Layer, int32 TileIndex);

	UFUNCTION(BlueprintCallable)
	void UnlockRandomTile(int32 Layers, const TArray<UCompositeRecord*>& WhitelistResources);

	UFUNCTION(BlueprintCallable)
	void UnlockLayer(int32 Layer);

	FEmpirePlanetKnowledgeUpdated OnUpdated;

	UPROPERTY(BlueprintAssignable, DisplayName="On Updated")
	FEmpirePlanetKnowledgeDynamicUpdated OnUpdated_K2;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UPlanet* TargetPlanet;
	
	TArray<TSet<int32>> UnlockedTileIndices;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TMap<UCompositeRecord*, int32> ResourcesCached;
};
