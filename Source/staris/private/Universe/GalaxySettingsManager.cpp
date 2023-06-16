// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/GalaxySettingsManager.h"

#include "Universe/GalaxySettings.h"

void AGalaxySettingsManager::UpdateSettingsSet()
{
	FindGalaxySettingsClasses();
	
	for (auto& SettingsClass : SettingsClasses)
	{
		auto& Object = SettingsObjects.FindOrAdd(SettingsClass);
		if (!Object)
		{
			Object = NewObject<UGalaxySettings>(this, SettingsClass);
		}
	}
}

void AGalaxySettingsManager::FindGalaxySettingsClasses()
{
	for (TObjectIterator<UClass> It; It; ++It)
	{
		if (It->IsChildOf(UGalaxySettings::StaticClass()) && !(It->ClassFlags & CLASS_Abstract))
		{
			SettingsClasses.AddUnique(*It);
		}
	}
}
