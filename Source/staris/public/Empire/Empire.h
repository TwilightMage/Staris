// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceAmountState.h"
#include "UObject/Object.h"
#include "Empire.generated.h"

class UEmpirePlanetKnowledge;
class UCompositeRecord;
class UColony;
class UContextMenu;
class IFocusable;
class URace;
class UPlanet;
class USystem;
class AStarisPlayerController;
class IStarisController;
class UContextMenuItem;

typedef TMap<UCompositeRecord*, FResourceAmountState> BalanceUpdateData;

DECLARE_MULTICAST_DELEGATE_TwoParams(FEmpireSystemChangedEvent, USystem* /* System */, bool /* Taken or Lost */);
DECLARE_MULTICAST_DELEGATE_OneParam(FEmpireBalanceUpdatedEvent, const BalanceUpdateData& /* New Balance */);

DECLARE_MULTICAST_DELEGATE_OneParam(FEmpirePlanetKnowledgeAddedEvent, UEmpirePlanetKnowledge* /* New Knowledge */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEmpirePlanetKnowledgeAddedDynamicEvent, UEmpirePlanetKnowledge*, NewKnowledge);

/**
 * 
 */
UCLASS()
class STARIS_API UEmpire : public UObject
{
	GENERATED_BODY()

	friend IStarisController;
	friend USystem;
	friend UColony;
	
public:
	const TArray<IStarisController*>& GetOwningControllers() const { return OwningControllers; }

	bool IsAssignedToPlayer() const;

	UFUNCTION(BlueprintCallable)
	void TakeSystem(USystem* System);

	TArray<UContextMenuItem*> CreateContextActions(IFocusable* HoveredFocusable, IFocusable* SelectedFocusable);
	
	const TArray<USystem*>& GetOwnedSystems() const { return OwnedSystems; }

	const TMap<UCompositeRecord*, int32>& GetBalance() const { return CurrentBalance; }
	const TMap<UCompositeRecord*, FResourceAmountState>& GetLatestBalanceUpdate() const { return LatestBalanceUpdate; }

	UFUNCTION(BlueprintPure)
	UEmpirePlanetKnowledge* GetPlanetKnowledge(UPlanet* Planet) const;
	
	UFUNCTION(BlueprintPure)
	UEmpirePlanetKnowledge* GetOrCreatePlanetKnowledge(UPlanet* Planet);

	void MonthPassed();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPlanet* Capital;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<URace*> FounderRaces;
	
	FEmpireSystemChangedEvent OnSystemChanged;
	FEmpireBalanceUpdatedEvent OnBalanceUpdated;

	UPROPERTY(BlueprintAssignable, DisplayName="On Planet Knowledge Added")
	FEmpirePlanetKnowledgeAddedDynamicEvent OnPlanetKnowledgeAdded_K2;
	FEmpirePlanetKnowledgeAddedEvent OnPlanetKnowledgeAdded;

private:
	void ClearControllerList();
	void ControllerAssigned(IStarisController* Controller);
	void ControllerRemoved(IStarisController* Controller);
	
	void SystemTaken(USystem* System);
	void SystemLost(USystem* System);
	
	TArray<IStarisController*> OwningControllers;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<USystem*> OwnedSystems;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UColony*> Colonies;

	UPROPERTY()
	TMap<UCompositeRecord*, int32> CurrentBalance;
	TMap<UCompositeRecord*, FResourceAmountState> LatestBalanceUpdate;

	UPROPERTY()
	TMap<UPlanet*, UEmpirePlanetKnowledge*> PlanetKnowledgeDatabase;
};
