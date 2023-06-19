// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CelestialEntity.h"
#include "Focusable.h"
#include "GalaxyMetaData.h"

#include "Planet.generated.h"

class UColony;
class UGenericToolTip;
class AGalaxy;
class ASystem;
class USceneComponent;

UCLASS()
class STARIS_API UPlanet : public UStaticMeshComponent, public ICelestialEntity, public IFocusable
{
	GENERATED_BODY()

	friend ASystem;

public:
	UPlanet();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyPattern(const FPlanetMetaData& Data);

	virtual void OnDayPassed(int32 Day) override;

	virtual UToolTip* CreateToolTip() override;
	virtual void SetupToolTip(UToolTip* ToolTip) override;

	FText GetTitle() const { return Title.IsEmpty() ? FText::FromName(Id) : FText::FromString(Title); }
	const FName& GetId() const { return Id; }
	ASystem* GetSystem() const { return System.Get(); }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UColony* Colony;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	FVector OrbitPoint;
	float OrbitOffset;
	float OrbitDistance;
	float OrbitSpeed;

private:
	virtual void OnInitCelestialEntity() override;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Id;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Biome;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int32 TemperatureDay;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int32 TemperatureNight;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGenericToolTip> ToolTipClass;

	TWeakObjectPtr<ASystem> System;
};
