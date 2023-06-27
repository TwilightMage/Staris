// Fill out your copyright notice in the Description page of Project Settings.


#include "Fleet/Fleet.h"

#include "StarisStatics.h"
#include "Fleet/FleetOrder.h"
#include "Fleet/FleetPath.h"
#include "Fleet/Ship.h"
#include "Game/StarisPlayerController.h"
#include "Universe/Galaxy.h"

#include "Universe/StarisInstancedStaticMesh.h"


AFleet::AFleet()
{
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("Root"));

	PrimaryActorTick.bCanEverTick = true;
}

void AFleet::BeginPlay()
{
	Super::BeginPlay();

	Galaxy = GetActorOfClass<AGalaxy>(this);
}

void AFleet::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const float TimeScale = Galaxy->GetRealTimeScale();
	float TickAmount = DeltaSeconds * TimeScale;
	
	while (TickAmount > 0 && (OrderExecutionState.CurrentPathNode || OrderExecutionState.ActiveOrder))
	{
		if (OrderExecutionState.CurrentPathNode)
		{
			if (OrderExecutionState.CurrentPathNode->MoveTick(TickAmount, this, OrderExecutionState.CurrentPathNodeDistance))
			{
				PathNodeFinished();
			}
		}

		if (!OrderExecutionState.CurrentPathNode && OrderExecutionState.ActiveOrder)
		{
			if (OrderExecutionState.ActiveOrder->ExecutionTick(TickAmount, this))
			{
				StartNextOrder();
			}
		}
	}

	if (TimeScale > 0)
	{
		auto FleetTransform = GetRootComponent()->GetComponentTransform();
		for (auto Ship : Ships)
		{
			FTransform ShipTransform;
			Ship->Tick(DeltaSeconds * TimeScale, FleetTransform, ShipTransform);
			Ship->MeshInstanceRef->GetSourceWeak()->UpdateInstanceTransform(Ship->MeshInstanceRef->GetIndex(), ShipTransform, true, true);
		}
	}

	//for (auto& InstancedMesh : InstancedShipMeshes)
	//{
	//	InstancedMesh.Value->MarkRenderInstancesDirty();
	//}
}

void AFleet::Setup(UEmpire* Empire)
{
	OwningEmpire = Empire;
}

bool AFleet::AssignShip(UShip* Ship, bool bForce)
{
	if (!Ships.IsEmpty() && Ships[0]->FullFleetShip()) return false;
	
	if (bForce || GetCurrentUsedControl() + Ship->ControlRequired <= ControlLimit)
	{
		Ship->Rename(nullptr, this);

		auto& InstancedMesh = InstancedShipMeshes.FindOrAdd(Ship->StaticMesh);
		if (!InstancedMesh)
		{
			InstancedMesh = NewObject<UStarisInstancedStaticMesh>(this);
			InstancedMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);
			InstancedMesh->RegisterComponent();
			InstancedMesh->SetStaticMesh(Ship->StaticMesh);
		}
		
		Ships.Add(Ship);

		auto Formation = CalculateFormation();
		for (int32 i = 0; i < Ships.Num(); i++)
		{
			Ships[i]->PositionInFormation = Formation[i];
		}

		auto FleetTransform = GetActorTransform();
		Ship->Transform = FTransform(FleetTransform.Rotator(), FleetTransform.TransformPosition(Ship->PositionInFormation));
		Ship->MeshInstanceRef = InstancedMesh->AddObjectInstance(Ship->Transform, true);
		Ship->MeshInstanceRef->Object = Ship;

		float SpeedSum = 0;
		for (auto FleetShip : Ships)
		{
			SpeedSum += FleetShip->GetSpeed();
		}
		SpeedCached = SpeedSum / Ships.Num();
		
		return true;
	}

	return false;
}

bool AFleet::CanExecuteOrder(UFleetOrder* Order) const
{
	for (auto Ship : Ships)
	{
		if (Ship->CanExecuteOrder(Order)) return true;
	}

	return false;
}

void AFleet::PutOrder(UFleetOrder* Order, bool bAssign)
{
	if (!bAssign)
	{
		CancelActiveOrder(true);
	}

	Orders.Add(Order);

	if (!OrderExecutionState.ActiveOrder)
	{
		StartNextOrder();
	}
}

void AFleet::PutOrderAuto(UFleetOrder* Order)
{
	if (auto Controller = GetActorOfClass<AStarisPlayerController>(this))
	{
		PutOrder(Order, Controller->IsAssignOrderDown());
	}
}

void AFleet::CancelActiveOrder(bool bCancelAllOrders)
{
	OrderExecutionState = FOrderExecutionState();
	
	if (bCancelAllOrders)
	{
		Orders.SetNum(0);
	}
}

int32 AFleet::GetCurrentUsedControl()
{
	int32 Result = 0;
	
	for (int32 i = 0; i < Ships.Num(); i++)
	{
		Result += Ships[i]->ControlRequired;
	}

	return Result;
}

TArray<FVector> AFleet::CalculateFormation() const
{
	TArray<FVector> Positions;
	Positions.SetNum(Ships.Num());

	int Row = 0;
	int j = 0;
	for (int32 i = 0; i < Ships.Num(); i++)
	{
		Positions[i] = FVector(-20 * Row, 20 * j - 10 * Row, 0);;

		if (j++ == Row)
		{
			Row++;
			j = 0;
		}
	}

	return Positions;
}

void AFleet::StartNextOrder()
{
	OrderExecutionState = FOrderExecutionState();

	while (!Orders.IsEmpty())
	{
		OrderExecutionState.ActiveOrder = Orders[0];
		Orders.RemoveAt(0);

		if (CanExecuteOrder(OrderExecutionState.ActiveOrder)) break;
	}

	if (OrderExecutionState.ActiveOrder)
	{
		if (auto Path = OrderExecutionState.ActiveOrder->BuildPathToTarget(GetActorLocation()))
		{
			OnOrderStarted.Broadcast(OrderExecutionState.ActiveOrder);
			OnOrderStarted_K2.Broadcast(OrderExecutionState.ActiveOrder);
			
			OrderExecutionState.CurrentPathNode = Path->GetFirstNode();

			PathNodeFinished();
		}
		else
		{
			OrderExecutionState = FOrderExecutionState();
			Orders.SetNum(0);
		}
	}
}

void AFleet::PathNodeFinished()
{
	OrderExecutionState.CurrentPathNode = OrderExecutionState.CurrentPathNode->NextNode;
	OrderExecutionState.CurrentPathNodeDistance = 0;
}
