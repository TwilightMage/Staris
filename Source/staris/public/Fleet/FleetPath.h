// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FleetPath.generated.h"

class AFleet;

USTRUCT(BlueprintType)
struct FFleetPathConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanEnterDeepSpace;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Start;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector End;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USystem* StartHint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USystem* EndHint;
};

UCLASS(Abstract)
class UFleetPathNode : public UObject
{
	GENERATED_BODY()

public:
	virtual bool MoveTick(float& TickAmount, AFleet* Fleet, float& Distance) { return true; }

	UPROPERTY()
	UFleetPathNode* NextNode;
};

UCLASS()
class UFleetPathLocationNode : public UFleetPathNode
{
	GENERATED_BODY()

public:
	virtual bool MoveTick(float& TickAmount, AFleet* Fleet, float& Distance) override;
	
	FVector Location;
};

UCLASS()
class STARIS_API UFleetPath : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static UFleetPath* BuildPath(const FFleetPathConfig& Config, const UObject* WorldContextObject, UObject* Outer = nullptr);

	UFUNCTION(BlueprintPure)
	UFleetPathNode* GetFirstNode() const;

private:
	void AddNode(UFleetPathNode* Node);
	
	UPROPERTY()
	TArray<UFleetPathNode*> Nodes;
};
