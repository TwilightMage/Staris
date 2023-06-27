// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisGameMode.h"

#include "JsonObjectConverter.h"
#include "StarisStatics.h"
#include "Empire/Empire.h"
#include "Empire/Race.h"
#include "Empire/UnitedEmpireGenerator.h"
#include "Game/StarisGameInstance.h"
#include "Game/StarisPlayerController.h"
#include "Game/StarisPlayerPawn.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Misc/DefaultValueHelper.h"
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

#define TRY_SET_GALAXY_NUMBER(Param) if (GalaxySettings.JsonObject->HasTypedField<EJson::Number>(#Param)) VanillaGalaxySettings->Param = GalaxySettings.JsonObject->GetNumberField(#Param);

#define TRY_SET_GALAXY_MAP(Param, PairKey, ValueType)                                                           \
if (GalaxySettings.JsonObject->HasTypedField<EJson::Object>(#Param))                                            \
{                                                                                                               \
	VanillaGalaxySettings->Param.Reset();                                                                       \
	auto Obj = GalaxySettings.JsonObject->GetObjectField(#Param);                                               \
	for (auto Pair : Obj->Values)                                                                               \
	{                                                                                                           \
		VanillaGalaxySettings->Param.Add(PairKey, Pair.Value->As##ValueType());                                 \
	}                                                                                                           \
}                                                                                                               \

#define TRY_SET_GALAXY_STRUCT(Param, StructType) \
if (GalaxySettings.JsonObject->HasTypedField<EJson::Object>(#Param))                                                                                                                     \
{                                                                                                                                                                                                     \
	FJsonObjectConverter::JsonObjectToUStruct(GalaxySettings.JsonObject->GetObjectField(#Param).ToSharedRef(), StructType::StaticStruct(), &VanillaGalaxySettings->Param); \
}                                                                                                                                                                                                     \

int32 ParseInt(const FString& Str) { int32 Result; return (FDefaultValueHelper::ParseInt(Str, Result)) ? Result : 0; }

void AStarisGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	FString GalaxySettingsParam = FGenericPlatformHttp::GetUrlParameter(Options, FString("GalaxySettings")).Get("");

	AGalaxySettingsManager* GalaxySettingsManager = GetActorOfClass<AGalaxySettingsManager>(this);

	GalaxySettingsManager = GetWorld()->SpawnActor<AGalaxySettingsManager>();
	
	if (!GalaxySettingsParam.IsEmpty())
	{
		FJsonObjectWrapper GalaxySettings;
		GalaxySettings.JsonObjectFromString(FGenericPlatformHttp::UrlDecode(GalaxySettingsParam));

		GalaxySettingsManager->UpdateSettingsSet();

		auto VanillaGalaxySettings = GalaxySettingsManager->GetSettings<UVanillaGalaxySettings>();
		TRY_SET_GALAXY_NUMBER(Seed);
		TRY_SET_GALAXY_NUMBER(SystemCount);
		TRY_SET_GALAXY_NUMBER(GalaxyRadius);
		VanillaGalaxySettings->GalaxyHeight = 0;
		VanillaGalaxySettings->SystemDistributionMap = UVanillaGalaxySettings::GalaxyMask;
		TRY_SET_GALAXY_MAP(StarCountDistribution, ParseInt(Pair.Key), Number);
		TRY_SET_GALAXY_STRUCT(PlanetAmountRange, FMinMaxInt32);
		TRY_SET_GALAXY_STRUCT(PlanetOrbitRange, FMinMaxFloat);
		TRY_SET_GALAXY_STRUCT(TemperatureGlobalRange, FMinMaxInt32);
		TRY_SET_GALAXY_NUMBER(TemperatureRandomSpread);
		TRY_SET_GALAXY_NUMBER(LayerNum);
		TRY_SET_GALAXY_NUMBER(LayerSize);
		TRY_SET_GALAXY_NUMBER(BlackHoleChance);
		TRY_SET_GALAXY_MAP(StarTypeDistribution, FName(Pair.Key), Number);
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
		EmpireGenerator->EmpireTitle = "Great Foxik Empire";
		EmpireGenerator->SystemTitle = "Solus";
		EmpireGenerator->CapitalTitle = "Terra";
		auto EmpireRace = NewObject<URace>();
		EmpireRace->Title = "Foxus";
		EmpireGenerator->EmpireRaces = { EmpireRace };
		EmpireGenerator->TotalPops = 8000000;
		Empires.Add(EmpireGenerator->Generate(Galaxy, 123));
	}
	
	SetupDone = true;
}
