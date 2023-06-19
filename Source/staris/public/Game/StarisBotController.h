// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StarisController.h"

#include "StarisBotController.generated.h"

UCLASS()
class STARIS_API AStarisBotController : public AActor, public IStarisController
{
	GENERATED_BODY()

public:
	AStarisBotController();

	virtual UEmpire* GetEmpire() const override;
	virtual void AssignEmpire(UEmpire* Empire) override;

	virtual bool IsPlayer() const override { return false; }
	virtual FString GetControllerName() const override { return GetName(); }

private:
	UPROPERTY()
	UEmpire* OwnedEmpire;

	UPROPERTY()
	FString BotName;
};
