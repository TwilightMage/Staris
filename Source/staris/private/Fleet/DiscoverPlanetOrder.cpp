// Copyright DragonCap. All rights reserved.


#include "Fleet/DiscoverPlanetOrder.h"

#include "Empire/EmpirePlanetKnowledge.h"
#include "Fleet/Fleet.h"
#include "Fleet/FleetPath.h"
#include "Universe/Planet.h"

FText UDiscoverPlanetOrder::GetTitle() const
{
	return FText::Format(NSLOCTEXT("DiscoverPlanetOrder", "Discover planet {0}", "Discover planet {0}"), TargetPlanet->GetTitle());
}

float UDiscoverPlanetOrder::ExecutionDistance() const
{
	check(TargetPlanet);

	return TargetPlanet->GetRealRadius() * 2.f;
}

UFleetPath* UDiscoverPlanetOrder::BuildPathToTarget(const FVector& Start, USystem* StartHint)
{
	FFleetPathConfig Config;
	Config.Start = Start;
	Config.StartHint = StartHint;
	Config.End = TargetPlanet->GetLocation();
	Config.EndHint = TargetPlanet->GetSystem();
	Config.EndOffset = ExecutionDistance();

	Config.bCanEnterDeepSpace = false;
	
	return Path = UFleetPath::BuildPath(Config, this, GetTransientPackage());
}

bool UDiscoverPlanetOrder::ExecutionTick(float& TickAmount, AFleet* Fleet)
{
	const float TotalDiscoverAmount = TargetPlanet->GetArea() / 10000000;

	const float DiscoverDelta = FMath::Min(TickAmount, TotalDiscoverAmount - DiscoverAmount);
	DiscoverAmount += DiscoverDelta * Fleet->GetShips().Num();
	TickAmount -= DiscoverDelta;

	OnOrderProgress.Broadcast(DiscoverAmount / TotalDiscoverAmount);
	OnOrderProgress_K2.Broadcast(DiscoverAmount / TotalDiscoverAmount);

	if (DiscoverAmount >= TotalDiscoverAmount)
	{
		if (auto Empire = Fleet->GetEmpire())
		{
			auto Knowledge = Empire->GetOrCreatePlanetKnowledge(TargetPlanet);

			Knowledge->UnlockLayer(0);

			OnOrderFinished.Broadcast();
			OnOrderFinished_K2.Broadcast();
		}
		
		return true;
	}

	return false;
}
