// Fill out your copyright notice in the Description page of Project Settings.


#include "Empire/UnitedEmpireGenerator.h"

#include "StarisStatics.h"
#include "Empire/Colony.h"
#include "Empire/Empire.h"
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

			TArray<ASystem*> FreeHabitableSystems;
			for (const auto& System : Galaxy->GetSystems())
			{
				if (System->GetOwningEmpire() == nullptr && System->GetPlanets().Num() > 0)
				{
					FreeHabitableSystems.Add(System);
				}
			}

			if (FreeHabitableSystems.IsEmpty())
			{
				UE_LOG(LogStaris, Error, TEXT("Failed to populate empire \"%s\" due to the lack of unowned systems"), *EmpireTitle);
				return nullptr;
			}
		
			auto Empire = NewObject<UEmpire>();
			Empire->Title = EmpireTitle;
			Empire->FounderRaces = EmpireRaces;
			
			auto System = FreeHabitableSystems[Random.RandRange(0, FreeHabitableSystems.Num() - 1)];
			System->Title = SystemTitle;
			Empire->TakeSystem(System);

			if (System->GetStars().Num() == 1)
			{
				System->GetStars()[0]->Title = SystemTitle;
			}
			else
			{
				for (int j = 0; j < System->GetStars().Num(); j++)
				{
					System->GetStars()[j]->Title = SystemTitle + "-" + LetterNames[j];
				}
			}

			Empire->Capital = GetRandomArrayItem(System->GetPlanets());
			Empire->Capital->Title = CapitalTitle;
			Empire->Capital->Colony = NewObject<UColony>();
			for (int j = 0; j < EmpireRaces.Num(); j++)
			{
				Empire->Capital->Colony->Population.Add(EmpireRaces[j], TotalPops / EmpireRaces.Num() * Random.FRandRange(0.9, 1.1));
			}

			return Empire;
		}
	}

	return nullptr;
}
