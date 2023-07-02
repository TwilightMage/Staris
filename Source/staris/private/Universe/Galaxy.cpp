// Fill out your copyright notice in the Description page of Project Settings.

#include "Universe/Galaxy.h"

#include "StarisStatics.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Empire/Empire.h"
#include "Engine/PointLight.h"
#include "Game/StarisGameInstance.h"
#include "Game/StarisGameMode.h"
#include "Game/StarisGraphicsSettings.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Universe/CelestialEntity.h"
#include "Universe/CompositeDatabase.h"
#include "Universe/GalaxySettingsManager.h"
#include "Universe/Star.h"
#include "Universe/StarisInstancedStaticMesh.h"
#include "Universe/System.h"
#include "Universe/VanillaGalaxyGenerator.h"
#include "Universe/VanillaGalaxySettings.h"
#include "Universe/VanillaStarTypeProperties.h"

AGalaxy::AGalaxy()
	: AActor()
{
	static auto TimeScaleCommand = IConsoleManager::Get().RegisterConsoleCommand(TEXT("Staris.SetTimeScale"), TEXT("Set the time scale for the game runninng"), FConsoleCommandWithWorldAndArgsDelegate::CreateLambda([](const TArray<FString>& Args, UWorld* World)
	{
		if (Args.Num() >= 1)
		{
			if (auto Galaxy = GetGalaxy(World))
			{
				Galaxy->SetTimeScale(FCString::Atof(*Args[0]));
			}
		}
	}));
}

void AGalaxy::BeginPlay()
{
	Super::BeginPlay();

	SetTimeScale(1);
	PauseTime(true);
}

void AGalaxy::ApplyPattern(const FGalaxyMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to galaxy %s"), *Id.ToString())
		return;
	}

	TSet<UCompositeRecord*> UsedStarTypes;
	for (auto& System : Data.Systems)
	{
		for (auto Star : System.Stars)
		{
			UsedStarTypes.Add(Star.Type);
		}
	}

	for (auto StarType : UsedStarTypes)
	{
		auto VanillaStarType = StarType->GetOrCreateComponent<UVanillaStarTypeProperties>();
		
		auto InstancedMesh = NewObject<UStarisInstancedStaticMesh>(this);
		InstancedMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		InstancedMesh->RegisterComponent();
		InstancedMesh->Mobility = EComponentMobility::Static;
		InstancedMesh->SetStaticMesh(StarMesh);
		InstancedMesh->SetMaterial(0, VanillaStarType->MaterialInstance);
		StarMeshes.Add(StarType, InstancedMesh);
	}

	auto GraphicsSettings = GetActorOfClass<AStarisGraphicsSettings>(this);

	PlanetMeshes = NewObject<UStarisInstancedStaticMesh>(this);
	PlanetMeshes->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PlanetMeshes->RegisterComponent();
	PlanetMeshes->Mobility = EComponentMobility::Static;
	PlanetMeshes->InstanceStartCullDistance = 1000 * GraphicsSettings->GetPlanetRenderDistanceScale();
	PlanetMeshes->InstanceEndCullDistance = 1500 * GraphicsSettings->GetPlanetRenderDistanceScale();
	PlanetMeshes->SetStaticMesh(PlanetMesh);
	PlanetMeshes->SetMaterial(0, PlanetMaterial);
	
	Id = Data.Id;
	Seed = Data.Seed;
	RegisterObjectById(Id, this);

	Systems.Reserve(Data.Systems.Num());
	for (auto& SystemData : Data.Systems)
	{
		auto System = NewObject<USystem>(this, SystemClass);
		System->InitCelestialEntity(this);
		System->ApplyPattern(SystemData);

		Systems.Add(System);
	}
}

void AGalaxy::SetTimeScale(float NewTimeScale)
{
	if (NewTimeScale == TimeScale) return;
	
	GetWorld()->GetTimerManager().ClearTimer(DayTickHandle);

	TimeScale = NewTimeScale;
	bPaused = TimeScale == 0;
	if (TimeScale > 0)
	{
		if (!bIsGameStarted)
		{
			bIsGameStarted = true;
			OnGameStarted.Broadcast(false);
		}
		
		GetWorld()->GetTimerManager().SetTimer(DayTickHandle, FTimerDelegate::CreateUObject(this, &AGalaxy::DayPassed), 1 / TimeScale, true);

		OnTimeStateChanged.Broadcast(TimeScale, bPaused);
		OnTimeStateChanged_K2.Broadcast(TimeScale, bPaused);
	}
}

