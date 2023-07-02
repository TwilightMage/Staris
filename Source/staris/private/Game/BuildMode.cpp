// Copyright DragonCap. All rights reserved.


#include "Game/BuildMode.h"

#include "Empire/VanillaGalaxyBuildingTypeProperties.h"
#include "Engine/StaticMeshActor.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/Galaxy.h"

void UBuildMode::GlobalPlaneClicked(const FVector& Location)
{
}

void UBuildMode::GlobalPlaneTick(const FVector& Location)
{
	if (BuildingMarker)
	{
		BuildingMarker->SetActorLocation(Location);
	}
}

void UBuildMode::SetActiveBuildingType(UCompositeRecord* NewGalaxyBuildingType)
{
	GalaxyBuildingType = NewGalaxyBuildingType;

	if (!BuildingMarker)
	{
		if (auto World = GetWorld())
		{
			BuildingMarker = World->SpawnActor<AStaticMeshActor>();

			if (auto Galaxy = GetActorOfClass<AGalaxy>(this))
			{
				BuildingMarker->GetStaticMeshComponent()->SetMaterial(0, Galaxy->BuildingPreviewMaterial);
			}
		}
	}

	if (BuildingMarker)
	{
		BuildingMarker->GetStaticMeshComponent()->SetStaticMesh(NewGalaxyBuildingType->GetOrCreateComponent<UVanillaGalaxyBuildingTypeProperties>()->Mesh);
	}
}
