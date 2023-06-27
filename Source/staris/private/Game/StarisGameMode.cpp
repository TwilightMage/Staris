// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisGameMode.h"

#include "JsonObjectConverter.h"
#include "Empire/Empire.h"
#include "Empire/Race.h"
#include "Empire/UnitedEmpireGenerator.h"
#include "Game/StarisPlayerController.h"
#include "Game/StarisPlayerPawn.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "UI/SettingsPanel.h"
#include "Universe/Galaxy.h"
#include "Universe/GalaxySettingsManager.h"
#include "Universe/VanillaGalaxySettings.h"

AStarisGameMode::AStarisGameMode()
	: AGameMode()
{
	PlayerControllerClass = AStarisPlayerController::StaticClass();
	DefaultPawnClass = AStarisPlayerPawn::StaticClass();
}

void AStarisGameMode::BeginPlay()
{
	Super::BeginPlay();

	Setup();
}

FString AStarisGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	Setup();
	
	if (auto Player = Cast<AStarisPlayerController>(NewPlayerController))
	{
		for (auto& Empire : Empires)
		{
			if (!Empire->IsAssignedToPlayer())
			{
				Player->AssignEmpire(Empire);
				break;
			}
		}
	}
	
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
}

void AStarisGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString GalaxySettingsParam = FGenericPlatformHttp::GetUrlParameter(Options, FString("GalaxySettings")).Get("");
	FString EmpireSettingsParam = FGenericPlatformHttp::GetUrlParameter(Options, FString("EmpireSettings")).Get("");

	AGalaxySettingsManager* GalaxySettingsManager = GetWorld()->SpawnActor<AGalaxySettingsManager>();
	
	if (!GalaxySettingsParam.IsEmpty())
	{
		FJsonObjectWrapper GalaxySettings;
		GalaxySettings.JsonObjectFromString(FGenericPlatformHttp::UrlDecode(GalaxySettingsParam));

		GalaxySettingsManager->UpdateSettingsSet();

		if (auto VanillaGalaxySettingsJson = GalaxySettings.JsonObject->Values.FindRef(UVanillaGalaxySettings::StaticClass()->GetClassPathName().ToString()))
		{
			auto Data = VanillaGalaxySettingsJson->AsObject();
			auto VanillaGalaxySettings = GalaxySettingsManager->GetSettings<UVanillaGalaxySettings>();
			VanillaGalaxySettings->GalaxyHeight = 0;
			VanillaGalaxySettings->SystemDistributionMap = UVanillaGalaxySettings::SavedMask;
			TRY_READ_SETTINGS_NUMBER(Data, "Seed",                    VanillaGalaxySettings->Seed);
			TRY_READ_SETTINGS_NUMBER(Data, "SystemCount",             VanillaGalaxySettings->SystemCount);
			TRY_READ_SETTINGS_NUMBER(Data, "GalaxyRadius",            VanillaGalaxySettings->GalaxyRadius);
			TRY_READ_SETTINGS_MAP(   Data, "StarCountDistribution",   VanillaGalaxySettings->StarCountDistribution, ParseInt(Pair.Key), Number);
			TRY_READ_SETTINGS_STRUCT(Data, "PlanetAmountRange",       VanillaGalaxySettings->PlanetAmountRange, FMinMaxInt32);
			TRY_READ_SETTINGS_STRUCT(Data, "PlanetOrbitRange",        VanillaGalaxySettings->PlanetOrbitRange, FMinMaxFloat);
			TRY_READ_SETTINGS_STRUCT(Data, "TemperatureGlobalRange",  VanillaGalaxySettings->TemperatureGlobalRange, FMinMaxInt32);
			TRY_READ_SETTINGS_NUMBER(Data, "TemperatureRandomSpread", VanillaGalaxySettings->TemperatureRandomSpread);
			TRY_READ_SETTINGS_NUMBER(Data, "LayerNum",                VanillaGalaxySettings->LayerNum);
			TRY_READ_SETTINGS_NUMBER(Data, "LayerSize",               VanillaGalaxySettings->LayerSize);
			TRY_READ_SETTINGS_NUMBER(Data, "BlackHoleChance",         VanillaGalaxySettings->BlackHoleChance);
			TRY_READ_SETTINGS_MAP(   Data, "StarTypeDistribution",    VanillaGalaxySettings->StarTypeDistribution, FName(Pair.Key), Number);
		}
	}

	if (!EmpireSettingsParam.IsEmpty())
	{
		FJsonObjectWrapper EmpireSettings;
		EmpireSettings.JsonObjectFromString(FGenericPlatformHttp::UrlDecode(EmpireSettingsParam));

		EmpireGeneratorSettings = EmpireSettings.JsonObject;
	}
}

void AStarisGameMode::GenerateGalaxy()
{
	if (auto Galaxy = AGalaxy::GetGalaxy(this))
	{
		Galaxy->Generate();
	}
}

void AStarisGameMode::GameStarted(bool IsVeryFirstStart)
{
	
}

void AStarisGameMode::Setup()
{
	if (SetupDone) return;

	GenerateGalaxy();

	if (auto Galaxy = AGalaxy::GetGalaxy(this))
	{
		Galaxy->OnGameStarted.AddUObject(this, &AStarisGameMode::GameStarted);

		auto EmpireGenerator = NewObject<UUnitedEmpireGenerator>();
		TRY_READ_SETTINGS_STRING(EmpireGeneratorSettings, "EmpireTitle", EmpireGenerator->EmpireTitle);
		TRY_READ_SETTINGS_STRING(EmpireGeneratorSettings, "SystemTitle", EmpireGenerator->SystemTitle);
		TRY_READ_SETTINGS_STRING(EmpireGeneratorSettings, "CapitalTitle", EmpireGenerator->CapitalTitle);
		auto EmpireRace = NewObject<URace>();
		TRY_READ_SETTINGS_STRING(EmpireGeneratorSettings, "FounderRace", EmpireRace->Title);
		EmpireGenerator->EmpireRaces = { EmpireRace };
		TRY_READ_SETTINGS_NUMBER(EmpireGeneratorSettings, "DesiredPopulation", EmpireGenerator->TotalPops);
		int32 EmpireSeed = 0;
		TRY_READ_SETTINGS_NUMBER(EmpireGeneratorSettings, "Seed", EmpireSeed);
		Empires.Add(EmpireGenerator->Generate(Galaxy, EmpireSeed));
	}
	
	SetupDone = true;
}
