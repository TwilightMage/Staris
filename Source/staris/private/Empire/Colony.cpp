// Fill out your copyright notice in the Description page of Project Settings.


#include "Empire/Colony.h"

TMap<UCompositeRecord*, int32> UColony::GetResourceBalance() const
{
	return {};
}

int32 UColony::getTotalPopulationAmount() const
{
	int32 Result = 0;
	for (const auto& Pop : Population)
	{
		Result += Pop.Value;
	}

	return Result;
}
