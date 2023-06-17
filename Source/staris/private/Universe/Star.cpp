// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/Star.h"

#include "StarisStatics.h"
#include "UI/StarToolTip.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/System.h"
#include "Universe/VanillaStarTypeProperties.h"

UStar::UStar()
{

}

UToolTip* UStar::CreateToolTip()
{
	return NewObject<UStarToolTip>(this, ToolTipClass);
}

void UStar::SetupToolTip(UToolTip* ToolTip)
{
	Cast<UStarToolTip>(ToolTip)->SetupToolTip(
		GetTitle(),
		System->GetTitle(),
		FText::FromString(TypeRecord->GetOrCreateComponent<UVanillaStarTypeProperties>()->Title));
}

void UStar::ApplyPattern_Implementation(const FStarMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to star %s"), *Id.ToString())
		return;
	}

	Id = Data.Id;
	TypeRecord = Data.Type;
	Rename(*FString::Printf(TEXT("Star_%s"), *Id.ToString()));

	auto VanillaTypeProps = TypeRecord->GetComponent<UVanillaStarTypeProperties>();
	SetMaterial(0, VanillaTypeProps->MaterialInstance);
	
	SetRelativeLocation(Data.Location);
	SetRelativeScale3D(FVector(Data.Scale));
}
