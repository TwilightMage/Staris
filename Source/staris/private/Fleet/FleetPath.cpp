// Copyright DragonCap. All rights reserved.


#include "Fleet/FleetPath.h"

#include "StarisStatics.h"
#include "Fleet/Fleet.h"
#include "Kismet/KismetMathLibrary.h"
#include "Universe/Galaxy.h"
#include "Universe/GalaxySettingsManager.h"
#include "Universe/System.h"
#include "Universe/VanillaGalaxySettings.h"

bool UFleetPathLocationNode::MoveTick(float& TickAmount, AFleet* Fleet, float& Distance)
{
	const float FleetSpeed = Fleet->GetSpeed();

	const auto FleetTransform = Fleet->GetActorTransform();
	const auto MoveVector = Location - FleetTransform.GetLocation();

	UKismetSystemLibrary::DrawDebugLine(Fleet, FleetTransform.GetLocation(), Location, FColor::Cyan);
	
	const float MoveVectorLength = MoveVector.Length();

	if (MoveVectorLength <= KINDA_SMALL_NUMBER)
	{
		return true;
	}
	
	const float MoveDeltaLength = FMath::Min(TickAmount * FleetSpeed, MoveVectorLength);
	const FVector MoveDelta = MoveVector.GetUnsafeNormal() * MoveDeltaLength;
	TickAmount -= MoveDeltaLength;

	Distance += MoveDeltaLength;
	Fleet->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(FleetTransform.GetLocation(), Location));
	Fleet->AddActorWorldOffset(MoveDelta);

	return false;
}

UFleetPath* UFleetPath::BuildPath(const FFleetPathConfig& Config, const UObject* WorldContextObject, UObject* Outer)
{
	if (Config.bCanEnterDeepSpace)
	{
		const auto Path = NewObject<UFleetPath>(Outer);

		const auto StartNode = NewObject<UFleetPathLocationNode>(Outer);
		StartNode->Location = Config.Start;

		Path->AddNode(StartNode);
		
		if (FVector::Distance(StartNode->Location, Config.End) <= Config.EndOffset)
		{
			return Path;
		}

		const auto EndNode = NewObject<UFleetPathLocationNode>(Outer);
		EndNode->Location = Config.End;

		if (Config.EndOffset > 0)
		{
			EndNode->Location += (StartNode->Location - EndNode->Location).GetUnsafeNormal() * Config.EndOffset;
		}

		Path->AddNode(EndNode);
			
		return Path;
	}

	const auto Galaxy = GetActorOfClass<AGalaxy>(WorldContextObject);
	const auto GalaxySettings = GetActorOfClass<AGalaxySettingsManager>(WorldContextObject);
	const auto VanillaGalaxySettings = GalaxySettings->GetSettings<UVanillaGalaxySettings>();

	auto EndSystem = Config.EndHint;
	UE_LOG(LogTemp, Log, TEXT("Position Range %f / %f"), FVector::Distance(EndSystem->GetLocation(), Config.End), VanillaGalaxySettings->PlanetOrbitRange.Max * 1.2);
	if (!EndSystem || FVector::Distance(EndSystem->GetLocation(), Config.End) > VanillaGalaxySettings->PlanetOrbitRange.Max * 1.2)
	{
		EndSystem = nullptr;
		for (const auto System : Galaxy->GetSystems())
		{
			if (FVector::Distance(System->GetLocation(), Config.End) <= VanillaGalaxySettings->PlanetOrbitRange.Max * 1.2)
			{
				EndSystem = System;
			}
		}
	}

	if (EndSystem == nullptr && !Config.bCanEnterDeepSpace) return nullptr;

	auto StartSystem = Config.StartHint;
	if (!StartSystem || FVector::Distance(StartSystem->GetLocation(), Config.Start) > VanillaGalaxySettings->PlanetOrbitRange.Max * 1.2)
	{
		StartSystem = nullptr;
		for (const auto System : Galaxy->GetSystems())
		{
			if (FVector::Distance(System->GetLocation(), Config.Start) <= VanillaGalaxySettings->PlanetOrbitRange.Max * 1.2)
			{
				StartSystem = System;
			}
		}
	}

	// For now we don't build inter-system paths
	if (StartSystem != EndSystem || !StartSystem || !EndSystem) return nullptr;

	const auto Path = NewObject<UFleetPath>(Outer);

	const auto StartNode = NewObject<UFleetPathLocationNode>(Outer);
	StartNode->Location = Config.Start;

	Path->AddNode(StartNode);
		
	if (FVector::Distance(StartNode->Location, Config.End) <= Config.EndOffset)
	{
		return Path;
	}

	const auto EndNode = NewObject<UFleetPathLocationNode>(Outer);
	EndNode->Location = Config.End;

	if (Config.EndOffset > 0)
	{
		EndNode->Location += (StartNode->Location - EndNode->Location).GetUnsafeNormal() * Config.EndOffset;
	}

	Path->AddNode(EndNode);
			
	return Path;
}

UFleetPathNode* UFleetPath::GetFirstNode() const
{
	return Nodes[0];
}

void UFleetPath::AddNode(UFleetPathNode* Node)
{
	if (!Nodes.IsEmpty())
	{
		Nodes.Last()->NextNode = Node;
	}
	
	Nodes.Add(Node);
}
