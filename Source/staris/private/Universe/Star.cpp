// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/Star.h"

#include "StarisStatics.h"
#include "Empire/Empire.h"
#include "Game/StarisGameInstance.h"
#include "UI/ContextMenu.h"
#include "UI/ToolTip.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/Galaxy.h"
#include "Universe/StarisInstancedStaticMesh.h"
#include "Universe/System.h"
#include "Universe/VanillaStarTypeProperties.h"

UStar::UStar()
{

}

void UStar::SetupToolTip(UToolTip* ToolTip)
{
	UEmpire* Empire = GetSystem()->GetOwningEmpire();

	ToolTip->AddLine(FText::Format(NSLOCTEXT("Star", "StarToolTip_Star", "Star: {0}"), GetTitle()));
	
	ToolTip->AddLine(FText::Format(NSLOCTEXT("Star", "StarToolTip_System", "System: {0} ({1})"), GetSystem()->GetTitle(), FText::FromString(Empire ? Empire->Title : "Unowned")));
	//ToolTip->AddLine(FText::Format(NSLOCTEXT("Star", "StarToolTip_Class", "Class: {0}"), FText::FromString(TypeRecord->GetOrCreateComponent<UVanillaStarTypeProperties>()->Title)));

	if (UStarisGameInstance::DebugToolsEnabled)
	{
		ToolTip->AddSeparator();
		ToolTip->AddLine(FText::Format(NSLOCTEXT("Star", "StarToolTip_StarId", "Star ID: {0}"), FText::FromName(Id)));
		ToolTip->AddLine(FText::Format(NSLOCTEXT("Star", "StarToolTip_StarSeed", "Star Seed: {0}"), FText::FromString(FString::FromInt(GetSeed()))));
		ToolTip->AddLine(FText::Format(NSLOCTEXT("Star", "StarToolTip_SystemId", "System ID: {0}"), FText::FromName(GetSystem()->GetId())));
		ToolTip->AddLine(FText::Format(NSLOCTEXT("Star", "StarToolTip_SystemSeed", "System Seed: {0}"), FText::FromString(FString::FromInt(GetSystem()->GetSeed()))));
	}
}

TArray<UContextMenuItem*> UStar::CreateContextActionsHovered(IFocusable* Selected)
{
	TArray<UContextMenuItem*> Result;

	if (UStarisGameInstance::DebugToolsEnabled)
	{
		Result.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Star", "(DEBUG) Copy Star ID", "(DEBUG) Copy Star ID"), FText(), FContextMenuActionDelegate::CreateLambda([this]()
		{
			FGenericPlatformMisc::ClipboardCopy(*GetId().ToString());
		}), nullptr));

		Result.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Star", "(DEBUG) Copy Star Seed", "(DEBUG) Copy Star Seed"), FText(), FContextMenuActionDelegate::CreateLambda([this]()
		{
			FGenericPlatformMisc::ClipboardCopy(*FString::FromInt(GetSeed()));
		}), nullptr));
		
		UContextMenuSeparator::AddSeparatorIfNeeded(Result);

		Result.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Star", "(DEBUG) Copy System ID", "(DEBUG) Copy System ID"), FText(), FContextMenuActionDelegate::CreateLambda([this]()
		{
			FGenericPlatformMisc::ClipboardCopy(*GetSystem()->GetId().ToString());
		}), nullptr));

		Result.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Star", "(DEBUG) Copy System Seed", "(DEBUG) Copy System Seed"), FText(), FContextMenuActionDelegate::CreateLambda([this]()
		{
			FGenericPlatformMisc::ClipboardCopy(*FString::FromInt(GetSystem()->GetSeed()));
		}), nullptr));
	}

	return Result;
}

USceneLabel* UStar::CreateLabel()
{
	return nullptr;
}

void UStar::SetupLabel(USceneLabel* Label)
{
}

FVector UStar::GetLabelLocation() const
{
	return GetLocation() + FVector(0, 0, Scale * 50 + 5);
}

FText UStar::GetTitle() const
{
	return Title.IsEmpty() ? FText::FromName(Id) : FText::FromString(Title);
}

UTexture2D* UStar::GetIcon() const
{
	return Icon;
}

USystem* UStar::GetSystem() const
{
	return Cast<USystem>(GetOuter());
}

void UStar::ApplyPattern_Implementation(const FStarMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to star %s"), *Id.ToString())
		return;
	}

	Title = Data.Title;
	Id = Data.Id;
	Seed = Data.Seed;
	Scale = Data.Scale;
	TypeRecord = Data.Type;

	if (auto Galaxy = GetGalaxy())
	{
		Galaxy->RegisterObjectById(Id, this);
		
		if (auto System = GetSystem())
		{
			Location = System->GetLocation() + Data.Location;
			MeshInstance = Galaxy->CreateStarInstance(Data.Type, FTransform(FRotator::ZeroRotator, Location, FVector(Data.Scale)));
			MeshInstance->Object = this;
		}
	}
}
