// Fill out your copyright notice in the Description page of Project Settings.

#include "Universe/Galaxy.h"

#include "StarisStatics.h"
#include "Game/StarisGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Universe/CelestialEntity.h"
#include "Universe/GalaxySettingsManager.h"
#include "Universe/System.h"
#include "Universe/VanillaGalaxyGenerator.h"

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
}

void AGalaxy::ApplyPattern(const FGalaxyMetaData& Data)
{
	if (!Id.IsNone())
	{
		UE_LOG(LogStaris, Error, TEXT("Pattern already has been applied to galaxy %s"), *Id.ToString())
		return;
	}
	
	Id = Data.Id;
	SetActorLabel(FString::Printf(TEXT("Galaxy_%s"), *Id.ToString()));
		
	Systems.Reserve(Data.Systems.Num());
	for (auto& System : Data.Systems)
	{
		auto SystemActor = GetWorld()->SpawnActor<ASystem>(SystemClass);
		SystemActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		SystemActor->InitCelestialEntity(this);
		SystemActor->ApplyPattern(System);

		Systems.Add(SystemActor);
	}
}

void AGalaxy::SetTimeScale(float NewTimeScale)
{
	if (NewTimeScale == TimeScale) return;
	
	GetWorld()->GetTimerManager().ClearTimer(DayTickHandle);

	TimeScale = NewTimeScale;
	if (TimeScale > 0)
	{
		if (!bIsGameStarted)
		{
			bIsGameStarted = true;
			GameStarted.Broadcast(false);
		}
		
		GetWorld()->GetTimerManager().SetTimer(DayTickHandle, FTimerDelegate::CreateUObject(this, &AGalaxy::DayPassed), 1 / TimeScale, true);
	}
}

void AGalaxy::PauseTime(bool Pause)
{
	if (Pause)
	{
		GetWorld()->GetTimerManager().PauseTimer(DayTickHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().UnPauseTimer(DayTickHandle);
	}
}

void AGalaxy::TogglePauseTime()
{
	if (GetWorld()->GetTimerManager().IsTimerPaused(DayTickHandle))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(DayTickHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().PauseTimer(DayTickHandle);
	}
}

AGalaxy* AGalaxy::GetGalaxy(UObject* WorldContextObject)
{
	return Cast<AGalaxy>(UGameplayStatics::GetActorOfClass(WorldContextObject ? WorldContextObject : GEngine->GetWorld(), AGalaxy::StaticClass()));
}

const TArray<ASystem*>& AGalaxy::GetSystems() const
{
	return Systems;
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
	
	GEngine->AddOnScreenDebugMessage(0, 60, FColor::Blue, FString::Printf(TEXT("Day %i passed"), DaysCounter));
	
	for (int i = DayUpdateCelestialEntities.Num() - 1; i >= 0; --i)
	{
		DayUpdateCelestialEntities[i]->OnDayPassed(DaysCounter);
	}

	if (DaysCounter > 0)
	{
		if (DaysCounter % 30 == 0)
		{
			static int MonthCounter = 0;
            		
            GEngine->AddOnScreenDebugMessage(1, 60, FColor::Blue, FString::Printf(TEXT("Month %i passed"), DaysCounter / 30));
        
            for (int i = DayUpdateCelestialEntities.Num() - 1; i >= 0; --i)
            {
            	DayUpdateCelestialEntities[i]->OnMonthPassed(DaysCounter / 30);
            }
		}

		if (DaysCounter % (30 * 12) == 0)
		{
			GEngine->AddOnScreenDebugMessage(2, 60, FColor::Blue, FString::Printf(TEXT("Year %i passed"), DaysCounter / (30 * 12)));
		}
	}

	DaysCounter++;
}

void AGalaxy::Generate()
{
	if (auto Settings = GetActorOfClass<AGalaxySettingsManager>(this))
	{
		if (auto StarisGameInstance = Cast<UStarisGameInstance>(UGameplayStatics::GetGameInstance(this)))
		{
			auto generator = NewObject<UVanillaGalaxyGenerator>();
			generator->StarTypeDatabase = StarisGameInstance->GetStarTypeDatabase();
		
			FGalaxyMetaData data;
			generator->GenerateGalaxy(data, 1, Settings);

			ApplyPattern(data);
		}
	}
}

void AGalaxy::Start()
{
	SetTimeScale(1);
}
