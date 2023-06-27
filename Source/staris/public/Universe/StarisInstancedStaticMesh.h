// Copyright DragonCap. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/InstancedStaticMeshComponent.h"

#include "StarisInstancedStaticMesh.generated.h"

class UStarisInstancedStaticMesh;

UCLASS()
class UMeshInstanceRef : public UObject
{
	GENERATED_BODY()

	friend UStarisInstancedStaticMesh;
	
public:
	int32 GetIndex() const { return Index; }
	UStarisInstancedStaticMesh* GetSource() const { return Source.Get(); }
	const TWeakObjectPtr<UStarisInstancedStaticMesh>& GetSourceWeak() const { return Source; }

	void RemoveInstance();
	
	TWeakObjectPtr<UObject> Object;

private:
	int32 Index;
	TWeakObjectPtr<UStarisInstancedStaticMesh> Source;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STARIS_API UStarisInstancedStaticMesh : public UInstancedStaticMeshComponent
{
	GENERATED_BODY()

public:
	virtual bool RemoveInstance(int32 InstanceIndex) override;
	virtual bool RemoveInstances(const TArray<int32>& InstancesToRemove) override;
	
	UMeshInstanceRef* AddObjectInstance(const FTransform& Transform, bool bWorldSpace = false);
	TArray<UMeshInstanceRef*> AddObjectInstances(const TArray<FTransform>& Transforms, bool bWorldSpace = false);

	UMeshInstanceRef* TryGetObjectRef(int32 InstanceIndex);
	
protected:
	UPROPERTY()
	TMap<int32, UMeshInstanceRef*> InstanceObjectMap;
};
