// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "ScienceShip.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARIS_API UScienceShip : public UShip
{
	GENERATED_BODY()

public:
	UScienceShip();
};
