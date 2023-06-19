// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GalaxyMetaData.h"
#include "GameFramework/Actor.h"

#include "Galaxy.generated.h"

class UBuildingShip;
class UScienceShip;
class AStarisGameMode;
class ASystem;
class ICelestialEntity;
class IGalaxyGenerator;
class UPlanet;
class UStar;
class AGalaxySettingsManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FGalaxyGameStarted, bool /* Is Very First Start */);

UCLASS()
class STARIS_API AGalaxy : public AActor
{
	GENERATED_BODY()

	friend ICelestialEntity;
	friend AStarisGameMode;
	
public:
	AGalaxy();

	virtual void BeginPlay() override;

	virtual void ApplyPattern(const FGalaxyMetaData& Data);

	bool IsGameStarted() const { return bIsGameStarted; }

#pragma region Time
	UFUNCTION(BlueprintCallable)
	void SetTimeScale(float NewTimeScale);

	UFUNCTION(BlueprintCallable)
	void PauseTime(bool Pause);

	UFUNCTION(BlueprintCallable)
	void TogglePauseTime();
#pragma endregion

	UFUNCTION(BlueprintCallable, meta=(WorldContext=WorldContextObject))
	static AGalaxy* GetGalaxy(UObject* WorldContextObject);

	const TArray<ASystem*>& GetSystems() const;

	const FName& GetId() const { return Id; }
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ASystem> SystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UStar> StarClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UPlanet> PlanetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UScienceShip> ScienceShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UBuildingShip> BuildingShipClass;

	UPROPERTY(BlueprintReadWrite)
	int64 DaysCounter;

	FGalaxyGameStarted GameStarted;

protected:
	UFUNCTION()
	void DayPassed();

	void Generate();
	void Start();

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	float TimeScale = 0;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Id;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<ASystem*> Systems;

	UPROPERTY()
	FTimerHandle DayTickHandle;

	bool bIsGameStarted;

	TArray<ICelestialEntity*> CelestialEntities;

	TArray<ICelestialEntity*> DayUpdateCelestialEntities;
	TArray<ICelestialEntity*> AddDayUpdateCelestialEntities;
	TArray<ICelestialEntity*> RemoveDayUpdateCelestialEntities;
	
	TArray<ICelestialEntity*> MonthUpdateCelestialEntities;
	TArray<ICelestialEntity*> AddMonthUpdateCelestialEntities;
	TArray<ICelestialEntity*> RemoveMonthUpdateCelestialEntities;

	TArray<ICelestialEntity*> YearUpdateCelestialEntities;
	TArray<ICelestialEntity*> AddYearUpdateCelestialEntities;
	TArray<ICelestialEntity*> RemoveYearUpdateCelestialEntities;
};
