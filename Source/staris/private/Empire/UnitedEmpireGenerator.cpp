// Fill out your copyright notice in the Description page of Project Settings.


#include "Empire/UnitedEmpireGenerator.h"

#include "StarisStatics.h"
#include "Empire/Colony.h"
#include "Empire/Empire.h"
#include "Fleet/BuildingShip.h"
#include "Fleet/Fleet.h"
#include "Fleet/ScienceShip.h"
#include "Universe/Galaxy.h"
#include "Universe/GalaxySettingsManager.h"
#include "Universe/Planet.h"
#include "Universe/Star.h"
#include "Universe/System.h"
#include "Universe/VanillaGalaxySettings.h"

TArray<FString> LetterNames = {
	"Alpha",
	"Beta",
	"Gamma",
	"Delta",
	"Epsilon",
	"Zeta",
	"Eta",
	"Theta",
	"Iota",
	"Kappa",
	"Lambda",
	"Mu",
	"Nu",
	"Xi",
	"Omicron",
	"Pi",
	"Rho",
	"Sigma",
	"Tau",
	"Upsilon",
	"Phi",
	"Chi",
	"Psi",
	"Omega"
};

UEmpire* UUnitedEmpireGenerator::Generate_Implementation(AGalaxy* Galaxy)
{
	if (auto GalaxySettingsManager = GetActorOfClass<AGalaxySettingsManager>(Galaxy))
	{
		if (auto VanillaGalaxySettings = GalaxySettingsManager->GetSettings<UVanillaGalaxySettings>())
		{
			FRandomStream Random(VanillaGalaxySettings->Seed);

			// Collect all available valid systems
			TArray<ASystem*> FreeHabitableSystems;
			for (const auto& System : Galaxy->GetSystems())
			{
				if (System->GetOwningEmpire() == nullptr && System->GetPlanets().Num() > 0)
				{
					FreeHabitableSystems.Add(System);
				}
			}

			// Check if there systems to take
			if (FreeHabitableSystems.IsEmpty())
			{
				UE_LOG(LogStaris, Error, TEXT("Failed to populate empire \"%s\" due to the lack of available systems"), *EmpireTitle);
				return nullptr;
			}

			// Setup Empire
			const auto Empire = NewObject<UEmpire>();
			Empire->Title = EmpireTitle;
			Empire->FounderRaces = EmpireRaces;

			// Configure System
			const auto System = FreeHabitableSystems[Random.RandRange(0, FreeHabitableSystems.Num() - 1)];
			System->RenameSystem(SystemTitle, true);
			Empire->TakeSystem(System);

			// Configure Capital
			Empire->Capital = GetRandomArrayItem(System->GetPlanets());
			Empire->Capital->Title = CapitalTitle;
			Empire->Capital->Colony = NewObject<UColony>();
			for (int j = 0; j < EmpireRaces.Num(); j++)
			{
				Empire->Capital->Colony->Population.Add(EmpireRaces[j], TotalPops / EmpireRaces.Num() * Random.FRandRange(0.9, 1.1));
			}

			// Configure Building Fleet
			AFleet* BuildingFleet = Galaxy->GetWorld()->SpawnActor<AFleet>(Empire->Capital->GetComponentLocation() + FVector(200, 200, 0), FRotator::ZeroRotator);
			for (int32 i = 0; i < 3; i++)
			{
				auto Ship = NewObject<UBuildingShip>(Empire, Galaxy->BuildingShipClass);
				Ship->Setup(Empire);
				BuildingFleet->AssignShip(Ship, true);
			}

			// Configure Science Fleet
			AFleet* ScienceFleet = Galaxy->GetWorld()->SpawnActor<AFleet>(Empire->Capital->GetComponentLocation() + FVector(-200, 200, 0), FRotator::ZeroRotator);
			for (int32 i = 0; i < 3; i++)
			{
				const auto Ship = NewObject<UScienceShip>(Empire, Galaxy->ScienceShipClass);
				Ship->Setup(Empire);
				ScienceFleet->AssignShip(Ship, true);
			}
			
			return Empire;
		}
	}

	return nullptr;
}
