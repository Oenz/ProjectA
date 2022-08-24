// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/CarPawn.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Car/CarMovementComponent.h"
#include "Car/CarMovementReplicatorComponent.h"
#include "Camera/CameraComponent.h"
#include "Car/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Projectile.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Weapon/ProjectileLauncher.h"


// Sets default values
ACarPawn::ACarPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(false);

	BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("BOXCollider"));
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BodyMesh"));
	MovementComponent = CreateDefaultSubobject<UCarMovementComponent>(TEXT("MovementComponent"));
	MovementReplicatorComponent = CreateDefaultSubobject<UCarMovementReplicatorComponent>(TEXT("MovementReplicator"));
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	
	//RootComponent = DefaultSceneRoot;
	DefaultSceneRoot->SetupAttachment(BoxCollider);
	BodyMesh->SetupAttachment(DefaultSceneRoot);
	CameraComponent->SetupAttachment(BodyMesh);

	USkeletalMesh* LoadBody = LoadObject<USkeletalMesh>(NULL, TEXT("/Game/Assets/VehicleVarietyPack/Skeletons/SK_SportsCar.SK_SportsCar"), NULL, LOAD_None, NULL);
	BodyMesh->SetSkeletalMesh(LoadBody);

	CameraComponent->SetRelativeLocation(FVector(-332,0,311));
	CameraComponent->SetRelativeRotation(FRotator(-10, 0, 0));

	//BoxCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	//BoxCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Vehicle, ECollisionResponse::ECR_Block);
	BoxCollider->SetCollisionProfileName("BlockAll");
	//BoxCollider->SetHiddenInGame(false);
	BoxCollider->SetBoxExtent(FVector(230, 90, 80));
	BoxCollider->SetGenerateOverlapEvents(true);
	BoxCollider->BodyInstance.bNotifyRigidBodyCollision = true;

	//Projectile = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Assets/VehicleVarietyPack/Skeletons/SK_SportsCar.SK_SportsCar"), NULL, LOAD_None, NULL);
	Projectile = TSoftClassPtr<AProjectile>(FSoftObjectPath(TEXT("/Game/Blueprint/BP_Projectile.BP_Projectile_C"))).LoadSynchronous();

}

// Called when the game starts or when spawned
void ACarPawn::BeginPlay()
{
	Super::BeginPlay();
	MovementReplicatorComponent->SetMeshOffsetRoot(DefaultSceneRoot);
	if (HasAuthority())
	{
		NetUpdateFrequency = 20;
	}
	
	ProjectileLauncher = GetWorld()->SpawnActor<AProjectileLauncher>(GetActorLocation(), GetActorRotation());
	ProjectileLauncher->AttachToComponent(BoxCollider, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ProjectileLauncher->SetActorRelativeLocation(GetActorForwardVector() * 300);//
ProjectileLauncher->SetOwner(this);
	/*UBlueprint* WBP = LoadObject<UBlueprint>(NULL, TEXT("/Game/Widget/WBP_HUD.WBP_HUD"));
	TSubclassOf<UUserWidget> HUDWidget = WBP->GeneratedClass;
	UUserWidget* hud = CreateWidget<UUserWidget>(GetWorld(), HUDWidget);
	hud->AddToViewport();*/
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

	//DrawDebugString(GetWorld(), FVector(0, 0, 100), UEnum::GetValueAsString(GetLocalRole()), this, FColor::White, DeltaTime);
}

// Called to bind functionality to input
void ACarPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ACarPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACarPawn::MoveRight);
	PlayerInputComponent->BindAxis("MoveUp", this, &ACarPawn::MoveUp);
	PlayerInputComponent->BindAction("Fire", IE_Pressed,this, &ACarPawn::Fire);
	PlayerInputComponent->BindAction("Use", IE_Pressed, this, &ACarPawn::Use);

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

void ACarPawn::MoveUp(float Value)
{
	if(MovementComponent == nullptr) return;

	MovementComponent->SetPitch(Value);
}

void ACarPawn::Fire()
{
	//load Failded
	if(Projectile == nullptr || ProjectileLauncher == nullptr) return;
	ProjectileLauncher->FireProjectile( Projectile );
}

void ACarPawn::Use()
{
	InventoryComponent->UseCurrentItem();
}
