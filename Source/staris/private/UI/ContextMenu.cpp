// Copyright DragonCap. All rights reserved.


#include "UI/ContextMenu.h"

void UContextMenuAction::Execute()
{
	if (auto ContextMenu = OwningContextMenu.Get())
	{
		ContextMenu->RemoveFromParent();
	}
}

void UContextMenuActionBlueprint::Execute()
{
	Super::Execute();
	
	Action.ExecuteIfBound();
}

bool UContextMenuActionBlueprint::IsEnabled() const
{
	if (EnabledDelegate.IsBound())
	{
		return EnabledDelegate.Execute();
	}

	return true;
}

UContextMenuActionCPP* UContextMenuActionCPP::Create(UObject* Outer, const FText& Title, const FText& ToolTip, const FContextMenuActionDelegate& ActionDelegate, const FContextMenuActionEnabledDelegate& IsEnabledDelegate)
{
	auto Result = NewObject<UContextMenuActionCPP>(Outer);
	Result->Title = Title;
	Result->ToolTip = ToolTip;
	Result->Action = ActionDelegate;
	Result->EnabledDelegate = IsEnabledDelegate;

	return Result;
}

void UContextMenuActionCPP::Execute()
{
	Super::Execute();
	
	Action.ExecuteIfBound();
}

bool UContextMenuActionCPP::IsEnabled() const
{
	if (EnabledDelegate.IsBound())
	{
		return EnabledDelegate.Execute();
	}

	return true;
}

void UContextMenuSeparator::AddSeparatorIfNeeded(TArray<UContextMenuItem*>& ItemSet)
{
	if (!ItemSet.IsEmpty() && !ItemSet.Last()->IsA<UContextMenuSeparator>())
	{
		ItemSet.Add(NewObject<UContextMenuSeparator>());
	}
}
