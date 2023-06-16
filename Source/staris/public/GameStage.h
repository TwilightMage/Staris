#pragma once

#include "GameStage.generated.h"

UENUM(BlueprintType)
enum class EGameStage : uint8
{
	Loading,
	MainMenu,
	Playing,
	Unloading
};