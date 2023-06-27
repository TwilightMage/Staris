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
class UEmpire;

UCLASS(BlueprintType)
class STARIS_API USystem : public UObject, public ICelestialEntity
{
	GENERATED_BODY()

	friend UEmpire;

public:
	USystem();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyPattern(const FSystemMetaData& Data);

	UFUNCTION(BlueprintPure)
	UEmpire* GetOwningEmpire();

	UFUNCTION(BlueprintPure)
	FText GetTitle() const;

	void RenameSystem(const FString& NewName, bool PropagateToChildren);

	const FName& GetId() const { return Id; }
	int32 GetSeed() const { return Seed; }
	const FVector& GetLocation() const { return Location; }
	const TArray<UStar*>& GetStars() const { return Stars; }
	const TArray<UPlanet*>& GetPlanets() const { return Planets; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;
	
protected:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Id;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int32 Seed;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector Location;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UStar*> Stars;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UPlanet*> Planets;

	TWeakObjectPtr<UEmpire> OwningEmpire;
};
