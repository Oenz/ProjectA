// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/RacingGameMode.h"

#include "Car/CarPawn.h"
#include "Car/CarPlayerController.h"

ARacingGameMode::ARacingGameMode()
{
	DefaultPawnClass = nullptr;// ACarPawn::StaticClass();
	RacingPawnClass = ACarPawn::StaticClass();
	PlayerControllerClass = ACarPlayerController::StaticClass();

}

void ARacingGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bDelayedStart = true;
	

}

void ARacingGameMode::ReadyForStart()
{
	FVector SpawnPos = FVector::Zero();
	for (FConstControllerIterator It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		
		APlayerController* PlayerController = Cast<APlayerController>(*It);
		UE_LOG(LogTemp, Warning, TEXT("%s"), *PlayerController->GetName());
		APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(RacingPawnClass, SpawnPos, FRotator::ZeroRotator);
		SpawnPos += FVector(200,0,0);

		SpawnedPawn->PossessedBy(PlayerController);
	}
}

void ARacingGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId,
	FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (GetMatchState() == MatchState::WaitingToStart)
	{
		StartMatch();
		
	}
}

void ARacingGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	
	FTimerHandle _TimerHandle;	
	GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &AGameMode::StartMatch ,2, false);
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
	ReadyForStart();
}




