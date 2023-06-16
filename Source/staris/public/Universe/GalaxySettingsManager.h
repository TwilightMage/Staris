// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GalaxySettingsManager.generated.h"

class UStarisGameInstance;
/**
 * 
 */
UCLASS()
class STARIS_API AGalaxySettingsManager : public AActor
{
	GENERATED_BODY()

	friend UStarisGameInstance;

public:
	UFUNCTION(CallInEditor, Category="Galaxy Settings Manager")
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
	
	const TArray<TSubclassOf<UGalaxySettings>>& GetAllSettingsClasses() const { return SettingsClasses; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<TSubclassOf<UGalaxySettings>, UGalaxySettings*> SettingsObjects;
	
private:
	void FindGalaxySettingsClasses();
	
	TArray<TSubclassOf<UGalaxySettings>> SettingsClasses;
};
