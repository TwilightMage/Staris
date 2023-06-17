// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/Planet.h"

#include "StarisStatics.h"
#include "Empire/Colony.h"
#include "UI/PlanetToolTip.h"
#include "Universe/System.h"

const int32 OrbitPositionsCount = 365 * 10;
FVector2D OrbitPositions[OrbitPositionsCount];
bool OrbitPositionsSet = false;

UPlanet::UPlanet()
{
	if (!OrbitPositionsSet)
	{
		for (int32 i = 0; i < OrbitPositionsCount; i++)
		{
			//const auto a = sizeof(OrbitPositions);
			OrbitPositions[i] = FVector2D(FMath::Cos(2 * 3.14 * (i / (float)OrbitPositionsCount)), FMath::Sin(2 * 3.14 * (i / (float)OrbitPositionsCount)));
		}

		OrbitPositionsSet = true;
	}
}

void UPlanet::OnDayPassed(int32 Day)
{
	//SetRelativeLocation(OrbitPoint + FVector(OrbitPositions[(int)(Day + OrbitOffset * (float)OrbitPositionsCount) % OrbitPositionsCount] * OrbitDistance, 0));
}

UToolTip* UPlanet::CreateToolTip()
{
	return NewObject<UPlanetToolTip>(this, ToolTipClass);
}

void UPlanet::SetupToolTip(UToolTip* ToolTip)
{
	Cast<UPlanetToolTip>(ToolTip)->SetupToolTip(
		GetTitle(),
		System->GetTitle(),
		Colony ? Colony->getTotalPopulationAmount() : 0,
		FText::FromName(Biome),
		TemperatureDay,
		TemperatureNight);
}

void UPlanet::OnInitCelestialEntity()
{
	//RegisterOnDayPassed();
}

void UPlanet::ApplyPattern_Implementation(const FPlanetMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to planet %s"), *Id.ToString())
		return;
	}

	Id = Data.Id;
	Biome = Data.Biome;
	TemperatureNight = Data.TemperatureMin;
	TemperatureDay = Data.TemperatureMax;
	Rename(*FString::Printf(TEXT("Planet_%s"), *Id.ToString()));
	
	SetRelativeScale3D(FVector(Data.Scale));
	SetRelativeLocation(Data.OrbitPoint + FVector(OrbitPositions[(int)(0 + Data.OrbitOffset * (float)OrbitPositionsCount)] * Data.OrbitDistance, 0));

	OrbitPoint = Data.OrbitPoint;
	OrbitOffset = Data.OrbitOffset;
	OrbitDistance = Data.OrbitDistance;
	OrbitSpeed = Data.OrbitSpeed;
}
