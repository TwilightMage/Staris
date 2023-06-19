// Fill out your copyright notice in the Description page of Project Settings.


#include "Fleet/Ship.h"

UShip::UShip()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	StaticMesh->SetupAttachment(this);
}

void UShip::Setup_Implementation(UEmpire* Empire)
{
	
}
