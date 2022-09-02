// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/RacingGameMode.h"

#include "Car/CarPawn.h"
#include "Car/CarPlayerController.h"
#include "Car/CarSpectatorPawn.h"
#include "Catch2/catch.hpp"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Online/CarPlayerState.h"
#include "Online/RacingGameState.h"

ARacingGameMode::ARacingGameMode()
{
	DefaultPawnClass = nullptr;
	RacingPawnClass = TSoftClassPtr<ACarPawn>(
		FSoftObjectPath(TEXT("/Game/Blueprint/BP_Car.BP_Car_C"))).LoadSynchronous();

	
	PlayerControllerClass = ACarPlayerController::StaticClass();
	PlayerStateClass = ACarPlayerState::StaticClass();
	SpectatorClass = ACarSpectatorPawn::StaticClass();
	GameStateClass = ARacingGameState::StaticClass();

}

void ARacingGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bDelayedStart = true;

	GameState = GetGameState<ARacingGameState>();

}

void ARacingGameMode::ReadyForStart()
{
	GameState->SetTime(WaitingForRaceTime);
	
	TArray<AActor*> PlayerStarts;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(),PlayerStarts);
	
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		FVector SpawnPos = PlayerStarts[Iterator.GetIndex()]->GetActorLocation();
		APlayerController* PlayerController = Cast<APlayerController>(*Iterator);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerController->GetName());
		FActorSpawnParameters SpawnPram;
		SpawnPram.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(RacingPawnClass, SpawnPos, FRotator::ZeroRotator,SpawnPram);

		if(ACarPlayerController* CarPlayerController = Cast<ACarPlayerController>(PlayerController))
		{
			CarPlayerController->CarPossessPawn(SpawnedPawn);
		}
	}
}

void ARacingGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);
}

void ARacingGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (GetMatchState() == MatchState::WaitingToStart && NumPlayers == 1)
	{
		GameState = GetGameState<ARacingGameState>();
		GameState->SetTime(WaitingForStartTime);
		StartTimer();
	}
}

void ARacingGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
}

void ARacingGameMode::HandleMatchIsWaitingToStart()
{
	Super::HandleMatchIsWaitingToStart();
}

void ARacingGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	ReadyForStart();
	//FTimerHandle timer;
	//GetWorld()->GetTimerManager().SetTimer(timer,this, &ARacingGameMode::ReadyForStart ,3.0f, false);
}

void ARacingGameMode::StartTimer()
{
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ARacingGameMode::CountTimer, 1, true);
}

void ARacingGameMode::RaceStart()
{
	GameState->SetTime(30);
	bRaceStarted = true;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		ACarPlayerController* PlayerController = Cast<ACarPlayerController>(*Iterator);
		PlayerController->ClientRaceStart();
		if(ACarPawn* player = Cast<ACarPawn>(PlayerController->GetPawn()))
		{
			player->freezeMove = false;
		}
		
	}
	GetGameState<ARacingGameState>()->RaceStart();
	//PC->ClientGameStart();
}

void ARacingGameMode::CountTimer()
{
	if(bRaceStarted) return;
	GameState->RemainingTime--;

	if(GameState->RemainingTime != 0) return;
	
	if(GetMatchState() == MatchState::WaitingToStart)
	{
		StartMatch();
	}
	else if(GetMatchState() == MatchState::InProgress)
	{
		RaceStart();
	}
}




