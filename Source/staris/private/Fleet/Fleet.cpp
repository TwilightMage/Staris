// Fill out your copyright notice in the Description page of Project Settings.


#include "Fleet/Fleet.h"

#include "Fleet/Ship.h"


AFleet::AFleet()
{
	
}

bool AFleet::AssignShip(UShip* Ship, bool Force)
{
	if (GetCurrentUsedControl() + Ship->ControlRequired <= ControlLimit || Force)
	{
		Ships.Add(Ship);
		return true;
	}

	return false;
}

int32 AFleet::GetCurrentUsedControl()
{
	int32 Result = 0;
	
	for (int32 i = Ships.Num(); i >= 0; --i)
	{
		Result += Ships[i]->ControlRequired;
	}

	return Result;
}


