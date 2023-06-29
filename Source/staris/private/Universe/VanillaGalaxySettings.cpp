// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/VanillaGalaxySettings.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "ImageUtils.h"
#include "UI/SettingsPanel.h"

UVanillaGalaxySettings::UVanillaGalaxySettings()
{
	DefaultShapeMasks = {
		{"Spiral", Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Textures/Galaxy/GalaxyDistribution_uneven_spiral.GalaxyDistribution_uneven_spiral")))},
		{"Clouds", Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Textures/Galaxy/GalaxyDistribution_clouds.GalaxyDistribution_clouds")))},
		{"Crescent", Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Textures/Galaxy/GalaxyDistribution_crescent.GalaxyDistribution_crescent")))},
		{"Torus", Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Textures/Galaxy/GalaxyDistribution_torus.GalaxyDistribution_torus")))},
		{"Webbed Torus", Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, TEXT("/Game/Textures/Galaxy/GalaxyDistribution.GalaxyDistribution")))}
	};
}

void UVanillaGalaxySettings::FillSettingsPanel(USettingsPanel* SettingsPanel, const TSharedPtr<FJsonObject>& Json)
{
	SETTINGS_SECTION("VanillaGalaxySettings", "Galaxy Settings");
	SETTINGS_FIELD_SEED(Seed);
	SETTINGS_FIELD_NUMBER("VanillaGalaxySettings", "System Count", true, true, 1, false, 0, 1000, "", int32, SystemCount);
	SETTINGS_FIELD_NUMBER("VanillaGalaxySettings", "Radius", false, true, 100000, false, 0, 1500000, "", float, GalaxyRadius);

	TArray<FString> ShapeMaskNames;
	DefaultShapeMasks.GetKeys(ShapeMaskNames);
	ShapeMaskNames.Add("None");
	ShapeMaskNames.Add("Custom...");
	SETTINGS_FIELD_SELECT_CUSTOM("VanillaGalaxySettings", "Shape", "Spiral", ShapeMaskNames, FSettingsPanelSelectCallbackNative::CreateWeakLambda(this, [=](const FString& Value)
	{
		if (Value == "Custom...")
		{
			SavedMask = LoadMaskFromComputer();
		}
		else if (Value != "None")
		{
			SavedMask = DefaultShapeMasks[Value];
		}
	}));
	
	SETTINGS_SECTION("VanillaGalaxySettings", "System Settings");
	//FIELD_MAP_NUMBER_STRING("VanillaGalaxySettings", "Star Count Distribution");
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "1 Star Amount", false, true, 0, false, 0, 8, "", float, FSettingsPanelNumberCallbackNative::CreateWeakLambda(this, [=](double Value)
	{
		Json->GetObjectField("StarCountDistribution")->SetNumberField("1", (float)Value);
	}));
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "2 Star Amount", false, true, 0, false, 0, 1.5, "", float, FSettingsPanelNumberCallbackNative::CreateWeakLambda(this, [=](double Value)
	{
		Json->GetObjectField("StarCountDistribution")->SetNumberField("2", (float)Value);
	}));
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "3 Star Amount", false, true, 0, false, 0, 0.5, "", float, FSettingsPanelNumberCallbackNative::CreateWeakLambda(this, [=](double Value)
	{
		Json->GetObjectField("StarCountDistribution")->SetNumberField("3", (float)Value);
	}));
	Json->SetObjectField("StarCountDistribution", MakeShared<FJsonObject>());
	Json->GetObjectField("StarCountDistribution")->SetNumberField("1", 8);
	Json->GetObjectField("StarCountDistribution")->SetNumberField("2", 1.5);
	Json->GetObjectField("StarCountDistribution")->SetNumberField("3", 0.5);
	SETTINGS_FIELD_MIN_MAX("VanillaGalaxySettings", "Planet Amount Range", true, true, 0, false, 0, 3, 6, int32, PlanetAmountRange);

	SETTINGS_SECTION("VanillaGalaxySettings", "Planet Settings");
	SETTINGS_FIELD_MIN_MAX("VanillaGalaxySettings", "Orbit Range", false, true, 200, false, 0, 300, 1300, float, PlanetOrbitRange);
	SETTINGS_FIELD_MIN_MAX("VanillaGalaxySettings", "Temperature Range", true, false, 0, false, 0, -300, 500, float, TemperatureGlobalRange);
	SETTINGS_FIELD_NUMBER("VanillaGalaxySettings", "Temperature Spread", true, true, 0, false, 0, 20, "", int32, TemperatureRandomSpread);
	SETTINGS_FIELD_NUMBER("VanillaGalaxySettings", "Layer Amount", true, true, 1, false, 0, 3, "", int32, LayerNum);
	SETTINGS_FIELD_NUMBER("VanillaGalaxySettings", "Layer Size", true, true, 1, false, 0, 15, "", int32, LayerSize);

	SETTINGS_SECTION("VanillaGalaxySettings", "Star Settings");
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "Black Hole Chance", true, true, 0, true, 100, 5, "%", float, FSettingsPanelNumberCallbackNative::CreateWeakLambda(this, [=](double Value)
	{
		Json->SetNumberField("BlackHoleChance", Value * 0.01);
	}));
	Json->SetNumberField("BlackHoleChance", 0.05);
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "Red Star Amount", false, true, 0, false, 0, 1, "", float, FSettingsPanelNumberCallbackNative::CreateWeakLambda(this, [=](double Value)
	{
		Json->GetObjectField("StarTypeDistribution")->SetNumberField("red", (float)Value);
	}));
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "Blue Star Amount", false, true, 0, false, 0, 1, "", float, FSettingsPanelNumberCallbackNative::CreateWeakLambda(this, [=](double Value)
	{
		Json->GetObjectField("StarTypeDistribution")->SetNumberField("blue", (float)Value);
	}));
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "Yellow Star Amount", false, true, 0, false, 0, 1, "", float, FSettingsPanelNumberCallbackNative::CreateWeakLambda(this, [=](double Value)
	{
		Json->GetObjectField("StarTypeDistribution")->SetNumberField("yellow", (float)Value);
	}));
	Json->SetObjectField("StarTypeDistribution", MakeShared<FJsonObject>());
	Json->GetObjectField("StarTypeDistribution")->SetNumberField("red", 1);
	Json->GetObjectField("StarTypeDistribution")->SetNumberField("blue", 1);
	Json->GetObjectField("StarTypeDistribution")->SetNumberField("yellow", 1);
}

UTexture2D* UVanillaGalaxySettings::LoadMaskFromComputer()
{
	TArray<FString> Files;
	if (FDesktopPlatformModule::Get()->OpenFileDialog(GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle(), "Select Galaxy Mask", "", "", "png files (*.png)|*.png", EFileDialogFlags::None, Files))
	{
		auto Texture = FImageUtils::ImportFileAsTexture2D(Files[0]);
		//Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
		//Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
		//Texture->SRGB = false;
		//
		//Texture->UpdateResource();

		Texture->AddToRoot();
    
		return Texture;
	}

	return nullptr;
}
