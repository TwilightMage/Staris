// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/Star.h"

#include "StarisStatics.h"


UStar::UStar()
{

}

void UStar::ApplyPattern_Implementation(const FStarMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to star %s"), *Id.ToString())
		return;
	}

	Id = Data.Id;
	Type = Data.Type;
	Rename(*FString::Printf(TEXT("Star_%s"), *Id.ToString()));
	
	SetRelativeLocation(Data.Location);
	SetRelativeScale3D(FVector(Data.Scale));
}
