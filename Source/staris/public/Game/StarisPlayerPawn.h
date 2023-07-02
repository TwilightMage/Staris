// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "StarisPlayerPawn.generated.h"

class APointLight;
class USceneLabel;
class UTitleLabel;
class USphereComponent;
class AStarisGameSettings;
class AGalaxy;
class USpringArmComponent;
class UCameraComponent;
class ILabeled;
class UInputComponent;

USTRUCT()
struct FSystemInProximity
{
	GENERATED_BODY()

	UPROPERTY()
	int32 StarsInProximity;

	UPROPERTY()
	TArray<APointLight*> StarLights;
};

UCLASS()
class STARIS_API AStarisPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	AStarisPlayerPawn();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void MoveToTarget(USceneComponent* Target, bool Track);

	UFUNCTION(BlueprintCallable)
	void MoveToLocation(const FVector& Location);

	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm=LabelExtensionClass))
	void SetActiveLabelExtension(const TSubclassOf<ULabelExtension>& LabelExtensionClass);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USceneComponent* CameraArm;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* ProximitySphere;

private:
	UFUNCTION()
	void ProximitySphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ProximitySphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTitleLabel> TitleLabelClass;

	TMap<ILabeled*, USceneLabel*> Labels;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(AllowPrivateAccess))
	TSubclassOf<ULabelExtension> ActiveLabelExtension;

	UPROPERTY()
	TMap<USystem*, FSystemInProximity> SystemsInProximity;
};
