// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisGameInstance.h"

#include "Empire/BuildingTypeProperties.h"
#include "Empire/ResourceTypeProperties.h"
#include "Empire/VanillaBuildingTypeProperties.h"
#include "Empire/VanillaResourceTypeProperties.h"
#include "GenericPlatform/GenericPlatformHttp.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/LetterNames.h"
#include "Universe/StarNames.h"
#include "Universe/StarTypeProperties.h"

void UStarisGameInstance::InitAssets_Implementation ()
{
	UVanillaResourceTypeProperties* ResourceProps;

	ResourceProps = ResourceTypeDatabase->GetOrCreateRecord("ferite")->GetOrCreateComponent<UVanillaResourceTypeProperties>();
	ResourceProps->Title = NSLOCTEXT("Resources", "Ferite", "Ferite");
	ResourceProps->MineralDensityPerLayer = {0.5, 0.6, 0.9};

	ResourceProps = ResourceTypeDatabase->GetOrCreateRecord("gold")->GetOrCreateComponent<UVanillaResourceTypeProperties>();
	ResourceProps->Title = NSLOCTEXT("Resources", "Gold", "Gold");
	ResourceProps->MineralDensityPerLayer = {0.2, 0.3, 0.4};

	ResourceProps = ResourceTypeDatabase->GetOrCreateRecord("titanium")->GetOrCreateComponent<UVanillaResourceTypeProperties>();
	ResourceProps->Title = NSLOCTEXT("Resources", "Titanium", "Titanium");
	ResourceProps->MineralDensityPerLayer = {0.4, 0.5, 0.7};

	ResourceProps = ResourceTypeDatabase->GetOrCreateRecord("aluminium")->GetOrCreateComponent<UVanillaResourceTypeProperties>();
	ResourceProps->Title = NSLOCTEXT("Resources", "Aluminium", "Aluminium");
	ResourceProps->MineralDensityPerLayer = {0.4, 0.5, 0.3};
	
	UVanillaBuildingTypeProperties* BuildingProps;

	BuildingProps = BuildingTypeDatabase->GetOrCreateRecord("shipyard")->GetOrCreateComponent<UVanillaBuildingTypeProperties>();
	BuildingProps->Title = NSLOCTEXT("Buildings", "ShipYard_Title", "Ship Yard");
	BuildingProps->Description = NSLOCTEXT("Buildings", "ShipYard_Description", "Allows you to build ships on planet.");
	BuildingProps->SingleOfType = true;
	BuildingProps->AddFlags = {"shipyard"};

	BuildingProps = BuildingTypeDatabase->GetOrCreateRecord("dome")->GetOrCreateComponent<UVanillaBuildingTypeProperties>();
	BuildingProps->Title = NSLOCTEXT("Buildings", "Dome_Title", "Dome");
	BuildingProps->Description = NSLOCTEXT("Buildings", "Dome_Description", "Habitants can live under that dome. Partly negates disadvantages of extreme temperature and toxic atmosphere.");
	BuildingProps->SingleOfType = true;
	BuildingProps->AddFlags = {"dome", "ignore_temperature"};
}

UStarisGameInstance::UStarisGameInstance()
	: Super()
{
	RegisterStarNames();
	RegisterLetterNames();

	auto DebugToolsConsoleVar = IConsoleManager::Get().RegisterConsoleVariableRef(TEXT("Staris.DebugTools"), DebugToolsEnabled, TEXT("Enable debug tools"), ECVF_Cheat);
}

void UStarisGameInstance::Init()
{
	Super::Init();

	StarTypeDatabase = NewObject<UCompositeDatabase>(this);
	StarTypeDatabase->RecordComponentType = UStarTypeProperties::StaticClass();

	ResourceTypeDatabase = NewObject<UCompositeDatabase>(this);
	ResourceTypeDatabase->RecordComponentType = UResourceTypeProperties::StaticClass();

	BuildingTypeDatabase = NewObject<UCompositeDatabase>(this);
	BuildingTypeDatabase->RecordComponentType = UBuildingTypeProperties::StaticClass();

	InitAssets();
}

bool UStarisGameInstance::SwitchGameStage(EGameStage NewGameStage)
{
	auto OldStage = GameStage;
	if (GameStage == EGameStage::Loading)
	{
		if (NewGameStage == EGameStage::MainMenu)
		{
			// OK
			GameStage = NewGameStage;
			GameStageChanged.Broadcast(OldStage, NewGameStage);
			return true;
		}
		else if (NewGameStage == EGameStage::Unloading)
		{
			// OK
			GameStage = NewGameStage;
			GameStageChanged.Broadcast(OldStage, NewGameStage);
			return true;
		}
	}
	else if (GameStage == EGameStage::MainMenu)
	{
		if (NewGameStage == EGameStage::Playing)
		{
			// OK
			GameStage = NewGameStage;
			GameStageChanged.Broadcast(OldStage, NewGameStage);
			return true;
		}
		else if (NewGameStage == EGameStage::Unloading)
		{
			// OK
			GameStage = NewGameStage;
			GameStageChanged.Broadcast(OldStage, NewGameStage);
			return true;
		}
	}
	else if (GameStage == EGameStage::Playing)
	{
		if (NewGameStage == EGameStage::MainMenu)
		{
			// OK
			GameStage = NewGameStage;
			GameStageChanged.Broadcast(OldStage, NewGameStage);
			return true;
		}
		else if (NewGameStage == EGameStage::Unloading)
		{
			// OK
			GameStage = NewGameStage;
			GameStageChanged.Broadcast(OldStage, NewGameStage);
			return true;
		}
	}

	// NOT OK
	return false;
}

FString UStarisGameInstance::MakeUrl(const TMap<FString, FString>& Map)
{
	TArray<FString> ResultArray;
	for (auto& Pair : Map)
	{
		if (Pair.Value.IsEmpty())
		{
			ResultArray.Add(Pair.Key);
		}
		else
		{
			ResultArray.Add(Pair.Key + "=" + FGenericPlatformHttp::UrlEncode(Pair.Value));
		}
	}

	return FString::Join(ResultArray, TEXT("&"));
}
