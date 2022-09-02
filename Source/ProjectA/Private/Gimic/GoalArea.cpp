// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimic/GoalArea.h"

#include "Car/CarPlayerController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Online/CarPlayerState.h"
#include "Online/RacingGameState.h"

// Sets default values
AGoalArea::AGoalArea()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(BoxCollider);
	MeshComponent->SetCollisionProfileName("NoCollision");
	BoxCollider->SetCollisionProfileName("OverlapAll");
	BoxCollider->bMultiBodyOverlap = true;
	BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AGoalArea::OnOverlapBegin);
}

void AGoalArea::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                               UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                               const FHitResult& SweepResult)
{


	if (!OtherActor->ActorHasTag(FName("Player"))) return;
	
	/*if (GEngine) 
	{
		FString str = FString::Printf(TEXT("overlap"));// FinishPlayerRanking[0]->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, str);
	}*/
	if (!HasAuthority()) return;

	
	ACarPlayerState* PlayerState = Cast<APawn>(OtherActor)->GetPlayerState<ACarPlayerState>();
	PlayerState->SetCheckPoint(CheckPointNumber);
	
	//GetWorld()->GetGameState<ARacingGameState>()->PlayerFinish(PlayerState);
}

void AGoalArea::SetVisiable(bool isVisiable)
{
	if(isVisiable == bVisiable) return;
	//particle
	bVisiable = isVisiable;
	MeshComponent->SetVisibility(bVisiable);
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
