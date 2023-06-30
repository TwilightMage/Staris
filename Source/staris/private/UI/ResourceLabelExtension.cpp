// Copyright DragonCap. All rights reserved.


#include "UI/ResourceLabelExtension.h"

#include "Universe/Planet.h"

bool UResourceLabelExtension::TestTarget(ILabeled* Labeled)
{
	return Cast<UObject>(Labeled)->IsA<UPlanet>();
}
