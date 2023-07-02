// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStage.h"
#include "Engine/GameInstance.h"

#include "StarisGameInstance.generated.h"

class UCompositeDatabasePreset;
class UCompositeDatabase;
DECLARE_MULTICAST_DELEGATE_TwoParams(FStarisGameStageChanged, EGameStage /* Old Game Stage */, EGameStage /* New Game Stage */);

class AGalaxySettingsManager;
/**
 * 
 */
UCLASS()
class STARIS_API UStarisGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UStarisGameInstance();
	virtual void Init() override;
	
	UFUNCTION(BlueprintNativeEvent)
	void InitAssets();

	UFUNCTION(BlueprintCallable)
	bool SwitchGameStage(EGameStage NewGameStage);

	UFUNCTION(BlueprintPure)
	static FString MakeUrl(const TMap<FString, FString>& Map);

	UFUNCTION(BlueprintPure)
	UCompositeDatabase* GetStarTypeDatabase() const { return StarTypeDatabase; }

	UFUNCTION(BlueprintPure)
	UCompositeDatabase* GetResourceTypeDatabase() const { return ResourceTypeDatabase; }

	UFUNCTION(BlueprintPure)
	UCompositeDatabase* GetBuildingTypeDatabase() const { return BuildingTypeDatabase; }

	UFUNCTION(BlueprintPure)
	UCompositeDatabase* GetGalaxyBuildingTypeDatabase() const { return GalaxyBuildingTypeDatabase; }

	FStarisGameStageChanged GameStageChanged;
	
	inline static bool DebugToolsEnabled =
#if UE_BUILD_SHIPPING
			false;
#else
			true;
#endif

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	EGameStage GameStage = EGameStage::Loading;

	UPROPERTY(EditAnywhere, Instanced)
	UCompositeDatabasePreset* StarTypeDatabasePreset;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeDatabase* StarTypeDatabase;

	UPROPERTY(EditAnywhere, Instanced)
	UCompositeDatabasePreset* ResourceTypeDatabasePreset;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeDatabase* ResourceTypeDatabase;

	UPROPERTY(EditAnywhere, Instanced)
	UCompositeDatabasePreset* BuildingTypeDatabasePreset;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeDatabase* BuildingTypeDatabase;

	UPROPERTY(EditAnywhere, Instanced)
	UCompositeDatabasePreset* GalaxyBuildingTypeDatabasePreset;
	
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeDatabase* GalaxyBuildingTypeDatabase;
};
