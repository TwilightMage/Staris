﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CelestialEntity.h"
#include "Focusable.h"
#include "Components/SceneComponent.h"
#include "GalaxyMetaData.h"

#include "Star.generated.h"

class ASystem;
class UStarToolTip;

UCLASS()
class STARIS_API UStar : public UStaticMeshComponent, public ICelestialEntity, public IFocusable
{
	GENERATED_BODY()

	friend ASystem;

public:
	UStar();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ApplyPattern(const FStarMetaData& Data);

	virtual UToolTip* CreateToolTip() override;
	virtual void SetupToolTip(UToolTip* ToolTip) override;

	FText GetTitle() const { return Title.IsEmpty() ? FText::FromName(Id) : FText::FromString(Title); }

	const FName& GetId() const { return Id; }
	UCompositeRecord* const& GetType() const { return TypeRecord; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

private:
	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	FName Id;

	UPROPERTY(BlueprintReadOnly, meta=(AllowPrivateAccess))
	UCompositeRecord* TypeRecord;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UStarToolTip> ToolTipClass;

	TWeakObjectPtr<ASystem> System;
};
