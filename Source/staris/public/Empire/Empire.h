// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Empire.generated.h"

class URace;
class UPlanet;
class ASystem;
class AStarisPlayerController;
class IStarisController;

DECLARE_MULTICAST_DELEGATE_TwoParams(FEmpireSystemChanged, ASystem* /* System */, bool /* Taken or Lost */);

/**
 * 
 */
UCLASS()
class STARIS_API UEmpire : public UObject
{
	GENERATED_BODY()

	friend IStarisController;
	friend ASystem;
	
public:
	const TArray<IStarisController*>& GetOwningControllers() const { return OwningControllers; }
	void AssignController(IStarisController* Controller);
	void RemoveController(IStarisController* Controller);
	bool TryAssignPlayer(AStarisPlayerController* Player);

	UFUNCTION(BlueprintCallable)
	void TakeSystem(ASystem* System);
	
	const TArray<ASystem*>& GetOwnedSystems() const { return OwnedSystems; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlanet* Capital;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<URace*> FounderRaces;
	
	FEmpireSystemChanged SystemChanged;

private:
	void SystemTaken(ASystem* System);
	void SystemLost(ASystem* System);
	
	TArray<IStarisController*> OwningControllers;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<ASystem*> OwnedSystems;
};
