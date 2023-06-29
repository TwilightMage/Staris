// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Focusable.h"
#include "Empire/Empire.h"
#include "GameFramework/Actor.h"
#include "Universe/Labelable.h"
#include "Universe/Titled.h"

#include "Fleet.generated.h"

class AGalaxy;
class UEmpire;
class UFleetPathNode;
class UFleetPath;
class UFleetOrder;
class UStarisInstancedStaticMesh;
class UShip;

DECLARE_MULTICAST_DELEGATE_OneParam(FFleetOrderStartedEvent, UFleetOrder* /* New Order */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFleetOrderStartedDynamicEvent, UFleetOrder*, NewOrder);

USTRUCT(BlueprintType)
struct FOrderExecutionState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFleetOrder* ActiveOrder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UFleetPathNode* CurrentPathNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPathNodeDistance;
};

UCLASS()
class STARIS_API AFleet : public AActor, public ITitled, public IFocusable, public ILabeled
{
	GENERATED_BODY()

	friend UShip;

public:
	AFleet();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual FText GetTitle() const override;
	virtual UTexture2D* GetIcon() const override;

	virtual void SetupToolTip(UToolTip* ToolTip) override;
	virtual void OnSelected() override;
	
	virtual USceneLabel* CreateLabel() override { return nullptr; }
	virtual void SetupLabel(USceneLabel* Label) override {}
	virtual FVector GetLabelLocation() const override;
	
	void Setup(UEmpire* Empire);
	
	UFUNCTION(BlueprintCallable)
	bool AssignShip(UShip* Ship, bool bForce);

	UFUNCTION(BlueprintPure)
	bool CanExecuteOrder(UFleetOrder* Order) const;
	
	UFUNCTION(BlueprintCallable)
	void PutOrder(UFleetOrder* Order, bool bAssign = false);

	UFUNCTION(BlueprintCallable)
	void PutOrderAuto(UFleetOrder* Order);

	UFUNCTION(BlueprintCallable)
	void CancelActiveOrder(bool bCancelAllOrders = false);

	const TArray<UShip*>& GetShips() const { return Ships; }

	float GetSpeed() const { return SpeedCached; }
	
	UEmpire* GetEmpire() const { return OwningEmpire.Get(); }

	int32 GetCurrentUsedControl();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ControlLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title = NSLOCTEXT("Fleet", "Unnamed Fleet", "Unnamed Fleet").ToString();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Icon;

	UPROPERTY(BlueprintAssignable, DisplayName="On Order Started")
	FFleetOrderStartedDynamicEvent OnOrderStarted_K2;
	FFleetOrderStartedEvent OnOrderStarted;

protected:
	UFUNCTION(BlueprintCallable)
	TArray<FVector> CalculateFormation() const;
	
private:
	void StartNextOrder();
	void PathNodeFinished();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UShip*> Ships;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TMap<UStaticMesh*, UStarisInstancedStaticMesh*> InstancedShipMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UFleetOrder*> Orders;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	FOrderExecutionState OrderExecutionState;

	UPROPERTY()
	AGalaxy* Galaxy;
	
	float SpeedCached;

	TWeakObjectPtr<UEmpire> OwningEmpire;
};
