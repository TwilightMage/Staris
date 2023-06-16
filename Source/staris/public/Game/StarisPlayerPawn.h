// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "StarisPlayerPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class STARIS_API AStarisPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AStarisPlayerPawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveToTarget(USceneComponent* Target, bool Track);
	void MoveToLocation(const FVector& Location);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* CameraArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

private:
	void RotateCamera_Pressed();
	void RotateCamera_Released();
	
	void MovementForward(float Axis);
	void MovementRight(float Axis);

	void Mouse2D(FVector Vector);
	void CameraZoom(float Axis);

	FVector Movement;
	bool bIsRotatingCamera;

	FVector DesiredLocation;
	float DesiredPitch;
	float DesiredYaw;
	float DesiredZoom;

	float MinCamDist = 1000;
	float MaxCamDist = 10000;
};
