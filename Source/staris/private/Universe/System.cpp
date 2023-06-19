// Fill out your copyright notice in the Description page of Project Settings.

#include "Universe/System.h"

#include "StarisStatics.h"
#include "Universe/Galaxy.h"
#include "Universe/LetterNames.h"
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

void ASystem::RenameSystem(const FString& NewName, bool PropagateToChildren)
{
	Title = NewName;

	if (PropagateToChildren)
	{
		for (int32 i = 0; i < Stars.Num(); i++)
		{
			Stars[i]->Title = NewName + "-" + GetLetterName(i);
		}
	}
}

void ASystem::ApplyPattern_Implementation(const FSystemMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to star system %s"), *Id.ToString())
		return;
	}

	Title = Data.Title;
	Id = Data.Id;
#if WITH_EDITOR
	SetActorLabel(FString::Printf(TEXT("System_%s"), *Id.ToString()));
#endif
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

			StarEntity->System = this;
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
