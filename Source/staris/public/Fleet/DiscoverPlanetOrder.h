// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "FleetOrder.h"

#include "DiscoverPlanetOrder.generated.h"

class UPlanet;

/**
 * 
 */
UCLASS()
class STARIS_API UDiscoverPlanetOrder : public UFleetOrder
{
	GENERATED_BODY()

public:
	virtual FText GetTitle() const override;

	virtual float ExecutionDistance() const override;

	virtual UFleetPath* BuildPathToTarget(const FVector& Start, USystem* StartHint) override;

	virtual bool ExecutionTick(float& TickAmount, AFleet* Fleet) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlanet* TargetPlanet;

private:
	float DiscoverAmount;
};
