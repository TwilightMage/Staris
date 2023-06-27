// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GalaxySettingsManager.generated.h"

class UStarisGameInstance;
class UGalaxySettings;

/**
 * 
 */
UCLASS()
class STARIS_API AGalaxySettingsManager : public AActor
{
	GENERATED_BODY()

	friend UStarisGameInstance;

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category="Galaxy Settings Manager")
	void UpdateSettingsSet();

	template<typename T>
	T* GetSettings()
	{
		if (auto Settings = SettingsObjects.Find(T::StaticClass()))
		{
			return Cast<T>(*Settings);
		}
		
		return nullptr;
	}

	UFUNCTION(BlueprintCallable, DisplayName="Get Settings", meta=(AutoCreateRefTerm=Class, DeterminesOutputType=Class))
	UGalaxySettings* GetSettings_K2(const TSubclassOf<UGalaxySettings>& Class) const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<UGalaxySettings>, UGalaxySettings*> SettingsObjects;

	UFUNCTION(BlueprintPure)
	static TArray<TSubclassOf<UGalaxySettings>> FindGalaxySettingsClasses();
	
private:
	TArray<TSubclassOf<UGalaxySettings>> SettingsClasses;
};
