// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/Star.h"

#include "StarisStatics.h"
#include "Empire/Empire.h"
#include "Game/StarisGameInstance.h"
#include "UI/GenericToolTip.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/System.h"
#include "Universe/VanillaStarTypeProperties.h"

UStar::UStar()
{

}

UToolTip* UStar::CreateToolTip()
{
	return NewObject<UGenericToolTip>(this, ToolTipClass);
}

void UStar::SetupToolTip(UToolTip* ToolTip)
{
	if (auto GenericToolTip = Cast<UGenericToolTip>(ToolTip))
	{
		UEmpire* Empire = System->GetOwningEmpire();

		if (UStarisGameInstance::DebugToolsEnabled)
		{
			GenericToolTip->AddLine(FText::Format(FTextFormat(NSLOCTEXT("Star", "StarToolTip_Star", "Star: {0} ({1})")), GetTitle(), FText::FromName(GetId())));
		}
		else
		{
			GenericToolTip->AddLine(FText::Format(FTextFormat(NSLOCTEXT("Star", "StarToolTip_Star", "Star: {0}")), GetTitle()));
		}
		
		GenericToolTip->AddLine(FText::Format(FTextFormat(NSLOCTEXT("Star", "StarToolTip_System", "System: {0} ({1})")), System->GetTitle(), FText::FromString(Empire ? Empire->Title : "Unowned")));
		GenericToolTip->AddLine(FText::Format(FTextFormat(NSLOCTEXT("Star", "StarToolTip_Class", "Class: {0}")), FText::FromString(TypeRecord->GetOrCreateComponent<UVanillaStarTypeProperties>()->Title)));
	}
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
	TypeRecord = Data.Type;
	Rename(*FString::Printf(TEXT("Star_%s"), *Id.ToString()));

	auto VanillaTypeProps = TypeRecord->GetComponent<UVanillaStarTypeProperties>();
	SetMaterial(0, VanillaTypeProps->MaterialInstance);
	
	SetRelativeLocation(Data.Location);
	SetRelativeScale3D(FVector(Data.Scale));
}
