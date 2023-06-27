// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/VanillaGalaxySettings.h"

#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "ImageUtils.h"
#include "UI/SettingsPanel.h"\

void UVanillaGalaxySettings::FillSettingsPanel(USettingsPanel* SettingsPanel, const TSharedPtr<FJsonObject>& Json)
{
	SETTINGS_SECTION("VanillaGalaxySettings", "Galaxy Settings");
	SETTINGS_FIELD_SEED(Seed);
	SETTINGS_FIELD_NUMBER("VanillaGalaxySettings", "System Count", true, true, 1, false, 0, 1000, "", int32, SystemCount);
	SETTINGS_FIELD_NUMBER("VanillaGalaxySettings", "Radius", false, true, 100000, false, 0, 1500000, "", float, GalaxyRadius);
	SETTINGS_FIELD_SELECT_CUSTOM("VanillaGalaxySettings", "Shape", "Spiral", SETTINGS_FIELD_SELECT_ITEMS("Spiral", "Clouds", "Crescent", "Torus", "Webbed Torus", "None", "Custom..."), FSettingsPanelSelectCallbackNative::CreateLambda([&](const FString& Value)
	{
		Json->SetStringField("Shape", Value);
		if (Value == "Custom...")
		{
			
		}
	}));
	Json->SetStringField("Shape", "Spiral");
	
	SETTINGS_SECTION("VanillaGalaxySettings", "System Settings");
	//FIELD_MAP_NUMBER_STRING("VanillaGalaxySettings", "Star Count Distribution");
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "1 Star Amount", false, true, 0, false, 0, 8, "", float, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value)
	{
		Json->GetObjectField("StarCountDistribution")->SetNumberField("1", (float)Value);
	}));
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "2 Star Amount", false, true, 0, false, 0, 1.5, "", float, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value)
	{
		Json->GetObjectField("StarCountDistribution")->SetNumberField("2", (float)Value);
	}));
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "3 Star Amount", false, true, 0, false, 0, 0.5, "", float, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value)
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
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "Black Hole Chance", true, true, 0, true, 100, 5, "%", float, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value)
	{
		Json->SetNumberField("BlackHoleChance", Value * 0.01);
	}));
	Json->SetNumberField("BlackHoleChance", 0.05);
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "Red Star Amount", false, true, 0, false, 0, 1, "", float, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value)
	{
		Json->GetObjectField("StarTypeDistribution")->SetNumberField("red", (float)Value);
	}));
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "Blue Star Amount", false, true, 0, false, 0, 1, "", float, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value)
	{
		Json->GetObjectField("StarTypeDistribution")->SetNumberField("blue", (float)Value);
	}));
	SETTINGS_FIELD_NUMBER_CUSTOM("VanillaGalaxySettings", "Yellow Star Amount", false, true, 0, false, 0, 1, "", float, FSettingsPanelNumberCallbackNative::CreateLambda([&](double Value)
	{
		Json->GetObjectField("StarTypeDistribution")->SetNumberField("yellow", (float)Value);
	}));
	Json->SetObjectField("StarTypeDistribution", MakeShared<FJsonObject>());
	Json->GetObjectField("StarTypeDistribution")->SetNumberField("red", 1);
	Json->GetObjectField("StarTypeDistribution")->SetNumberField("blue", 1);
	Json->GetObjectField("StarTypeDistribution")->SetNumberField("yellow", 1);
}

void UVanillaGalaxySettings::SaveGalaxyMask(UTexture2D* Mask)
{
	GalaxyMask = Mask;
}

UTexture2D* UVanillaGalaxySettings::LoadMaskFromComputer()
{
	//TArray<FString> Files;
	//if (FDesktopPlatformModule::Get()->OpenFileDialog(nullptr, "Select Galaxy Mask", "", "", "png files (*.png)|*.png", EFileDialogFlags::None, Files))
	//{
	//	TArray64<uint8> Buffer;
	//	if (FFileHelper::LoadFileToArray(Buffer, *Files[0]))
	//	{
	//		
	//	}
	//	FMaterialUtilities::CreateTexture(GetTransientPackage(), "/GalaxyMask/" + Files[0].Replace(TEXT("/"), TEXT("_")), )
	//	auto Texture = FImageUtils::ImportFileAsTexture2D(Files[0]);
	//	Texture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
	//	Texture->CompressionSettings = TextureCompressionSettings::TC_VectorDisplacementmap;
	//	Texture->SRGB = false;
	//	Texture->RefreshSamplerStates();
    //
	//	return Texture;
	//}

	return nullptr;
}
