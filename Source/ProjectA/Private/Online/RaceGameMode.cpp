// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/RaceGameMode.h"
#include "Car/CarPawn.h"
#include "Car/CarPlayerController.h"


ARaceGameMode::ARaceGameMode()
{
	DefaultPawnClass = ACarPawn::StaticClass();
	PlayerControllerClass = ACarPlayerController::StaticClass();

	
}

void ARaceGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}
