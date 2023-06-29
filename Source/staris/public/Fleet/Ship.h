// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Fleet.h"
#include "Focusable.h"
#include "Components/SceneComponent.h"
#include "..\Universe\Titled.h"
#include "Universe/LabelableProxy.h"
#include "Ship.generated.h"

class UMeshInstanceRef;
class UCompositeRecord;
class AFleet;

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARIS_API UShip : public UObject, public IFocusable, public ILabeledProxy
{
	GENERATED_BODY()
public:
	UShip();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Setup(UEmpire* Empire);

	virtual void SetupToolTip(UToolTip* ToolTip) override;
	virtual void OnSelected() override;

	virtual ILabeled* GetLabeled() const override;

	UFUNCTION(BlueprintPure)
	virtual bool CanExecuteOrder(UFleetOrder* Order) const { return false; }

	UFUNCTION(BlueprintPure)
	virtual bool FullFleetShip() const { return false; }

	UFUNCTION(BlueprintPure)
	AFleet* GetFleet() const { return Cast<AFleet>(GetOuter()); }

	UFUNCTION(BlueprintPure)
	UCompositeRecord* GetType() const { return Type; }

	UFUNCTION(BlueprintPure)
	float GetSpeed() const;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ControlRequired;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* StaticMesh;

	UPROPERTY()
	UMeshInstanceRef* MeshInstanceRef;

	virtual void TickOrder(float TimeDelta) {}
	
	void Tick(float TickDelta, const FTransform& FleetTransform, FTransform& NewShipTransform);
	FTransform Transform;
	FVector PositionInFormation;

	virtual FText GetClassTitle() const { return FText(); }

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	UCompositeRecord* Type;
};
