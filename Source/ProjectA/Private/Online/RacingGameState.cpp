// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/RacingGameState.h"

#include <string>

#include "Car/CarPawn.h"
#include "Car/CarPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Online/CarPlayerState.h"

class ACarPawn;

ARacingGameState::ARacingGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;
}

void ARacingGameState::PlayerFinish(APlayerState* Player)
{
	if(!HasAuthority()) return;
	ACarPlayerController* PlayerController = Cast<ACarPlayerController>(Player->GetPlayerController());
	PlayerController->ClientRaceEnd();
	FinishPlayerRanking.Add(Player);

	PlayerList.Remove(Cast<ACarPlayerState>(Player));


	bool allplayergoal = true;
	for(APlayerState* ps : PlayerArray)
	{
		if(ACarPlayerState* cps = Cast<ACarPlayerState>(ps))
		{
			if(cps->isGoal)
			{
				if(ACarPawn* car = cps->GetPawn<ACarPawn>())
				{
					car->ClientViewTargetChange();
				}
			}
			else
			{
				allplayergoal = false;
			}
		}
	}
	if(!allplayergoal) return;
	FTimerHandle goal;
	GetWorld()->GetTimerManager().SetTimer(goal, [&]()
	{
		UGameplayStatics::OpenLevel(this, GetWorld()->GetFName(), false);
	}, 5, true);
	
}

void ARacingGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
	
	if(!HasAuthority()) return;

	if(PlayerList.Num() <= 0) return;
	PlayerList.Sort([](const ACarPlayerState& A, const ACarPlayerState& B) {
	return A.GoalDistance < B.GoalDistance;
	});
	
	PlayerRanking = FinishPlayerRanking;
	PlayerRanking.Append(PlayerList);

	
}

void ARacingGameState::RaceStart()
{
	PlayerList.Reset();
	for(APlayerState* PlayerState : PlayerArray)
	{
		ACarPlayerState* RacingPlayerState = Cast<ACarPlayerState>(PlayerState);
		PlayerList.Add(RacingPlayerState);
	}
}

void ARacingGameState::OnRep_TimeChanged()
{
	/*if (GEngine) 
	{
		FString str = FString::FromInt( RemainingTime);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, str);
	}*/
	
	if(!TimeChangedDelegate.IsBound()) return;
	//TimeChangedDelegate.Broadcast();
}

void ARacingGameState::OnRep_FinishPlayerRanking()
{
	/*if (GEngine) 
	{
		FString str = FString::Printf(TEXT("Goal"));// FinishPlayerRanking[0]->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, str);
	}*/
}

void ARacingGameState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME(ARacingGameState, RemainingTime );
}
