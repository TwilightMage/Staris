// Fill out your copyright notice in the Description page of Project Settings.


#include "Fleet/Ship.h"

#include "StarisStatics.h"
#include "Game/StarisHUD.h"
#include "Kismet/KismetMathLibrary.h"
#include "UI/ToolTip.h"

UShip::UShip()
{
	
}

void UShip::SetupToolTip(UToolTip* ToolTip)
{
	ToolTip->AddLine(FText::Format(FTextFormat(NSLOCTEXT("Ship", "ShipToolTip_Class", "Vessel Class: {0}")), GetClassTitle()));

	GetFleet()->SetupToolTip(ToolTip);
}

TArray<UContextMenuItem*> UShip::CreateContextActionsSelected(IFocusable* Hovered)
{
	return GetFleet()->CreateContextActionsSelected(Hovered);
}

void UShip::OnSelected()
{
	if (auto HUD = GetActorOfClass<AStarisHUD>(this))
	{
		HUD->OpenFocusablePanel(this);
	}
}

ILabeled* UShip::GetLabeled() const
{
	return GetFleet();
}

float UShip::GetSpeed() const
{
	return 10;
}

void UShip::Tick(float TickDelta, const FTransform& FleetTransform, FTransform& NewShipTransform)
{
	FVector NewLocation = FleetTransform.TransformPosition(PositionInFormation);
	Transform.SetRotation(UKismetMathLibrary::Quat_Slerp(Transform.GetRotation(), UKismetMathLibrary::FindLookAtRotation(Transform.GetLocation(), NewLocation).Quaternion(), TickDelta * 10));
	Transform.SetLocation(FMath::Lerp(Transform.GetLocation(), NewLocation, TickDelta * 10));

	NewShipTransform = Transform;
}

void UShip::Setup_Implementation(UEmpire* Empire)
{
	
}
