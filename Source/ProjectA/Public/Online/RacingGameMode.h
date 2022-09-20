// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RacingGameState.h"
#include "GameFramework/GameMode.h"
#include "RacingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API ARacingGameMode : public AGameMode
{
	GENERATED_BODY()

	ARacingGameMode();
	
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	
    void ReadyForStart();

	virtual void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	virtual void Logout(AController* Exiting) override;

	virtual void HandleMatchIsWaitingToStart() override;

	virtual void HandleMatchHasStarted() override;

	class ARacingGameState* GameState;

public:
	TSubclassOf<APawn> RacingPawnClass;

	void StartTimer();

	void StopTimer();

	void RaceStart();

	bool bRaceStarted = false;
	
	FTimerHandle TimerHandle;

	void CountTimer();

	int32 WaitingForStartTime = 15;

	int32 WaitingForRaceTime = 7;

	void CheckAllPlayersReady();
};
