// Fill out your copyright notice in the Description page of Project Settings.

#include "Universe/System.h"

#include "StarisStatics.h"
#include "Universe/Galaxy.h"
#include "Universe/Planet.h"
#include "Universe/Star.h"

ASystem::ASystem()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
}

UEmpire* ASystem::GetOwningEmpire()
{
	return OwningEmpire.Get();
}

void ASystem::ApplyPattern_Implementation(const FSystemMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to star system %s"), *Id.ToString())
		return;
	}

	Id = Data.Id;
	SetActorLabel(FString::Printf(TEXT("System_%s"), *Id.ToString()));

	SetActorRelativeLocation(Data.Location);
	
	if (auto Galaxy = GetGalaxy())
	{
		for (const auto& Star : Data.Stars)
		{
			const auto StarEntity = NewObject<UStar>(this, Galaxy->StarClass);
			StarEntity->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			StarEntity->RegisterComponent();
			StarEntity->InitCelestialEntity(Galaxy);
			StarEntity->ApplyPattern(Star);

			Stars.Add(StarEntity);
		}

		for (const auto& Planet : Data.Planets)
		{
			const auto PlanetEntity = NewObject<UPlanet>(this, Galaxy->PlanetClass);
			PlanetEntity->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			PlanetEntity->RegisterComponent();
			PlanetEntity->InitCelestialEntity(Galaxy);
			PlanetEntity->ApplyPattern(Planet);

			PlanetEntity->System = this;
			Planets.Add(PlanetEntity);
		}
	}
}
