// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fleet.generated.h"

class UShip;

UCLASS()
class STARIS_API AFleet : public AActor
{
	GENERATED_BODY()

	friend UShip;

public:
	AFleet();

	bool AssignShip(UShip* Ship, bool Force);

	int32 GetCurrentUsedControl();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ControlLimit;
	
private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UShip*> Ships;
};
