// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StarisGraphicsSettings.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FFloatDelegate, float /* New Value */);

UCLASS()
class STARIS_API AStarisGraphicsSettings : public AActor
{
	GENERATED_BODY()

public:
	float GetPlanetRenderDistanceScale() const { return PlanetRenderDistanceScale; }

	UFUNCTION(BlueprintSetter)
	void SetPlanetRenderDistanceScale(float Value);

	FFloatDelegate PlanetRenderDistanceScaleChanged;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintSetter=SetPlanetRenderDistanceScale, meta=(AllowPrivateAccess))
	float PlanetRenderDistanceScale = 1.0f;
};
