// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/RacingGameState.h"

#include <string>

#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

ARacingGameState::ARacingGameState()
{
	
}

void ARacingGameState::OnRep_TimeChanged()
{
	if (GEngine) 
	{
		FString str = FString::FromInt( RemainingTime);
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, str);
	}
	
	if(!TimeChangedDelegate.IsBound()) return;
	TimeChangedDelegate.Broadcast();
}

void ARacingGameState::OnRep_FinishPlayerRanking()
{
	if (GEngine) 
	{
		FString str = FString::Printf(TEXT("Goal"));// FinishPlayerRanking[0]->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, str);
	}
}

void ARacingGameState::GetLifetimeReplicatedProps( TArray< FLifetimeProperty > & OutLifetimeProps ) const
{
	Super::GetLifetimeReplicatedProps( OutLifetimeProps );
	DOREPLIFETIME(ARacingGameState, RemainingTime );
	DOREPLIFETIME(ARacingGameState, FinishPlayerRanking);
}
