// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StarisController.h"
#include "GameFramework/PlayerController.h"

#include "StarisPlayerController.generated.h"

class ASystem;
class USoundCue;
class UContextMenu;
class IFocusable;
class UToolTip;

UCLASS()
class STARIS_API AStarisPlayerController : public APlayerController, public IStarisController
{
	GENERATED_BODY()

public:
	AStarisPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnPossess(APawn* InPawn) override;

	virtual UEmpire* GetEmpire() const override;
	virtual void AssignEmpire(UEmpire* Empire) override;

	virtual bool IsPlayer() const override { return true; }
	virtual FString GetControllerName() const override { return GetName(); }

	void Click();
	void OpenContextMenu();

	UFUNCTION(BlueprintCallable)
	void PlayNextSound();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UContextMenu> ContextMenuClass;

	inline static const FName ADVISOR_SystemTaken = "SystemTaken";
	inline static const FName ADVISOR_SystemLost = "SystemLost";

private:
	UFUNCTION()
	void EmpireSystemChanged(ASystem* System, bool TakenOrLost);

	void PlayAdvisorPhrase(const FName& Key);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	USoundBase* NotificationSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TMap<FName, USoundBase*> AdvisorPhrases;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess))
	TArray<USoundBase*> MusicPlaySet;
	
	UPROPERTY()
	UEmpire* OwnedEmpire;

	IFocusable* FocusableUnderMouse;
	IFocusable* SelectedFocusable;

	UPROPERTY()
	UToolTip* CurrentToolTip;
	
	// Context Menu
	
	bool HasContextMenuAnchor;
	FVector ContextMenuAnchor;

	bool HasLocationUnderMouse;
	FVector LocationUnderMouse;
	
	UPROPERTY()
	UContextMenu* CurrentContextMenu;

	// Components

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess))
	UAudioComponent* MusicPlayer;
};
