// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/CarPlayerState.h"

#include "Car/CarPawn.h"
#include "Gimic/GoalArea.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Online/RacingGameState.h"
#include "Online/RacingGameMode.h"

bool ACarPlayerState::SetCheckPoint(int num)
{
	if (!HasAuthority()) return false;
	if (CurrentCheckPoint + 1 != num) return false;
	CurrentCheckPoint = num;

	if (CurrentCheckPoint >= goals.Num())
	{
		isGoal = true;
		GetWorld()->GetGameState<ARacingGameState>()->PlayerFinish(this);
		if(ACarPawn* car = GetPawn<ACarPawn>())
		{
			car->isGoal = true;
			car->freezeMove = true;
			
		}
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

void ACarPlayerState::ServerPlayerReady_Implementation(bool ready)
{
	isReady = ready;
	ARacingGameMode* GameState = Cast<ARacingGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameState->CheckAllPlayersReady();
}


void ACarPlayerState::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoalArea::StaticClass(), actors);

	goals.Reset();
	
	for (AActor* actor : actors)
	{
		if(AGoalArea* goal = CastChecked<AGoalArea>(actor))
		{
			goals.Add(goal);
			//goals.EmplaceAt(goal->CheckPointNumber -1, goal);
		}
	}
	goals.Sort([](const AGoalArea& A, const AGoalArea& B) {
	return A.CheckPointNumber < B.CheckPointNumber;
	});
	
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
