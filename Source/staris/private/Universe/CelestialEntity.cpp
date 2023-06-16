// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/CelestialEntity.h"

#include "Universe/Galaxy.h"

void ICelestialEntity::RegisterOnDayPassed()
{
	if (auto Galaxy = GetGalaxy())
	{
		if (!SignedToDayUpdate)
		{
			Galaxy->RemoveDayUpdateCelestialEntities.Remove(this);
			Galaxy->AddDayUpdateCelestialEntities.Add(this);
			SignedToDayUpdate = true;
		}
	}
}

void ICelestialEntity::UnRegisterFromDayPassed()
{
	if (auto Galaxy = GetGalaxy())
	{
		if (SignedToDayUpdate)
		{
			Galaxy->AddDayUpdateCelestialEntities.Remove(this);
			Galaxy->RemoveDayUpdateCelestialEntities.Add(this);
			SignedToDayUpdate = false;
		}
	}
}

void ICelestialEntity::RegisterOnMonthPassed()
{
	if (auto Galaxy = GetGalaxy())
	{
		if (!SignedToMonthUpdate)
		{
			Galaxy->RemoveMonthUpdateCelestialEntities.Remove(this);
			Galaxy->AddMonthUpdateCelestialEntities.Add(this);
			SignedToMonthUpdate = true;
		}
	}
}

void ICelestialEntity::UnRegisterFromMonthPassed()
{
	if (auto Galaxy = GetGalaxy())
	{
		if (SignedToMonthUpdate)
		{
			Galaxy->AddMonthUpdateCelestialEntities.Remove(this);
			Galaxy->RemoveMonthUpdateCelestialEntities.Add(this);
			SignedToMonthUpdate = false;
		}
	}
}

void ICelestialEntity::RegisterOnYearPassed()
{
	if (auto Galaxy = GetGalaxy())
	{
		if (!SignedToYearUpdate)
		{
			Galaxy->RemoveYearUpdateCelestialEntities.Remove(this);
			Galaxy->AddYearUpdateCelestialEntities.Add(this);
			SignedToYearUpdate = true;
		}
	}
}

void ICelestialEntity::UnRegisterFromYearPassed()
{
	if (auto Galaxy = GetGalaxy())
	{
		if (SignedToYearUpdate)
		{
			Galaxy->AddYearUpdateCelestialEntities.Remove(this);
			Galaxy->RemoveYearUpdateCelestialEntities.Add(this);
			SignedToYearUpdate = false;
		}
	}
}

AGalaxy* ICelestialEntity::GetGalaxy()
{
	return WeakGalaxy.Get();
}

void ICelestialEntity::InitCelestialEntity(AGalaxy* Galaxy)
{
	WeakGalaxy = Galaxy;
	
	if (Galaxy)
	{
		Galaxy->CelestialEntities.Add(this);
	}

	OnInitCelestialEntity();
}
