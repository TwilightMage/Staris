// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GlobalPlaneConsumer.h"
#include "UObject/Object.h"

#include "BuildMode.generated.h"

class AStaticMeshActor;
class UCompositeRecord;
/**
 * 
 */
UCLASS()
class STARIS_API UBuildMode : public UObject, public IGlobalPlaneConsumer
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void GlobalPlaneClicked(const FVector& Location) override;

	UFUNCTION(BlueprintCallable)
	virtual void GlobalPlaneTick(const FVector& Location) override;
	
	UFUNCTION(BlueprintCallable)
	void SetActiveBuildingType(UCompositeRecord* NewGalaxyBuildingType);

private:
	UPROPERTY()
	UCompositeRecord* GalaxyBuildingType;

	UPROPERTY()
	AStaticMeshActor* BuildingMarker;
};
