// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/CarPawn.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "Car/CarMovementComponent.h"
#include "Car/CarMovementReplicatorComponent.h"
#include "Camera/CameraComponent.h"
#include "Car/CarPlayerController.h"
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
	//USkeletalMesh* LoadBody = LoadObject<USkeletalMesh>(NULL, TEXT("/Game/Assets/VigilanteContent/Vehicles/East_Fighter_Su33/SK_East_Fighter_Su33.SK_East_Fighter_Su33"), NULL, LOAD_None, NULL);
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

	Tags.Add(FName("Player"));
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

	
	SetFreezeMove(true);
	//Cast<ACarPlayerController>(GetController())->SetHUD();
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
	PlayerInputComponent->BindAction("SwitchBlend", IE_Pressed, this, &ACarPawn::SwitchBlend);
}

void ACarPawn::MoveForward(float Value)
{
	if(freezeMove) return;
	if (MovementComponent == nullptr) return;

	MovementComponent->SetThrottle(Value);
}

void ACarPawn::MoveRight(float Value)
{
	if(freezeMove) return;
	if (MovementComponent == nullptr) return;

	MovementComponent->SetSteeringThrow(Value);
}

void ACarPawn::MoveUp(float Value)
{
	if(freezeMove) return;
	if(MovementComponent == nullptr) return;

	MovementComponent->SetPitch(Value);
}

void ACarPawn::Fire()
{
	if(freezeMove) return;
	//load Failded
	if(Projectile == nullptr || ProjectileLauncher == nullptr) return;
	ProjectileLauncher->FireProjectile( Projectile );
}

void ACarPawn::Use()
{
	if(freezeMove) return;
	InventoryComponent->UseCurrentItem();
}

void ACarPawn::SwitchBlend()
{
	if(freezeMove) return;
	InventoryComponent->SwitchBlend();
	ACarPlayerController* CPC = Cast<ACarPlayerController>(GetController());
	CPC->OnSwitchBlend();
}

void ACarPawn::Stan(float second)
{
	FTimerHandle _TimerHandle;

	SetFreezeMove(true);
	GetWorld()->GetTimerManager().SetTimer(_TimerHandle, this, &ACarPawn::EndStan,second, false);
	DrawDebugBox(GetWorld(), GetActorLocation(), FVector::One() * 100, FColor::Red, true, 5);
}

void ACarPawn::EndStan()
{
	SetFreezeMove(false);
		DrawDebugBox(GetWorld(), GetActorLocation(), FVector::One() * 100, FColor::Green, true, 1);
}

void ACarPawn::SetFreezeMove(bool isFreeze)
{
	freezeMove = isFreeze;

	if(!isFreeze) return;
	
	MovementComponent->SetThrottle(0);
	MovementComponent->SetSteeringThrow(0);
	MovementComponent->SetPitch(0);
}
