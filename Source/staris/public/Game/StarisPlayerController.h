// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StarisController.h"
#include "GameFramework/PlayerController.h"

#include "StarisPlayerController.generated.h"

class USceneLabel;
class ILabeled;
class USystem;
class USoundCue;
class UContextMenu;
class IFocusable;
class UToolTip;

DECLARE_MULTICAST_DELEGATE_OneParam(FSelectedFocusableChangedEvent, IFocusable* /* New Selected Focusable */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSelectedFocusableChangedDynamicEvent, const TScriptInterface<IFocusable>&, NewSelectedFocusable);

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

	bool IsAssignOrderDown() const { return AssignOrderDown; }

	UFUNCTION(BlueprintCallable)
	void Click();
	void OpenContextMenu();
	
	void SelectFocusable(IFocusable* NewFocusable);

	UFUNCTION(BlueprintCallable, DisplayName="Select Focusable", meta=(AutoCreateRefTerm=NewFocusable))
	void SelectFocusable_K2(const TScriptInterface<IFocusable>& NewFocusable);
	
	UFUNCTION(BlueprintCallable)
	void PlayNextMusic();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UToolTip> ToolTipClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UContextMenu> ContextMenuClass;

	inline static const FName ADVISOR_SystemTaken = "SystemTaken";
	inline static const FName ADVISOR_SystemLost = "SystemLost";

	FSelectedFocusableChangedEvent SelectedFocusableChanged;

	UPROPERTY(BlueprintAssignable, DisplayName="Selected Focusable Changed")
	FSelectedFocusableChangedDynamicEvent SelectedFocusableChanged_K2;

private:
	UFUNCTION()
	void EmpireSystemChanged(USystem* System, bool TakenOrLost);

	void PlayAdvisorPhrase(const FName& Key);

	void AssignOrderPressed() { AssignOrderDown = true; }
	void AssignOrderReleased() { AssignOrderDown = false; }
	
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

	bool AssignOrderDown = false;
	
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
