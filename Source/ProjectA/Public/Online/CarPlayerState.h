// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CarPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API ACarPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnRep_CurrentCheckPoint();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentCheckPoint)
	int CurrentCheckPoint = 0;

	UFUNCTION()
	bool SetCheckPoint(int num);

	TArray<class AGoalArea*> goals;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GoalDistance = 0;

	UPROPERTY(ReplicatedUsing=OnRep_Ranking, EditAnywhere, BlueprintReadOnly)
	int Ranking = 0;

	UFUNCTION()
	void OnRep_Ranking();
	
	void GetDistance();
};
