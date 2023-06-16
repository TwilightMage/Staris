// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "CelestialEntity.generated.h"

class AGalaxy;

// This class does not need to be modified.
UINTERFACE()
class UCelestialEntity : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARIS_API ICelestialEntity
{
	GENERATED_BODY()

public:
	void RegisterOnDayPassed();
	void UnRegisterFromDayPassed();
	
	void RegisterOnMonthPassed();
	void UnRegisterFromMonthPassed();
	
	void RegisterOnYearPassed();
	void UnRegisterFromYearPassed();
	
	virtual void OnDayPassed(int32 Day) {}
	virtual void OnMonthPassed(int32 Month) {}
	virtual void OnYearPassed(int32 Year) {}

	AGalaxy* GetGalaxy();

	void InitCelestialEntity(AGalaxy* Galaxy);
	
protected:
	virtual void OnInitCelestialEntity() {}
	
private:
	TWeakObjectPtr<AGalaxy> WeakGalaxy;
	bool SignedToDayUpdate = false;
	bool SignedToMonthUpdate = false;
	bool SignedToYearUpdate = false;
};
