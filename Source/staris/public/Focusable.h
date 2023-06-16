// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Focusable.generated.h"

class UToolTip;
// This class does not need to be modified.
UINTERFACE()
class UFocusable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class STARIS_API IFocusable
{
	GENERATED_BODY()

public:
	virtual UToolTip* CreateToolTip() = 0;
	virtual void SetupToolTip(UToolTip* ToolTip) = 0;
};
