// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "StarisStatics.h"
#include "UObject/Object.h"

#include "CompositeDatabase.generated.h"

class UCompositeDatabasePreset;

UCLASS(EditInlineNew, Abstract, Blueprintable)
class UCompositeRecordComponent : public UObject
{
	GENERATED_BODY()
};

UCLASS(EditInlineNew, BlueprintType)
class UCompositeRecord : public UObject
{
	GENERATED_BODY()

	friend UCompositeDatabase;
	
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
	void GetComponent_K2(UPARAM(meta=(AllowAbstract=false)) const TSubclassOf<UCompositeRecordComponent>& ComponentClass, UCompositeRecordComponent*& Component)
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

		auto RecordComponentType = GetRecordComponentType();
		if (RecordComponentType && !ComponentType::StaticClass()->IsChildOf(RecordComponentType))
		{
			return nullptr;
		}

		ComponentType* NewComponent = NewObject<ComponentType>(this, ComponentType::StaticClass());
		Components.Add(ComponentType::StaticClass(), NewComponent);
		return NewComponent;
	}

	UFUNCTION(BlueprintCallable, DisplayName="Get Or Create Component", meta=(DeterminesOutputType=ComponentClass, AutoCreateRefTerm=ComponentClass, DynamicOutputParam=Component))
	void GetOrCreateComponent_K2(UPARAM(meta=(AllowAbstract=false)) const TSubclassOf<UCompositeRecordComponent>& ComponentClass, UCompositeRecordComponent*& Component)
	{
		Component = nullptr;
		
		if (auto ComponentFound = Components.Find(ComponentClass))
		{
			Component = *ComponentFound;
			return;
		}

		if (!ComponentClass->IsChildOf(GetRecordComponentType()))
		{
			UE_LOG(LogStats, Error, TEXT("Failed to create composite record component of class %s. Record component base class is %s."), *ComponentClass->GetName(), *GetRecordComponentType()->GetName())
			return;
		}

		Component = NewObject<UCompositeRecordComponent>(this, ComponentClass);
		Components.Add(ComponentClass, Component);
	}

	UFUNCTION(BlueprintPure)
	TSubclassOf<UCompositeRecordComponent> GetRecordComponentType() const;

private:
	UPROPERTY(EditAnywhere, Instanced, meta=(AllowedClasses=RecordComponentType, ExactClass=false))
	TMap<TSubclassOf<UCompositeRecordComponent>, UCompositeRecordComponent*> Components;
};

UCLASS(EditInlineNew, BlueprintType)
class UCompositeDatabase : public UObject
{
	GENERATED_BODY()

public:
	void ApplyPreset(UCompositeDatabasePreset* Preset);
	
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
			UE_LOG(LogStaris, Warning, TEXT("Record %s in database of %s has been created"), *Key.ToString(), *RecordComponentType->GetName());
			Record = NewObject<UCompositeRecord>(this);
		}

		return Record;
	}

	const TMap<FName, UCompositeRecord*>& GetAllRecords() const { return Records; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowAbstract=true))
	TSubclassOf<UCompositeRecordComponent> RecordComponentType;

private:
	UPROPERTY(EditAnywhere, Instanced)
	TMap<FName, UCompositeRecord*> Records;
};


UCLASS(EditInlineNew, BlueprintType)
class UCompositeDatabaseRecordPreset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UCompositeRecordComponent*> Components;
};

UCLASS(EditInlineNew, BlueprintType)
class UCompositeDatabasePreset : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Instanced)
	TMap<FName, UCompositeDatabaseRecordPreset*> Records;
};
