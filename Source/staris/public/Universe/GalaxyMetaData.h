// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "JsonObjectWrapper.h"

#include "GalaxyMetaData.generated.h"

class UCompositeRecord;

USTRUCT(BlueprintType)
struct FStarMetaData
{
	GENERATED_BODY()

	inline const static FName TYPE_Red = "red";
	inline const static FName TYPE_Yellow = "yellow";
	inline const static FName TYPE_Blue = "blue";
	inline const static FName TYPE_Black_Hole = "black_hole";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Seed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Title;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UCompositeRecord* Type = nullptr;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale;

	inline static TMap<FName, TSharedPtr<FJsonObject>> Collection = {};
};

USTRUCT(BlueprintType)
struct FPlanetLayer
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UCompositeRecord*> ResourceTiles;
};

USTRUCT(BlueprintType)
struct FPlanetMetaData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Seed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Temperature;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OrbitOffset;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector OrbitPoint;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OrbitDistance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float OrbitSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPlanetLayer> Layers;
};

USTRUCT(BlueprintType)
struct FSystemMetaData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Seed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString Title;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FStarMetaData> Stars;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPlanetMetaData> Planets;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;
};

USTRUCT(BlueprintType)
struct FGalaxyMetaData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Seed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FSystemMetaData> Systems;
};