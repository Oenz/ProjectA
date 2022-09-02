// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/CarPlayerState.h"

#include "Gimic/GoalArea.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Online/RacingGameState.h"

bool ACarPlayerState::SetCheckPoint(int num)
{
	if (!HasAuthority()) return false;
	if (CurrentCheckPoint + 1 != num) return false;
	CurrentCheckPoint = num;

	if (CurrentCheckPoint >= goals.Num())
	{
		GetWorld()->GetGameState<ARacingGameState>()->PlayerFinish(this);
	}

	return true;
}


void ACarPlayerState::OnRep_Ranking()
{
	//GetPlayerController()
}

void ACarPlayerState::GetDistance()
{
	/*if (GEngine) 
	{
		FString str = FString::FromInt( (int)GoalDistance);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, str);
	}*/

	if (CurrentCheckPoint >= goals.Num())
	{
		//Debug.Log($"GOAL");
		return;
	}

	
	GoalDistance = FVector::Distance(GetPawn()->GetActorLocation(), goals[CurrentCheckPoint]->GetActorLocation());


	
	for (int i = CurrentCheckPoint; i < goals.Num() - 1; i++)
	{
		GoalDistance +=  FVector::Distance(goals[i]->GetActorLocation(), goals[i + 1]->GetActorLocation());
	}

	//Debug.Log($"To Goal Distance : {Distance}m Current : {CheckPointIndex}");

	if(!HasAuthority()) return;
	ARacingGameState* GameState = GetWorld()->GetGameState<ARacingGameState>();
	Ranking = GameState->PlayerRanking.Find(this) + 1;
}


void ACarPlayerState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalArea::StaticClass(), actors);

	
	for (AActor* actor : actors)
	{
		AGoalArea* goal = Cast<AGoalArea>(actor);
		goals.EmplaceAt(goal->CheckPointNumber -1, goal);
	}
	
	OnRep_CurrentCheckPoint();
}

void ACarPlayerState::OnRep_CurrentCheckPoint()
{
	if(HasAuthority()) return;
	if(this->GetPlayerController() == nullptr) return;
	for (AGoalArea* actor : goals)
	{
		actor->SetVisiable(actor->CheckPointNumber == CurrentCheckPoint + 1);
	}
}

void ACarPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACarPlayerState, CurrentCheckPoint);
	DOREPLIFETIME(ACarPlayerState, Ranking);
}