void AGalaxy::PauseTime(bool Pause)
{
	if (bPaused == Pause) return;
	
	bPaused = Pause;
	if (Pause)
	{
		GetWorld()->GetTimerManager().PauseTimer(DayTickHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().UnPauseTimer(DayTickHandle);
	}

	OnTimeStateChanged.Broadcast(TimeScale, bPaused);
	OnTimeStateChanged_K2.Broadcast(TimeScale, bPaused);
}

bool AGalaxy::TogglePauseTime()
{
	auto& TimerManager = GetWorld()->GetTimerManager();
	if (TimerManager.TimerExists(DayTickHandle))
	{
		if (TimerManager.IsTimerPaused(DayTickHandle))
		{
			TimerManager.UnPauseTimer(DayTickHandle);
			bPaused = false;

			OnTimeStateChanged.Broadcast(TimeScale, bPaused);
			OnTimeStateChanged_K2.Broadcast(TimeScale, bPaused);
		}
		else
		{
			TimerManager.PauseTimer(DayTickHandle);
			bPaused = true;

			OnTimeStateChanged.Broadcast(TimeScale, bPaused);
			OnTimeStateChanged_K2.Broadcast(TimeScale, bPaused);
		}
	}
	else
	{
		SetTimeScale(1);
	}

	return bPaused;
}

AGalaxy* AGalaxy::GetGalaxy(UObject* WorldContextObject)
{
	return Cast<AGalaxy>(UGameplayStatics::GetActorOfClass(WorldContextObject ? WorldContextObject : GEngine->GetWorld(), AGalaxy::StaticClass()));
}

const TArray<USystem*>& AGalaxy::GetSystems() const
{
	return Systems;
}

int32 AGalaxy::GenerateGalaxySeed()
{
	return FMath::Rand();
}

UMeshInstanceRef* AGalaxy::CreateStarInstance(UCompositeRecord* StarType, const FTransform& StarTransform)
{
	return StarMeshes[StarType]->AddObjectInstance(StarTransform);
}

UMeshInstanceRef* AGalaxy::CreatePlanetInstance(const FTransform& PlanetTransform)
{
	return PlanetMeshes->AddObjectInstance(PlanetTransform);
}

void AGalaxy::SetStarsScale(float Scale)
{
	if (Scale == StarSizeBiasCached) return;
	StarSizeBiasCached = Scale;

	//for (int32 i = 0; i < Systems.Num(); i++)
	//{
	//	auto& Stars = Systems[i]->GetStars();
	//	for (int32 j = 0; j < Stars.Num(); j++)
	//	{
	//		Stars[j]->MeshInstance->GetSourceWeak()->settrans
	//	}
	//}

	UMaterialParameterCollectionInstance* Instance = GetWorld()->GetParameterCollectionInstance(MaterialParameterCollection);
	Instance->SetScalarParameterValue("StarScale", Scale);
}

UObject* AGalaxy::GetObjectById(const FName& ObjectId) const
{
	return IdDatabase.FindRef(ObjectId).Get();
}

void AGalaxy::RegisterObjectById(const FName& ObjectId, UObject* Object)
{
	IdDatabase.Add(ObjectId, Object);
}

APointLight* AGalaxy::RequireStarLight()
{
	if (FreeStarLightPool.IsEmpty())
	{
		if (EnsureGalaxySettingsManager())
		{
			if (auto VanillaGalaxySettings = GalaxySettingsManager->GetSettings<UVanillaGalaxySettings>())
			{
				auto NewStarLight = GetWorld()->SpawnActor<APointLight>();
				NewStarLight->SetMobility(EComponentMobility::Movable);
				NewStarLight->PointLightComponent->SetIntensity(6);
				NewStarLight->PointLightComponent->SetAttenuationRadius(VanillaGalaxySettings->PlanetOrbitRange.Max);
				NewStarLight->PointLightComponent->SetUseInverseSquaredFalloff(false);
				NewStarLight->PointLightComponent->SetLightFalloffExponent(2);
				NewStarLight->PointLightComponent->SetInverseExposureBlend(1);
		
				FreeStarLightPool.Add(NewStarLight);
			}
		}
	}

	verify(FreeStarLightPool.Num() > 0);

	auto TakenStarLight = FreeStarLightPool[0];
	FreeStarLightPool.RemoveAt(0);
	UsedStarLightPool.Add(TakenStarLight);

	TakenStarLight->PointLightComponent->SetVisibility(true);

	return TakenStarLight;
}

void AGalaxy::FreeStarLight(APointLight* StarLight)
{
	if (UsedStarLightPool.Contains(StarLight))
	{
		UsedStarLightPool.Remove(StarLight);
		FreeStarLightPool.Add(StarLight);

		StarLight->PointLightComponent->SetVisibility(false);
	}
}

void AGalaxy::DayPassed()
{
	for (auto& Entity : AddDayUpdateCelestialEntities)
	{
		DayUpdateCelestialEntities.Add(Entity);
	}
	for (auto& Entity : RemoveDayUpdateCelestialEntities)
	{
		DayUpdateCelestialEntities.Remove(Entity);
	}
	for (auto& Entity : AddMonthUpdateCelestialEntities)
	{
		MonthUpdateCelestialEntities.Add(Entity);
	}
	for (auto& Entity : RemoveMonthUpdateCelestialEntities)
	{
		MonthUpdateCelestialEntities.Remove(Entity);
	}
	for (auto& Entity : AddYearUpdateCelestialEntities)
	{
		YearUpdateCelestialEntities.Add(Entity);
	}
	for (auto& Entity : RemoveYearUpdateCelestialEntities)
	{
		YearUpdateCelestialEntities.Remove(Entity);
	}

	DaysCounter++;

	OnDayPassed.Broadcast();
	OnDayPassed_K2.Broadcast();
	
	//GEngine->AddOnScreenDebugMessage(0, 60, FColor::White, FString::Printf(TEXT("Day %i passed"), DaysCounter));
	
	for (int i = DayUpdateCelestialEntities.Num() - 1; i >= 0; --i)
	{
		DayUpdateCelestialEntities[i]->OnDayPassed(DaysCounter);
	}

	if (DaysCounter > 0)
	{
		if (DaysCounter % 30 == 0)
		{
			auto GameMode = GetActorOfClass<AStarisGameMode>(this);
			for (auto Empire : GameMode->GetEmpires())
			{
				Empire->MonthPassed();
			}
			
			OnMonthPassed.Broadcast();
			OnMonthPassed_K2.Broadcast();
            		
            //GEngine->AddOnScreenDebugMessage(1, 60, FColor::White, FString::Printf(TEXT("Month %i passed"), DaysCounter / 30));
        
            for (int i = DayUpdateCelestialEntities.Num() - 1; i >= 0; --i)
            {
            	DayUpdateCelestialEntities[i]->OnMonthPassed(DaysCounter / 30);
            }
		}

		if (DaysCounter % 360 == 0)
		{
			OnYearPassed.Broadcast();
			OnYearPassed_K2.Broadcast();
			
			//GEngine->AddOnScreenDebugMessage(2, 60, FColor::White, FString::Printf(TEXT("Year %i passed"), DaysCounter / (30 * 12)));
		}
	}

	OnDateUpdated.Broadcast(GetDate(), GetMonth(), GetYear());
	OnDateUpdated_K2.Broadcast(GetDate(), GetMonth(), GetYear());
}

void AGalaxy::Generate()
{
	if (EnsureGalaxySettingsManager())
	{
		if (auto StarisGameInstance = Cast<UStarisGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			auto generator = NewObject<UVanillaGalaxyGenerator>();
			generator->GameInstance = StarisGameInstance;
		
			FGalaxyMetaData data;
			generator->GenerateGalaxy(data, 1, GalaxySettingsManager, NewObject<UCompositeRecord>());

			ApplyPattern(data);
		}
	}
}

void AGalaxy::Start()
{
	SetTimeScale(1);
}

bool AGalaxy::EnsureGalaxySettingsManager()
{
	if (!GalaxySettingsManager)
	{
		GalaxySettingsManager = GetActorOfClass<AGalaxySettingsManager>(this);
	}

	return GalaxySettingsManager->IsValidLowLevel();
}
