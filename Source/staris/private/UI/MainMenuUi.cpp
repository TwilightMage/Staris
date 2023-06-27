// Copyright DragonCap. All rights reserved.


#include "UI/MainMenuUi.h"

#include "Empire/UnitedEmpireGenerator.h"
#include "UI/SettingsPanel.h"
#include "Universe/GalaxySettings.h"
#include "Universe/GalaxySettingsManager.h"

void UMainMenuUi::CreateGameSettingsPanels(const FMainMenuUiSettingsPanelDelegate& GalaxySettingsAppendDelegate, const FMainMenuUiSettingsPanelDelegate& EmpireSettingsAppendDelegate)
{
	GalaxySettingsJsons = MakeShared<FJsonObject>();
	
	TArray<TSubclassOf<UGalaxySettings>> GalaxySettingsClasses = AGalaxySettingsManager::FindGalaxySettingsClasses();
	
	for (auto Class : GalaxySettingsClasses)
	{
		auto JsonObject = MakeShared<FJsonObject>();
		auto GalaxyPanel = NewObject<USettingsPanel>(GetTransientPackage(), SettingsPanelClass);
		GalaxyPanel->SetOwningPlayer(GetOwningPlayer());
		GalaxySettingsAppendDelegate.ExecuteIfBound(GalaxyPanel);
		Class.GetDefaultObject()->FillSettingsPanel(GalaxyPanel, JsonObject);
		GalaxySettingsJsons->SetObjectField(Class->GetClassPathName().ToString(), JsonObject);
	}

	TSubclassOf<UEmpireGenerator> EmpireSettingsClass = UUnitedEmpireGenerator::StaticClass();
	{
		EmpireSettingsJson = MakeShared<FJsonObject>();
		auto EmpirePanel = NewObject<USettingsPanel>(GetTransientPackage(), SettingsPanelClass);
		EmpirePanel->SetOwningPlayer(GetOwningPlayer());
		EmpireSettingsAppendDelegate.ExecuteIfBound(EmpirePanel);
		EmpireSettingsClass.GetDefaultObject()->FillSettingsPanel(EmpirePanel, EmpireSettingsJson);
	}
}

FString UMainMenuUi::GetGalaxySettingsJsonString() const
{
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(GalaxySettingsJsons.ToSharedRef(), Writer);

	return OutputString;
}

FString UMainMenuUi::GetEmpireSettingsJsonString() const
{
	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(EmpireSettingsJson.ToSharedRef(), Writer);

	return OutputString;
}
