// Copyright DragonCap. All rights reserved.


#include "Game/StarisGraphicsSettings.h"

void AStarisGraphicsSettings::SetPlanetRenderDistanceScale(float Value)
{
	PlanetRenderDistanceScale = Value;
	
	PlanetRenderDistanceScaleChanged.Broadcast(Value);
}
