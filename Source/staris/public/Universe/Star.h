// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CelestialEntity.h"
#include "Components/SceneComponent.h"
#include "GalaxyMetaData.h"

#include "Star.generated.h"

UCLASS()
class STARIS_API UStar : public UStaticMeshComponent, public ICelestialEntity
{
	GENERATED_BODY()

public:
	UStar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyPattern(const FStarMetaData& Data);

	const FName& GetId() const { return Id; }
	const FName& GetType() const { return Type; }

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Id;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Type;
};
