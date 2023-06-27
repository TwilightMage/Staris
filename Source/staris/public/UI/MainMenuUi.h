// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectWrapper.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUi.generated.h"

class USettingsPanel;
class UWrapBox;

DECLARE_DYNAMIC_DELEGATE_OneParam(FMainMenuUiSettingsPanelDelegate, USettingsPanel*, SettingsPanel);

UCLASS()
class STARIS_API UMainMenuUi : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	void CreateGameSettingsPanels(const FMainMenuUiSettingsPanelDelegate& GalaxySettingsAppendDelegate, const FMainMenuUiSettingsPanelDelegate& EmpireSettingsAppendDelegate);

	UFUNCTION(BlueprintPure)
	FString GetGalaxySettingsJsonString() const;

	UFUNCTION(BlueprintPure)
	FString GetEmpireSettingsJsonString() const;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USettingsPanel> SettingsPanelClass;
	
private:
	TSharedPtr<FJsonObject> GalaxySettingsJsons;
	TSharedPtr<FJsonObject> EmpireSettingsJson;
};
