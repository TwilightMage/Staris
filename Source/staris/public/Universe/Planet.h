// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CelestialEntity.h"
#include "Focusable.h"
#include "GalaxyMetaData.h"

#include "Planet.generated.h"

class UEmpire;
class UColony;
class UToolTip;
class AGalaxy;
class USystem;
class USceneComponent;
class UMeshInstanceRef;

UCLASS(BlueprintType)
class STARIS_API UPlanet : public UObject, public ICelestialEntity, public IFocusable
{
	GENERATED_BODY()

	friend USystem;

public:
	UPlanet();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyPattern(const FPlanetMetaData& Data);

	UColony* SettleColony();
	UColony* GetColony() const { return Colony; }

	virtual void OnMonthPassed(int32 Month) override;

	virtual void SetupToolTip(UToolTip* ToolTip) override;

	virtual TArray<UContextMenuItem*> CreateContextActionsHovered(IFocusable* Selected) override;

	virtual void OnSelected() override;
	
	UFUNCTION(BlueprintPure)
	FText GetTitle() const;

	const FName& GetId() const { return Id; }
	int32 GetSeed() const { return Seed; }
	const FVector& GetLocation() const { return Location; }
	int32 GetTemperature() const { return Temperature; }

	UFUNCTION(BlueprintPure)
	int32 GetArea() const { return 4 * 3.14 * FMath::Pow((float)Radius, 2); }
	int32 GetRadius() const { return Radius; }
	float GetRealRadius() const { return Radius / 12000.0f * 15; }

	const TArray<FPlanetLayer>& GetLayers() const { return Layers; }
	void AddTileToLayer(int32 Layer, UCompositeRecord* ResourceType);

	UFUNCTION(BlueprintPure)
	USystem* GetSystem() const;

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
	int32 Seed;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UMeshInstanceRef* MeshInstance;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector Location;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int32 Temperature;

	// Radius in kilometers
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int32 Radius;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<FPlanetLayer> Layers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	UColony* Colony;
};
