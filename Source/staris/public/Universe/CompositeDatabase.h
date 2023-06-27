// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "CompositeDatabase.generated.h"

UCLASS(Abstract, Blueprintable)
class UCompositeRecordComponent : public UObject
{
	GENERATED_BODY()
};

UCLASS(BlueprintType)
class UCompositeRecord : public UObject
{
	GENERATED_BODY()
	
public:
	template<typename T>
	T* GetComponent()
	{
		if (auto ComponentFound = Components.Find(T::StaticClass()))
		{
			return Cast<T>(*ComponentFound);
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, DisplayName="Get Component", meta=(DeterminesOutputType=ComponentClass, AutoCreateRefTerm=ComponentClass, DynamicOutputParam=Component))
	void GetComponent_K2(const TSubclassOf<UCompositeRecordComponent>& ComponentClass, UCompositeRecordComponent*& Component)
	{
		Component = nullptr;
		if (auto ComponentFound = Components.Find(ComponentClass))
		{
			Component = *ComponentFound;
		}
	}

	template<typename ComponentType>
	ComponentType* GetOrCreateComponent()
	{
		if (auto ComponentFound = Components.Find(ComponentType::StaticClass()))
		{
			return Cast<ComponentType>(*ComponentFound);
		}

		if (RecordComponentType && !ComponentType::StaticClass()->IsChildOf(RecordComponentType))
		{
			return nullptr;
		}

		ComponentType* NewComponent = NewObject<ComponentType>(this, ComponentType::StaticClass());
		Components.Add(ComponentType::StaticClass(), NewComponent);
		return NewComponent;
	}

	UFUNCTION(BlueprintCallable, DisplayName="Get Or Create Component", meta=(DeterminesOutputType=ComponentClass, AutoCreateRefTerm=ComponentClass, DynamicOutputParam=Component))
	void GetOrCreateComponent_K2(const TSubclassOf<UCompositeRecordComponent>& ComponentClass, UCompositeRecordComponent*& Component)
	{
		Component = nullptr;
		
		if (auto ComponentFound = Components.Find(ComponentClass))
		{
			Component = *ComponentFound;
			return;
		}

		if (!ComponentClass->IsChildOf(RecordComponentType))
		{
			UE_LOG(LogStats, Error, TEXT("Failed to create composite record component of class %s. Record component base class is %s."), *ComponentClass->GetName(), *RecordComponentType->GetName())
			return;
		}

		Component = NewObject<UCompositeRecordComponent>(this, ComponentClass);
		Components.Add(ComponentClass, Component);
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCompositeRecordComponent> RecordComponentType;

private:
	UPROPERTY()
	TMap<TSubclassOf<UCompositeRecordComponent>, UCompositeRecordComponent*> Components;
};

UCLASS(BlueprintType)
class UCompositeDatabase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm=Key))
	UCompositeRecord* GetRecord(const FName& Key)
	{
		if (auto FoundRecord = Records.Find(Key))
		{
			return *FoundRecord;
		}

		return nullptr;
	}

	UFUNCTION(BlueprintCallable, meta=(AutoCreateRefTerm=Key))
	UCompositeRecord* GetOrCreateRecord(const FName& Key)
	{
		auto& Record = Records.FindOrAdd(Key);
		if (!Record)
		{
			Record = NewObject<UCompositeRecord>(this);
			Record->RecordComponentType = RecordComponentType;
		}

		return Record;
	}

	const TMap<FName, UCompositeRecord*>& GetAllRecords() const { return Records; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCompositeRecordComponent> RecordComponentType;

private:
	UPROPERTY()
	TMap<FName, UCompositeRecord*> Records;
};
