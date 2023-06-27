// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "StarisPlayerPawn.generated.h"

class AStarisGameSettings;
class AGalaxy;
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

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(USceneComponent* Target, bool Track);

	UFUNCTION(BlueprintCallable)
	void MoveToLocation(const FVector& Location);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* CameraArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

private:
	void RotateCamera_Pressed();
	void RotateCamera_Released();

	void Click();
	void OpenContextMenu();

	void PauseGameTime();
	
	void MovementForward(float Axis);
	void MovementRight(float Axis);

	void Mouse2D(FVector Vector);
	void CameraZoom(float Axis);

	FVector Movement;
	bool bIsRotatingCamera;

	FVector DesiredLocation;
	float DesiredPitch;
	float DesiredYaw;
	float DesiredCamDist;

	float StarScale = 1.0f;
	float StarScaleCached = 1.0f;

	UPROPERTY()
	AGalaxy* Galaxy;

	UPROPERTY()
	AStarisGameSettings* GameSettings;
};
