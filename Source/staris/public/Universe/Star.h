// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CelestialEntity.h"
#include "Focusable.h"
#include "Components/SceneComponent.h"
#include "GalaxyMetaData.h"

#include "Star.generated.h"

class UMeshInstanceRef;
class USystem;
class UToolTip;

UCLASS(BlueprintType)
class STARIS_API UStar : public UObject, public ICelestialEntity, public IFocusable
{
	GENERATED_BODY()

	friend USystem;

public:
	UStar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyPattern(const FStarMetaData& Data);

	virtual void SetupToolTip(UToolTip* ToolTip) override;

	virtual TArray<UContextMenuItem*> CreateContextActionsHovered(IFocusable* Selected) override;

	UFUNCTION(BlueprintPure)
	FText GetTitle() const;

	const FName& GetId() const { return Id; }
	int32 GetSeed() const { return Seed; }
	const FVector& GetLocation() const { return Location; }
	const UCompositeRecord* GetType() const { return TypeRecord; }

	UFUNCTION(BlueprintPure)
	USystem* GetSystem() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Id;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	int32 Seed;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UMeshInstanceRef* MeshInstance;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FVector Location;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeRecord* TypeRecord;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UToolTip> ToolTipClass;
};
