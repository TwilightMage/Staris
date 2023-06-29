// Fill out your copyright notice in the Description page of Project Settings.


#include "Universe/Planet.h"

#include "StarisStatics.h"
#include "Empire/Colony.h"
#include "Empire/Empire.h"
#include "Empire/EmpirePlanetKnowledge.h"
#include "Fleet/DiscoverPlanetOrder.h"
#include "Fleet/Fleet.h"
#include "Fleet/Ship.h"
#include "Game/StarisGameInstance.h"
#include "Game/StarisHUD.h"
#include "Game/StarisPlayerController.h"
#include "Misc/DefaultValueHelper.h"
#include "UI/ContextMenu.h"
#include "UI/ToolTip.h"
#include "Universe/Galaxy.h"
#include "Universe/StarisInstancedStaticMesh.h"
#include "Universe/System.h"

const int32 OrbitPositionsCount = 365 * 10;
FVector2D OrbitPositions[OrbitPositionsCount];
bool OrbitPositionsSet = false;

UPlanet::UPlanet()
{
	if (!OrbitPositionsSet)
	{
		for (int32 i = 0; i < OrbitPositionsCount; i++)
		{
			//const auto a = sizeof(OrbitPositions);
			OrbitPositions[i] = FVector2D(FMath::Cos(2 * 3.14 * (i / (float)OrbitPositionsCount)), FMath::Sin(2 * 3.14 * (i / (float)OrbitPositionsCount)));
		}

		OrbitPositionsSet = true;
	}
	
	const static auto DiscoverLayerCommand = IConsoleManager::Get().RegisterConsoleCommand(TEXT("Staris.DiscoverPlanetLayer"), TEXT("<Planet ID> <Layer> - Unlock whole layer on the planet provided"), FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (Args.Num() <= 2)
		{
			if (!UKismetSystemLibrary::IsDedicatedServer(World))
			{
				if (auto PlayerController = Cast<AStarisPlayerController>(UGameplayStatics::GetPlayerController(World, 0)))
				{
					if (auto Empire = PlayerController->GetEmpire())
					{
						if (auto Galaxy = GetActorOfClass<AGalaxy>(World))
						{
							if (auto Planet = Cast<UPlanet>(Galaxy->GetObjectById(FName(Args[0]))))
							{
								auto Knowledge = Empire->GetOrCreatePlanetKnowledge(Planet);
								int32 Layer = 0;
								FDefaultValueHelper::ParseInt(Args[1], Layer);
								Knowledge->UnlockLayer(Layer);
							}
						}
					}
				}	
			}
		}
	}));
}

UColony* UPlanet::SettleColony()
{
	if (Colony) return nullptr;

	Colony = NewObject<UColony>(this);
	RegisterOnMonthPassed();

	return Colony;
}

void UPlanet::OnMonthPassed(int32 Month)
{
	//SetRelativeLocation(OrbitPoint + FVector(OrbitPositions[(int)(Day + OrbitOffset * (float)OrbitPositionsCount) % OrbitPositionsCount] * OrbitDistance, 0));

	if (Colony)
	{
		Colony->MonthPassed();
	}
}

void UPlanet::SetupToolTip(UToolTip* ToolTip)
{
	UEmpire* Empire = GetSystem()->GetOwningEmpire();

	ToolTip->AddLine(FText::Format(NSLOCTEXT("Planet", "PlanetToolTip_Star", "Planet: {0}"), GetTitle()));
	ToolTip->AddLine(FText::Format(NSLOCTEXT("Planet", "PlanetToolTip_System", "System: {0} ({1})"), GetSystem()->GetTitle(), FText::FromString(Empire ? Empire->Title : "Unowned")));
	//ToolTip->AddLine(FText::Format(NSLOCTEXT("Planet", "PlanetToolTip_AverageTemperature", "Average Temperature: {0}°C"), Temperature));

	if (Colony)
	{
		//ToolTip->AddLine(FText::Format(NSLOCTEXT("Planet", "PlanetToolTip_Population", "Population: {0}"), Colony->getTotalPopulationAmount()));
		if (Empire->Capital == this)
		{
			ToolTip->AddLine(FText::Format(NSLOCTEXT("Planet", "PlanetToolTip_State", "State: Capital of {0}"), FText::FromString(Empire->Title)));
		}
		else
		{
			ToolTip->AddLine(FText::Format(NSLOCTEXT("Planet", "PlanetToolTip_State", "State: Colony of {0}"), FText::FromString(Empire->Title)));
		}
	}

	if (UStarisGameInstance::DebugToolsEnabled)
	{
		ToolTip->AddSeparator();
		ToolTip->AddLine(FText::Format(NSLOCTEXT("Planet", "PlanetToolTip_PlanetSeed", "Planet ID: {0}"), FText::FromName(Id)));
		ToolTip->AddLine(FText::Format(NSLOCTEXT("Planet", "PlanetToolTip_PlanetSeed", "Planet Seed: {0}"), FText::FromString(FString::FromInt(GetSeed()))));
	}
}

