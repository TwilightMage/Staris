// Fill out your copyright notice in the Description page of Project Settings.


#include "Empire/UnitedEmpireGenerator.h"

#include "StarisStatics.h"
#include "Empire/Colony.h"
#include "Empire/Empire.h"
#include "Empire/EmpirePlanetKnowledge.h"
#include "Fleet/BuildingShip.h"
#include "Fleet/Fleet.h"
#include "Fleet/ScienceShip.h"
#include "Game/StarisGameInstance.h"
#include "UI/SettingsPanel.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/Galaxy.h"
#include "Universe/GalaxySettingsManager.h"
#include "Universe/Planet.h"
#include "Universe/System.h"
#include "Universe/VanillaGalaxySettings.h"

UEmpire* UUnitedEmpireGenerator::Generate_Implementation(AGalaxy* Galaxy)
{
	if (auto StarisGameInstance = Cast<UStarisGameInstance>(UGameplayStatics::GetGameInstance(this)))
	{
		if (auto GalaxySettingsManager = GetActorOfClass<AGalaxySettingsManager>(Galaxy))
		{
			if (auto VanillaGalaxySettings = GalaxySettingsManager->GetSettings<UVanillaGalaxySettings>())
			{
				FRandomStream Random(VanillaGalaxySettings->Seed);

				// Collect all available valid systems
				TArray<USystem*> FreeHabitableSystems;
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
					UE_LOG(LogStaris, Error, TEXT("Failed to populate empire \"%s\": No available systems"), *EmpireTitle);
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

				// Pick Planet for capital
				Empire->Capital = System->GetPlanets()[0];
				for (int i = 1; i < System->GetPlanets().Num(); i++)
				{
					if (FMath::Abs(System->GetPlanets()[i]->GetTemperature()) < FMath::Abs(Empire->Capital->GetTemperature()))
					{
						Empire->Capital = System->GetPlanets()[i];
					}
				}
				
				// Configure Capital
				Empire->Capital->Title = CapitalTitle;
				Empire->Capital->SettleColony();
				for (int j = 0; j < EmpireRaces.Num(); j++)
				{
					Empire->Capital->GetColony()->Population.Add(EmpireRaces[j], TotalPops / EmpireRaces.Num() * Random.FRandRange(0.9, 1.1));
				}

				auto AnotherPlanets = System->GetPlanets();
				AnotherPlanets.Remove(Empire->Capital);

				auto PlanetWithCrystals = GetRandomArrayItem(AnotherPlanets, Random);
				PlanetWithCrystals->AddTileToLayer(0, StarisGameInstance->GetResourceTypeDatabase()->GetOrCreateRecord("crystals"));

				auto CapitalKnowledge = Empire->GetOrCreatePlanetKnowledge(Empire->Capital);
				CapitalKnowledge->UnlockLayer(0);

				// Configure Building Fleet
				AFleet* BuildingFleet = Galaxy->GetWorld()->SpawnActor<AFleet>(Galaxy->FleetClass, Empire->Capital->GetLocation() + FVector(20, 0, 0), FRotator::ZeroRotator);
				BuildingFleet->Setup(Empire);
				BuildingFleet->Title = "Construction Fleet";
				const auto BuildingShip = NewObject<UBuildingShip>(BuildingFleet, Galaxy->BuildingShipClass);
				BuildingShip->Setup(Empire);
				BuildingFleet->AssignShip(BuildingShip, true);

				// Configure Science Fleet
				AFleet* ScienceFleet = Galaxy->GetWorld()->SpawnActor<AFleet>(Galaxy->FleetClass, Empire->Capital->GetLocation() + FVector(-20, 0, 0), FRotator::ZeroRotator);
				ScienceFleet->Setup(Empire);
				ScienceFleet->Title = "Wings Of Discovery";
				const auto ScienceShip = NewObject<UScienceShip>(ScienceFleet, Galaxy->ScienceShipClass);
				ScienceShip->Setup(Empire);
				ScienceFleet->AssignShip(ScienceShip, true);
				
				return Empire;
			}
			else
			{
				UE_LOG(LogStaris, Error, TEXT("Failed to populate empire \"%s\": Vanilla galaxy settings not found"), *EmpireTitle);
			}
		}
		else
		{
			UE_LOG(LogStaris, Error, TEXT("Failed to populate empire \"%s\": Galaxy settings not found"), *EmpireTitle);
		}
	}
	else
	{
		UE_LOG(LogStaris, Error, TEXT("Failed to populate empire \"%s\": Game Instance not found"), *EmpireTitle);
	}

	return nullptr;
}

void UUnitedEmpireGenerator::FillSettingsPanel(USettingsPanel* SettingsPanel, const TSharedPtr<FJsonObject>& Json)
{
	SETTINGS_SECTION("UnitedEmpireGenerator", "United Empire Settings");
	SETTINGS_FIELD_STRING("UnitedEmpireGenerator", "Title", "Great Foxik Empire", EmpireTitle);
	SETTINGS_FIELD_STRING("UnitedEmpireGenerator", "System Title", "Solus", SystemTitle);
	SETTINGS_FIELD_STRING("UnitedEmpireGenerator", "Capital Title", "Terra", CapitalTitle);
	SETTINGS_FIELD_STRING("UnitedEmpireGenerator", "Founder Race", "Foxus", FounderRace);
	SETTINGS_FIELD_NUMBER("UnitedEmpireGenerator", "Desired Population", true, true, 1, false, 0, 8000000, "", int32, DesiredPopulation);
}
