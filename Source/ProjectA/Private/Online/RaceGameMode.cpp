// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/RaceGameMode.h"
#include "Car/CarPawn.h"
#include "Car/CarPlayerController.h"


ARaceGameMode::ARaceGameMode(const FObjectInitializer& ObjectInitializer)
{
	DefaultPawnClass = ACarPawn::StaticClass();
	PlayerControllerClass = ACarPlayerController::StaticClass();

	
}

void ARaceGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	//SetMatch
	//bDelayedStart = true;
}
