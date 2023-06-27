// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GalaxyMetaData.h"
#include "GameFramework/Actor.h"

#include "Galaxy.generated.h"

class AFleet;
class UShip;
class UColony;
class UStarisInstancedStaticMesh;
class UBuildingShip;
class UScienceShip;
class AStarisGameMode;
class USystem;
class ICelestialEntity;
class IGalaxyGenerator;
class UPlanet;
class UStar;
class AGalaxySettingsManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FGalaxyGameStarted, bool /* Is Very First Start */);

DECLARE_MULTICAST_DELEGATE(FGalaxyTimePasedEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGalaxyTimePasedDynamicEvent);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FGalaxyDateUpdatedEvent, int32 /* New Date */, int32 /* New Month */, int32 /* New Year */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FGalaxyDateUpdatedDynamicEvent, int32, NewDate, int32, NewMonth, int32, NewYear);

DECLARE_MULTICAST_DELEGATE_TwoParams(FGalaxyTimeStateChangedEvent, float TimeScale, bool IsPaused);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGalaxyTimeStateChangedDynamicEvent, float, TimeScale, bool, IsPaused);

UCLASS(BlueprintType)
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
	bool TogglePauseTime();

	UFUNCTION(BlueprintPure)
	float GetRealTimeScale() const { return bPaused ? 0 : TimeScale; }
	
	float GetTimeScale() const { return TimeScale; }
	bool IsPaused() const { return bPaused; }

	int32 GetDate() const { return DaysCounter % 30 + 1; }
	int32 GetMonth() const { return DaysCounter / 30 % 12 + 1; }
	int32 GetYear() const { return DaysCounter / 360 + 1; }

	UPROPERTY(BlueprintAssignable, DisplayName="On Time State Changed")
	FGalaxyTimeStateChangedDynamicEvent OnTimeStateChanged_K2;
	FGalaxyTimeStateChangedEvent OnTimeStateChanged;
#pragma endregion

	UFUNCTION(BlueprintCallable, meta=(WorldContext=WorldContextObject))
	static AGalaxy* GetGalaxy(UObject* WorldContextObject);

	const TArray<USystem*>& GetSystems() const;

	const FName& GetId() const { return Id; }

	UFUNCTION(BlueprintPure)
	static int32 GenerateGalaxySeed();

	UFUNCTION(BlueprintCallable)
	UMeshInstanceRef* CreateStarInstance(UCompositeRecord* StarType, const FTransform& StarTransform);

	UFUNCTION(BlueprintCallable)
	UMeshInstanceRef* CreatePlanetInstance(const FTransform& PlanetTransform);

	void SetStarsScale(float Scale);

	UFUNCTION(BlueprintCallable)
	UObject* GetObjectById(const FName& ObjectId) const;

	UFUNCTION(BlueprintCallable)
	void RegisterObjectById(const FName& ObjectId, UObject* Object);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	UStaticMesh* StarMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	UStaticMesh* PlanetMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	UMaterial* PlanetMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game Settings")
	UMaterialParameterCollection* MaterialParameterCollection;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	TSubclassOf<USystem> SystemClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	TSubclassOf<UStar> StarClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	TSubclassOf<UPlanet> PlanetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	TSubclassOf<UScienceShip> ScienceShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	TSubclassOf<UBuildingShip> BuildingShipClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Game Settings")
	TSubclassOf<AFleet> FleetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int64 DaysCounter;

	FGalaxyGameStarted OnGameStarted;

	UPROPERTY(BlueprintAssignable, DisplayName="On Day Passed")
	FGalaxyTimePasedDynamicEvent OnDayPassed_K2;
	FGalaxyTimePasedEvent OnDayPassed;

	UPROPERTY(BlueprintAssignable, DisplayName="On Month Passed")
	FGalaxyTimePasedDynamicEvent OnMonthPassed_K2;
	FGalaxyTimePasedEvent OnMonthPassed;

	UPROPERTY(BlueprintAssignable, DisplayName="On Year Passed")
	FGalaxyTimePasedDynamicEvent OnYearPassed_K2;
	FGalaxyTimePasedEvent OnYearPassed;

	UPROPERTY(BlueprintAssignable, DisplayName="On Date Updated")
	FGalaxyDateUpdatedDynamicEvent OnDateUpdated_K2;
	FGalaxyDateUpdatedEvent OnDateUpdated;

protected:
	UFUNCTION()
	void DayPassed();

	void Generate();
	void Start();

	UPROPERTY(VisibleAnywhere)
	TMap<UCompositeRecord*, UStarisInstancedStaticMesh*> StarMeshes;

	UPROPERTY(VisibleAnywhere)
	UStarisInstancedStaticMesh* PlanetMeshes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float TimeScale = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bPaused = false;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Id;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int32 Seed;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	TArray<USystem*> Systems;

	TMap<FName, TWeakObjectPtr<UObject>> IdDatabase;

	UPROPERTY()
	FTimerHandle DayTickHandle;

	bool bIsGameStarted;

	float StarSizeBiasCached;

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
