// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisGameInstance.h"

#include "..\..\public\Universe\CompositeDatabase.h"
#include "Universe/StarTypeProperties.h"

void UStarisGameInstance::InitAssets_Implementation ()
{
	//FStarMetaData::Collection.Add(FStarMetaData::TYPE_Blue, UJsonUtils::StringToJson("{\"color\": {\"R\": 0, \"G\": 0, \"B\": 1, \"A\": 1}}"));
	//FStarMetaData::Collection.Add(FStarMetaData::TYPE_Red, UJsonUtils::StringToJson("{\"color\": {\"R\": 1, \"G\": 0, \"B\": 0, \"A\": 1}}"));
	//FStarMetaData::Collection.Add(FStarMetaData::TYPE_Yellow, UJsonUtils::StringToJson("{\"color\": {\"R\": 1, \"G\": 1, \"B\": 0, \"A\": 1}}"));
	//FStarMetaData::Collection.Add(FStarMetaData::TYPE_Black_Star, UJsonUtils::StringToJson("{\"color\": {\"R\": 0, \"G\": 0, \"B\": 0, \"A\": 1}}"));
}

void UStarisGameInstance::Init()
{
	Super::Init();

	StarTypeDatabase = NewObject<UCompositeDatabase>();
	StarTypeDatabase->RecordComponentType = UStarTypeProperties::StaticClass();

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
