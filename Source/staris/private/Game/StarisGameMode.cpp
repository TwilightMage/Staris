// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisGameMode.h"

#include "Empire/Empire.h"
#include "Empire/Race.h"
#include "Empire/UnitedEmpireGenerator.h"
#include "Game/StarisPlayerController.h"
#include "Game/StarisPlayerPawn.h"
#include "Universe/Galaxy.h"

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
			if (Empire->TryAssignPlayer(Player))
			{
				break;
			}
		}
	}
	
	return Super::InitNewPlayer(NewPlayerController, UniqueId, Options, Portal);
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
		Galaxy->GameStarted.AddUObject(this, &AStarisGameMode::GameStarted);

		auto EmpireGenerator = NewObject<UUnitedEmpireGenerator>();
		EmpireGenerator->EmpireTitle = "Great Foxik Empire";
		EmpireGenerator->SystemTitle = "Solus";
		EmpireGenerator->CapitalTitle = "Terra";
		auto EmpireRace = NewObject<URace>();
		EmpireRace->Title = "Foxus";
		EmpireGenerator->EmpireRaces = { EmpireRace };
		EmpireGenerator->TotalPops = 8000000;
		Empires.Add(EmpireGenerator->Generate(Galaxy));
	}
	
	SetupDone = true;
}
