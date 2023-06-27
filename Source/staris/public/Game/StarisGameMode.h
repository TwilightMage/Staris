// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "StarisGameMode.generated.h"

class UEmpireGenerator;
class UGalaxySettings;
class UEmpire;

UCLASS()
class STARIS_API AStarisGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	AStarisGameMode();

	virtual void BeginPlay() override;

	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal) override;
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	const TArray<UEmpire*>& GetEmpires() const { return Empires; }

private:
	void GenerateGalaxy();

	UFUNCTION()
	void GameStarted(bool IsVeryFirstStart);

	void Setup();

	TArray<UEmpireGenerator*> EmpireGenerators;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UEmpire*> Empires;

	bool SetupDone = false;
};
