// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "JsonObjectWrapper.h"

#include "GalaxyMetaData.generated.h"

USTRUCT(BlueprintType)
struct FPlanetRegionMetadata
{
	GENERATED_BODY()

	inline const static FName Type_City = "city";
	inline const static FName Type_Agrarian = "agrarian";
	inline const static FName Type_Industrial = "industrial";
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<FName> AllowedTypes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName BuildedType;
};

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
	FName Type;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Scale;

	inline static TMap<FName, TSharedPtr<FJsonObject>> Collection = {};
};

USTRUCT(BlueprintType)
struct FPlanetMetaData
{
	GENERATED_BODY()

	inline const static FName BIOME_Desert = "desert";
	inline const static FName BIOME_Forest = "forest";
	inline const static FName BIOME_Arctic = "arctic";

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Id;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Biome;

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
	TArray<FPlanetRegionMetadata> Regions;
};

USTRUCT(BlueprintType)
struct FSystemMetaData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName Id;

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
	TArray<FSystemMetaData> Systems;
};