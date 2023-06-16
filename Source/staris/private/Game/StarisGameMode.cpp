// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisGameMode.h"

#include "Empire.h"
#include "StarisStatics.h"
#include "StarisUtils.h"
#include "Game/StarisPlayerController.h"
#include "Game/StarisPlayerPawn.h"
#include "Universe/Galaxy.h"
#include "Universe/Star.h"
#include "Universe/System.h"

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

TArray<UEmpire*> AStarisGameMode::PopulateEmpires(int32 Num)
{
	if (Num == 0) return {};
	
	if (auto Galaxy = AGalaxy::GetGalaxy(this))
	{
		TArray<ASystem*> FreeHabitableSystems;
		for (const auto& System : Galaxy->GetSystems())
		{
			if (System->GetOwningEmpire() == nullptr && System->GetPlanets().Num() > 0)
			{
				FreeHabitableSystems.Add(System);
			}
		}

		if (Num > FreeHabitableSystems.Num())
		{
			UE_LOG(LogStaris, Error, TEXT("Failed to populate %i empires due to the lack of unowned systems. Only %i empires will be populated"), Num, FreeHabitableSystems.Num());
		}

		TArray<UEmpire*> Result;
		for (int32 i = 0; i < Num && i < FreeHabitableSystems.Num(); i++)
		{
			auto Empire = NewObject<UEmpire>();
			
			auto System = FreeHabitableSystems[FMath::RandRange(0, FreeHabitableSystems.Num() - 1)];
			Empire->TakeSystem(System);
			UStarisUtils::FillPlanetPopulation(GetRandomArrayItem(System->GetPlanets()), Empire);

			UE_LOG(LogStaris, Log, TEXT("Empire \"%s\" spawned on system %s"), *Empire->GetTitle().ToString(), *System->GetId().ToString());

			Empires.Add(Empire);
			Result.Add(Empire);
			FreeHabitableSystems.Remove(System);
		}

		return Result;
	}

	return {};
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
	}

	PopulateEmpires(1);
	
	SetupDone = true;
}
