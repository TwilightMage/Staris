// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "ContextMenu.generated.h"

DECLARE_DYNAMIC_DELEGATE(FContextMenuDynamicActionDelegate);
DECLARE_DELEGATE(FContextMenuActionDelegate);

DECLARE_DYNAMIC_DELEGATE_RetVal(bool, FContextMenuActionEnabledDynamicDelegate);
DECLARE_DELEGATE_RetVal(bool, FContextMenuActionEnabledDelegate);

UCLASS(Abstract)
class UContextMenuItem : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FText ToolTip;
};

UCLASS(Abstract, Blueprintable)
class UContextMenuAction : public UContextMenuItem
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual void Execute();

	UFUNCTION(BlueprintCallable)
	virtual bool IsEnabled() const { return true; }

	TWeakObjectPtr<UContextMenu> OwningContextMenu;
};

UCLASS()
class UContextMenuActionBlueprint : public UContextMenuAction
{
	GENERATED_BODY()
	
public:
	virtual void Execute() override;
	virtual bool IsEnabled() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FContextMenuDynamicActionDelegate Action;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn))
	FContextMenuActionEnabledDynamicDelegate EnabledDelegate;
};

UCLASS(NotBlueprintable)
class UContextMenuActionCPP : public UContextMenuAction
{
	GENERATED_BODY()
	
public:
	static UContextMenuActionCPP* Create(UObject* Outer, const FText& Title, const FText& ToolTip, const FContextMenuActionDelegate& ActionDelegate, const FContextMenuActionEnabledDelegate& IsEnabledDelegate);
	
	virtual void Execute() override;
	virtual bool IsEnabled() const override;
	
	FContextMenuActionDelegate Action;
	FContextMenuActionEnabledDelegate EnabledDelegate;
};

//UCLASS()
//class UContextMenuCategory : public UContextMenuItem
//{
//	GENERATED_BODY()
//
//public:
//	UPROPERTY(BlueprintReadWrite)
//	TArray<UContextMenuItem*> Items;
//};

UCLASS()
class STARIS_API UContextMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddItem(UContextMenuItem* Item);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void AddSeparator();
};
