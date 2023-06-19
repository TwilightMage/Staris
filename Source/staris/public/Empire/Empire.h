// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Empire.generated.h"

class UContextMenu;
class IFocusable;
class URace;
class UPlanet;
class ASystem;
class AStarisPlayerController;
class IStarisController;
class UContextMenuItem;

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

	bool IsAssignedToPlayer() const;

	UFUNCTION(BlueprintCallable)
	void TakeSystem(ASystem* System);

	TArray<UContextMenuItem*> CreateContextActions(IFocusable* HoveredFocusable, IFocusable* SelectedFocusable);
	
	const TArray<ASystem*>& GetOwnedSystems() const { return OwnedSystems; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlanet* Capital;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<URace*> FounderRaces;
	
	FEmpireSystemChanged SystemChanged;

private:
	void ClearControllerList();
	void ControllerAssigned(IStarisController* Controller);
	void ControllerRemoved(IStarisController* Controller);
	
	void SystemTaken(ASystem* System);
	void SystemLost(ASystem* System);
	
	TArray<IStarisController*> OwningControllers;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<ASystem*> OwnedSystems;
};
