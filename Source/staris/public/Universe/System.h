// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CelestialEntity.h"
#include "GalaxyMetaData.h"
#include "GameFramework/Actor.h"

#include "System.generated.h"

class AGalaxy;
class AStaticMeshActor;
class UPlanet;
class UStar;

UCLASS()
class STARIS_API ASystem : public AActor, public ICelestialEntity
{
	GENERATED_BODY()

	friend UEmpire;

public:
	ASystem();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyPattern(const FSystemMetaData& Data);

	UFUNCTION(BlueprintPure)
	UEmpire* GetOwningEmpire();

	const FName& GetId() const { return Id; }
	const TArray<UStar*>& GetStars() const { return Stars; }
	const TArray<UPlanet*>& GetPlanets() const { return Planets; }
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	USceneComponent* Root;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Id;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UStar*> Stars;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UPlanet*> Planets;

	TWeakObjectPtr<UEmpire> OwningEmpire;
};
