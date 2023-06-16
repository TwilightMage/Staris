// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisPlayerPawn.h"

#include "Camera/CameraComponent.h"


AStarisPlayerPawn::AStarisPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(RootComponent);

	CameraArm = CreateDefaultSubobject<USceneComponent>("Camera Arm");
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->SetRelativeRotation(FRotator(-45, 0, 0));
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(CameraArm);
	CameraComponent->SetRelativeLocation(FVector(-1000, 0, 0));
}

void AStarisPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	RootComponent->SetWorldLocation(RootComponent->GetComponentLocation() * FVector(1, 1, 0));
	DesiredLocation = RootComponent->GetComponentLocation();

	DesiredYaw = RootComponent->GetComponentRotation().Yaw;
	DesiredPitch = CameraArm->GetComponentRotation().Pitch;

	DesiredZoom = -CameraComponent->GetComponentLocation().X;
}

void AStarisPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float MovementScale = FMath::Lerp(1.0f, 4.0f, (DesiredZoom - MinCamDist) / (MaxCamDist - MinCamDist));
	DesiredLocation += (RootComponent->GetForwardVector() * Movement.X * MovementScale + RootComponent->GetRightVector() * Movement.Y * MovementScale) * DeltaTime * 2000;
	DesiredLocation.Z = 0;
	RootComponent->SetWorldLocation(FMath::Lerp(RootComponent->GetComponentLocation(), DesiredLocation, DeltaTime * 10));

	auto NewRotation = FRotator(FQuat::Slerp(FRotator(CameraArm->GetRelativeRotation().Pitch, RootComponent->GetComponentRotation().Yaw, 0).Quaternion(), FRotator(DesiredPitch, DesiredYaw, 0).Quaternion(), DeltaTime * 20));
	RootComponent->SetRelativeRotation(FRotator(0, NewRotation.Yaw, 0));
	CameraArm->SetRelativeRotation(FRotator(NewRotation.Pitch, 0, 0));

	CameraComponent->SetRelativeLocation(FVector(FMath::Lerp(CameraComponent->GetRelativeLocation().X, -DesiredZoom, DeltaTime * 10), 0, 0));
}

void AStarisPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("RotateCamera", IE_Pressed, this, &AStarisPlayerPawn::RotateCamera_Pressed);
	PlayerInputComponent->BindAction("RotateCamera", IE_Released, this, &AStarisPlayerPawn::RotateCamera_Released);
	
	PlayerInputComponent->BindAxis("MovementForward", this, &AStarisPlayerPawn::MovementForward);
	PlayerInputComponent->BindAxis("MovementRight", this, &AStarisPlayerPawn::MovementRight);

	PlayerInputComponent->BindVectorAxis("Mouse2D", this, &AStarisPlayerPawn::Mouse2D);
	PlayerInputComponent->BindAxis("CameraZoom", this, &AStarisPlayerPawn::CameraZoom);
}

void AStarisPlayerPawn::MoveToTarget(USceneComponent* Target, bool Track)
{
	MoveToLocation(Target->GetComponentLocation());
}

void AStarisPlayerPawn::MoveToLocation(const FVector& Location)
{
	DesiredLocation = FVector(Location.X, Location.Y, 0);

	if (!HasActorBegunPlay())
	{
		RootComponent->SetWorldLocation(DesiredLocation);
	}
}

void AStarisPlayerPawn::RotateCamera_Pressed()
{
	bIsRotatingCamera = true;
}

void AStarisPlayerPawn::RotateCamera_Released()
{
	bIsRotatingCamera = false;
}

void AStarisPlayerPawn::MovementForward(float Axis)
{
	Movement.X = Axis;
}

void AStarisPlayerPawn::MovementRight(float Axis)
{
	Movement.Y = Axis;
}

void AStarisPlayerPawn::Mouse2D(FVector Vector)
{
	if (bIsRotatingCamera)
	{
		DesiredPitch = FMath::Clamp(DesiredPitch + Vector.Y * 3, -89, 89);
		DesiredYaw += Vector.X * 3;
	}
}

void AStarisPlayerPawn::CameraZoom(float Axis)
{
	DesiredZoom = FMath::Clamp(DesiredZoom - Axis * 300 * FMath::Lerp(1, 2, (DesiredZoom - MinCamDist) / (MaxCamDist - MinCamDist)), MinCamDist, MaxCamDist);
}

