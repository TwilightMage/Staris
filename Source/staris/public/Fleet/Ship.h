// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Ship.generated.h"

class AFleet;

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARIS_API UShip : public USceneComponent
{
	GENERATED_BODY()

	friend AFleet;

public:
	UShip();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ControlRequired;

private:
	TWeakObjectPtr<AFleet> OwningFleet;
};
