// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StarisController.generated.h"

class UEmpire;

UINTERFACE()
class UStarisController : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARIS_API IStarisController
{
	GENERATED_BODY()

	friend UEmpire;
	
public:
	virtual UEmpire* GetEmpire() const = 0;
	virtual bool IsPlayer() const = 0;
	virtual FString GetControllerName() const = 0;

protected:
	virtual void OnEmpireAssigned(UEmpire* Empire) {}
};
