// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/StarisController.h"

#include "Empire/Empire.h"


void IStarisController::NotifyControllerRemoved(UEmpire* Empire)
{
	Empire->ControllerAssigned(this);
}

void IStarisController::NotifyControllerAssigned(UEmpire* Empire)
{
	Empire->ControllerRemoved(this);
}
