// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimic/GoalArea.h"

#include "Car/CarPlayerController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Online/RacingGameState.h"

// Sets default values
AGoalArea::AGoalArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxCollider->SetCollisionProfileName("OverlapAll");
	BoxCollider->bMultiBodyOverlap = true;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGoalArea::OnOverlapBegin);
}

void AGoalArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{
	if (GEngine) 
	{
		FString str = FString::Printf(TEXT("overlap"));// FinishPlayerRanking[0]->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, str);
	}
	if (!HasAuthority()) return;

	if (!OtherActor->ActorHasTag(FName("Player"))) return;

	AController* controller = OtherActor->GetInstigatorController();
	if(!IsValid(controller)) return;
	ACarPlayerController* PlayerController = Cast<ACarPlayerController>(controller);
	if (!IsValid(PlayerController)) return;
	PlayerController->ClientRaceEnd();
	ARacingGameState* GameState = GetWorld()->GetGameState<ARacingGameState>();
	APlayerState* PlayerState = PlayerController->GetPlayerState<APlayerState>();
	GameState->FinishPlayerRanking.Add(PlayerState );
}

// Called when the game starts or when spawned
void AGoalArea::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGoalArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
