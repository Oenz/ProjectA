// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "RacingGameState.generated.h"

DECLARE_EVENT(ARacingGameState, FTimeChangedDelegate)

/**
 * 
 */
UCLASS()
class PROJECTA_API ARacingGameState : public AGameState
{
	GENERATED_BODY()

	ARacingGameState();
public:
	UPROPERTY(Transient, Replicated, ReplicatedUsing=OnRep_TimeChanged, BlueprintReadOnly)
	int32 RemainingTime;

	UPROPERTY(Transient, Replicated, BlueprintReadOnly)
	TArray<APlayerState*> PlayerRanking;
	
	TArray<APlayerState*> FinishPlayerRanking;
	
	TArray<class ACarPlayerState*> PlayerList;

	void PlayerFinish(class APlayerState* Player);

	virtual void Tick(float DeltaSeconds) override;

	void RaceStart();
	
	UFUNCTION()
	void OnRep_TimeChanged();

	UFUNCTION()
	void OnRep_FinishPlayerRanking();

	FTimeChangedDelegate TimeChangedDelegate;

	void SetTime(int32 time) { RemainingTime = time; };
};
