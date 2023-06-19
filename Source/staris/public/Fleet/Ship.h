// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Ship.generated.h"

class UCompositeRecord;
class AFleet;

UCLASS(Abstract, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARIS_API UShip : public USceneComponent
{
	GENERATED_BODY()

	friend AFleet;

public:
	UShip();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Setup(UEmpire* Empire);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 ControlRequired;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMesh;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	UCompositeRecord* Type;
	
	TWeakObjectPtr<AFleet> OwningFleet;
};