TArray<UContextMenuItem*> UPlanet::CreateContextActionsHovered(IFocusable* Selected)
{
	TArray<UContextMenuItem*> Result;

	if (auto Ship = Cast<UShip>(Selected))
	{
		if (auto Fleet = Ship->GetFleet())
		{
			auto Order = NewObject<UDiscoverPlanetOrder>(Fleet);
			Order->TargetPlanet = this;

			if (Fleet->CanExecuteOrder(Order))
			{
				Result.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Planet", "Discover Planet", "Discover Planet"), FText(), FContextMenuActionDelegate::CreateLambda([this, Fleet, Order]()
				{
					Fleet->PutOrderAuto(Order);
				}), nullptr));
			}
		}
	}

	if (UStarisGameInstance::DebugToolsEnabled)
	{
		UContextMenuSeparator::AddSeparatorIfNeeded(Result);
		
		Result.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Planet", "(DEBUG) Copy ID", "(DEBUG) Copy ID"), FText(), FContextMenuActionDelegate::CreateLambda([this]()
		{
			FGenericPlatformMisc::ClipboardCopy(*Id.ToString());
		}), nullptr));

		Result.Add(UContextMenuActionCPP::Create(this, NSLOCTEXT("Planet", "(DEBUG) Copy Planet Seed", "(DEBUG) Copy Planet Seed"), FText(), FContextMenuActionDelegate::CreateLambda([this]()
		{
			FGenericPlatformMisc::ClipboardCopy(*FString::FromInt(GetSeed()));
		}), nullptr));
	}

	return Result;
}

USceneLabel* UPlanet::CreateLabel()
{
	return nullptr;
}

void UPlanet::SetupLabel(USceneLabel* Label)
{
}

FVector UPlanet::GetLabelLocation() const
{
	return GetLocation() + FVector(0, 0, Scale * 15 + 5);
}

void UPlanet::OnSelected()
{
	if (auto HUD = GetActorOfClass<AStarisHUD>(this))
	{
		HUD->OpenFocusablePanel(this);
	}
}

FText UPlanet::GetTitle() const
{
	return Title.IsEmpty() ? FText::FromName(Id) : FText::FromString(Title);
}

UTexture2D* UPlanet::GetIcon() const
{
	return Icon;
}

void UPlanet::AddTileToLayer(int32 Layer, UCompositeRecord* ResourceType)
{
	Layers[Layer].ResourceTiles.Add(ResourceType);
}

USystem* UPlanet::GetSystem() const
{
	return Cast<USystem>(GetOuter());
}

void UPlanet::OnInitCelestialEntity()
{
	//RegisterOnDayPassed();
}

void UPlanet::ApplyPattern_Implementation(const FPlanetMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to planet %s"), *Id.ToString())
		return;
	}

	Id = Data.Id;
	Seed = Data.Seed;
	Temperature = Data.Temperature;
	Scale = Data.Scale;
	Radius = Data.Scale * 12000;
	Layers = Data.Layers;

	OrbitPoint = Data.OrbitPoint;
	OrbitOffset = Data.OrbitOffset;
	OrbitDistance = Data.OrbitDistance;
	OrbitSpeed = Data.OrbitSpeed;

	if (auto Galaxy = GetGalaxy())
	{
		Galaxy->RegisterObjectById(Id, this);
		
		if (auto System = GetSystem())
		{
			const auto PlanetPosition = Data.OrbitPoint + FVector(OrbitPositions[(int)(0 + Data.OrbitOffset * (float)OrbitPositionsCount)] * Data.OrbitDistance, 0);
			Location = System->GetLocation() + PlanetPosition;
			MeshInstance = Galaxy->CreatePlanetInstance(FTransform(FRotator::ZeroRotator, Location, FVector(Data.Scale)));
			MeshInstance->Object = this;
		}
	}
}
