// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StarisController.h"
#include "GameFramework/PlayerController.h"

#include "StarisPlayerController.generated.h"

class IFocusable;
class UToolTip;

UCLASS()
class STARIS_API AStarisPlayerController : public APlayerController, public IStarisController
{
	GENERATED_BODY()

public:
	AStarisPlayerController();

	virtual void Tick(float DeltaSeconds) override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual UEmpire* GetEmpire() const override;
	virtual void OnEmpireAssigned(UEmpire* Empire) override;

	virtual bool IsPlayer() const override { return true; }
	virtual FString GetControllerName() const override { return GetName(); }

private:
	UPROPERTY()
	UEmpire* OwnedEmpire;

	IFocusable* TargetedFocusable;

	UPROPERTY()
	UToolTip* CurrentToolTip;
};
