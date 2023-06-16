// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "StarisGameMode.generated.h"

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

private:
	void GenerateGalaxy();
	TArray<UEmpire*> PopulateEmpires(int32 Num);

	UFUNCTION()
	void GameStarted(bool IsVeryFirstStart);

	void Setup();

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<UEmpire*> Empires;

	UPROPERTY()
	TMap<TSubclassOf<UGalaxySettings>, UGalaxySettings*> GalaxySettings;

	bool SetupDone = false;
};
