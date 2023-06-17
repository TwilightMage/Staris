#pragma once
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogStaris, Log, All);

template<typename T>
T GetRandomArrayItem(const TArray<T>& Array, int32 Seed = MAX_int32) { return Array[Seed % Array.Num()]; }

template<typename T>
T* GetActorOfClass(UObject* WorldContext) { return Cast<T>(UGameplayStatics::GetActorOfClass(WorldContext, T::StaticClass())); }