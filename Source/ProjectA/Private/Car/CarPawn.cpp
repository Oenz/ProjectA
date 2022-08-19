// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/CarPawn.h"
#include "Net/UnrealNetwork.h"
#include "Car/CarMovementComponent.h"
#include "Car/CarMovementReplicatorComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
ACarPawn::ACarPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(false);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	MovementComponent = CreateDefaultSubobject<UCarMovementComponent>(TEXT("MovementComponent"));
	MovementReplicatorComponent = CreateDefaultSubobject<UCarMovementReplicatorComponent>(TEXT("MovementReplicator"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	
	RootComponent = DefaultSceneRoot;
	BodyMesh->SetupAttachment(RootComponent);
	CameraComponent->SetupAttachment(BodyMesh);

	USkeletalMesh* LoadBody = LoadObject<USkeletalMesh>(NULL, TEXT("/Game/Assets/VehicleVarietyPack/Skeletons/SK_SportsCar.SK_SportsCar"), NULL, LOAD_None, NULL);
	BodyMesh->SetSkeletalMesh(LoadBody);

	CameraComponent->SetRelativeLocation(FVector(-332,0,311));
	CameraComponent->SetRelativeRotation(FRotator(-10, 0, 0));
}

// Called when the game starts or when spawned
void ACarPawn::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		NetUpdateFrequency = 1;
	}
}

FString GetEnumText(ENetRole Role)
{
	switch (Role)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "ERROR";
	}
}

// Called every frame
void ACarPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawDebugString(GetWorld(), FVector(0, 0, 100), UEnum::GetValueAsString(GetLocalRole()), this, FColor::White, DeltaTime);
}

// Called to bind functionality to input
void ACarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACarPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACarPawn::MoveRight);

}

void ACarPawn::MoveForward(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetThrottle(Value);
}

void ACarPawn::MoveRight(float Value)
{
	if (MovementComponent == nullptr) return;

	MovementComponent->SetSteeringThrow(Value);
}
