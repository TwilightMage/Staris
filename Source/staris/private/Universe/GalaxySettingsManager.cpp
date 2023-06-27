// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/GalaxySettingsManager.h"

#include "Universe/GalaxySettings.h"

void AGalaxySettingsManager::UpdateSettingsSet()
{
	SettingsClasses = FindGalaxySettingsClasses();
	
	for (auto& SettingsClass : SettingsClasses)
	{
		auto& Object = SettingsObjects.FindOrAdd(SettingsClass);
		if (!Object)
		{
			Object = NewObject<UGalaxySettings>(this, SettingsClass);
		}
	}
}

UGalaxySettings* AGalaxySettingsManager::GetSettings_K2(const TSubclassOf<UGalaxySettings>& Class) const
{
	if (auto Settings = SettingsObjects.FindRef(Class))
	{
		return Settings;
	}
		
	return nullptr;
}

TArray<TSubclassOf<UGalaxySettings>> AGalaxySettingsManager::FindGalaxySettingsClasses()
{
	TArray<TSubclassOf<UGalaxySettings>> Result;
	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(UGalaxySettings::StaticClass()) && !(It->ClassFlags & CLASS_Abstract))
		{
			Result.AddUnique(*It);
		}
	}

	return Result;
}
