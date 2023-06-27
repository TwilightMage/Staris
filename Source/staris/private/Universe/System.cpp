// Fill out your copyright notice in the Description page of Project Settings.

#include "Universe/System.h"

#include "StarisStatics.h"
#include "Game/StarisGameInstance.h"
#include "Universe/Galaxy.h"
#include "Universe/LetterNames.h"
#include "Universe/Planet.h"
#include "Universe/Star.h"

USystem::USystem()
{
	
}

UEmpire* USystem::GetOwningEmpire()
{
	return OwningEmpire.Get();
}

FText USystem::GetTitle() const
{
	return Title.IsEmpty() ? FText::FromName(Id) : FText::FromString(UStarisGameInstance::DebugToolsEnabled ? Title + " [" + Id.ToString() + "]" : Title);
}

void USystem::RenameSystem(const FString& NewName, bool PropagateToChildren)
{
	Title = NewName;

	if (PropagateToChildren)
	{
		if (Stars.Num() > 1)
		{
			for (int32 i = 0; i < Stars.Num(); i++)
			{
				Stars[i]->Title = NewName + "-" + GetLetterName(i);
			}
		}
		else
		{
			Stars[0]->Title = NewName;
		}
	}
}

void USystem::ApplyPattern_Implementation(const FSystemMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to star system %s"), *Id.ToString())
		return;
	}

	Title = Data.Title;
	Id = Data.Id;
	Seed = Data.Seed;
	Location = Data.Location;
	
	if (auto Galaxy = GetGalaxy())
	{
		Galaxy->RegisterObjectById(Id, this);
		
		for (const auto& Star : Data.Stars)
		{
			const auto StarEntity = NewObject<UStar>(this, Galaxy->StarClass);
			StarEntity->InitCelestialEntity(Galaxy);
			StarEntity->ApplyPattern(Star);
			
			Stars.Add(StarEntity);
		}

		for (const auto& Planet : Data.Planets)
		{
			const auto PlanetEntity = NewObject<UPlanet>(this, Galaxy->PlanetClass);
			PlanetEntity->InitCelestialEntity(Galaxy);
			PlanetEntity->ApplyPattern(Planet);
			
			Planets.Add(PlanetEntity);
		}
	}
}
