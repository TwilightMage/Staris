// Copyright DragonCap. All rights reserved.


#include "Universe/StarisInstancedStaticMesh.h"

void UMeshInstanceRef::RemoveInstance()
{
	if (auto SourceInstancedMesh = GetSource())
	{
		SourceInstancedMesh->RemoveInstance(Index);
	}
}

bool UStarisInstancedStaticMesh::RemoveInstance(int32 InstanceIndex)
{
	auto Object = InstanceObjectMap[InstanceIndex];
	Object->Source = nullptr;
	InstanceObjectMap.Remove(InstanceIndex);
	for (auto& Entry : InstanceObjectMap)
	{
		if (Entry.Key > InstanceIndex)
		{
			Entry.Key--;
			Entry.Value->Index--;
		}
	}
	
	return Super::RemoveInstance(InstanceIndex);
}

bool UStarisInstancedStaticMesh::RemoveInstances(const TArray<int32>& InstancesToRemove)
{
	for (auto Instance : InstancesToRemove)
	{
		RemoveInstance(Instance);
	}

	return true;
}

UMeshInstanceRef* UStarisInstancedStaticMesh::AddObjectInstance(const FTransform& Transform, bool bWorldSpace)
{
	UMeshInstanceRef* Result = NewObject<UMeshInstanceRef>(this);
	Result->Source = this;
	Result->Index = AddInstance(Transform, bWorldSpace);
	InstanceObjectMap.Add(Result->Index, Result);
	return Result;
}

TArray<UMeshInstanceRef*> UStarisInstancedStaticMesh::AddObjectInstances(const TArray<FTransform>& Transforms, bool bWorldSpace)
{
	TArray<UMeshInstanceRef*> Result;
	for (auto Instance : AddInstances(Transforms, true, bWorldSpace))
	{
		UMeshInstanceRef* InstanceRef = NewObject<UMeshInstanceRef>(this);
		InstanceRef->Source = this;
		InstanceRef->Index = Instance;
		InstanceObjectMap.Add(InstanceRef->Index, InstanceRef);
	}

	return Result;
}

UMeshInstanceRef* UStarisInstancedStaticMesh::TryGetObjectRef(int32 InstanceIndex)
{
	return InstanceObjectMap.FindRef(InstanceIndex);
}
