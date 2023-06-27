// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FleetOrder.generated.h"

class AFleet;
class UFleetPath;

DECLARE_MULTICAST_DELEGATE(FFleetOrderFinishedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FFleetOrderFinishedDynamicEvent);

DECLARE_MULTICAST_DELEGATE_OneParam(FFleetOrderProgressEvent, float /* New Progress */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFleetOrderProgressDynamicEvent, float, NewProgress);

UCLASS(BlueprintType)
class STARIS_API UFleetOrder : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	virtual FText GetTitle() const { return FText(); }

	UFUNCTION(BlueprintPure)
	virtual float ExecutionDistance() const { return 0.f; }

	UFUNCTION(BlueprintCallable)
	virtual UFleetPath* BuildPathToTarget(const FVector& Start, USystem* StartHint = nullptr) { return nullptr; }

	UFUNCTION(BlueprintPure)
	const UFleetPath* GetPath() const { return Path; }

	virtual bool ExecutionTick(float& TickAmount, AFleet* Fleet) { return false; }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bValid;
	
	FFleetOrderFinishedEvent OnOrderFinished;

	UPROPERTY(BlueprintAssignable, DisplayName="On Order Finished")
	FFleetOrderFinishedDynamicEvent OnOrderFinished_K2;

	FFleetOrderProgressEvent OnOrderProgress;

	UPROPERTY(BlueprintAssignable, DisplayName="On Order Progress")
	FFleetOrderProgressDynamicEvent OnOrderProgress_K2;

protected:
	UPROPERTY()
	UFleetPath* Path;
};
