// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/RaceGameMode.h"
#include "Car/CarPawn.h"

ARaceGameMode::ARaceGameMode()
{
	DefaultPawnClass = ACarPawn::StaticClass();
}
