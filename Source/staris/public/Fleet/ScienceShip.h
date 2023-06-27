// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Ship.h"
#include "ScienceShip.generated.h"


UCLASS(Blueprintable)
class STARIS_API UScienceShip : public UShip
{
	GENERATED_BODY()

public:
	UScienceShip();
	
	virtual FText GetClassTitle() const override;

	virtual bool CanExecuteOrder(UFleetOrder* Order) const override;

	virtual bool FullFleetShip() const override;
};
