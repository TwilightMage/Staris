// Fill out your copyright notice in the Description page of Project Settings.


#include "Fleet/ScienceShip.h"

#include "Fleet/DiscoverPlanetOrder.h"

UScienceShip::UScienceShip()
	: Super()
{
	ControlRequired = 10;
}

FText UScienceShip::GetClassTitle() const
{
	return NSLOCTEXT("ScienceShip", "VesselClass", "Science Ship");
}

bool UScienceShip::CanExecuteOrder(UFleetOrder* Order) const
{
	return Order->IsA<UDiscoverPlanetOrder>();
}

bool UScienceShip::FullFleetShip() const
{
	return true;
}
