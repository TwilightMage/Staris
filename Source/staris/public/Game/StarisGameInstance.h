// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameStage.h"
#include "Engine/GameInstance.h"

#include "StarisGameInstance.generated.h"

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

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeDatabase* StarTypeDatabase;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeDatabase* ResourceTypeDatabase;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeDatabase* BuildingTypeDatabase;
};
