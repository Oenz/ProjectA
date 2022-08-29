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

	UPROPERTY(Transient, Replicated, ReplicatedUsing=OnRep_FinishPlayerRanking, BlueprintReadOnly)
	TArray<APlayerState*> FinishPlayerRanking;

	UFUNCTION()
	void OnRep_TimeChanged();

	UFUNCTION()
	void OnRep_FinishPlayerRanking();

	FTimeChangedDelegate TimeChangedDelegate;

	void SetTime(int32 time) {RemainingTime = time;};
};
