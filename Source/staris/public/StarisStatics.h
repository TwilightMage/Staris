#pragma once
#include "Kismet/GameplayStatics.h"

DECLARE_LOG_CATEGORY_EXTERN(LogStaris, Log, All);

template<typename T>
T GetRandomArrayItem(const TArray<T>& Array, int32 Seed = MAX_int32) { return Array[Seed % Array.Num()]; }

template<typename T>
T GetRandomArrayItem(const TArray<T>& Array, FRandomStream& RandomStream) { return Array[(int32)(RandomStream.FRand() * MAX_int32) % Array.Num()]; }

template<typename T>
T GetRandomSetItem(const TSet<T>& Set, int32 Seed = MAX_int32) { return Set[FSetElementId::AsInteger(Seed % Set.Num())]; }

template<typename T>
T GetRandomSetItem(const TSet<T>& Set, FRandomStream& RandomStream) { return Set[FSetElementId::AsInteger((int32)(RandomStream.FRand() * MAX_int32) % Set.Num())]; }

template<typename T>
T* GetActorOfClass(const UObject* WorldContext) { return Cast<T>(UGameplayStatics::GetActorOfClass(WorldContext, T::StaticClass())); }

template<typename T>
T UnLerp(T Val, T Min, T Max) { return (Val - Min) / (Max - Min); }