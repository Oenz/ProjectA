// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileLauncher.h"

#include "Car/InventoryComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/Projectile.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AProjectileLauncher::AProjectileLauncher()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.1f;
	bReplicates = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//UStaticMesh* LoadBody = LoadObject<UStaticMesh>(
	//	NULL, TEXT("/Game/Assets/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid"), NULL, LOAD_None, NULL);
	//Mesh->SetStaticMesh(LoadBody);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	DefaultProjectile = TSoftClassPtr<AProjectile>(
		FSoftObjectPath(TEXT("/Game/Blueprint/BP_Projectile.BP_Projectile_C"))).LoadSynchronous();
	BombProjectile = TSoftClassPtr<AProjectile>(
		FSoftObjectPath(TEXT("/Game/Blueprint/BP_BombProjectile.BP_BombProjectile_C"))).LoadSynchronous();
}

// Called when the game starts or when spawned
void AProjectileLauncher::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AProjectileLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetCursorFirePosition();
	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetName());
}

void AProjectileLauncher::FireProjectile(TSubclassOf<AProjectile> ProjectileObject, FVector CursourPosition)
{
	if (!HasAuthority()) return;
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("FirePrj")));

	FVector SpawnPos = GetActorLocation();
	UInventoryComponent* inv = Cast<UInventoryComponent>(
		GetOwner()->GetComponentByClass(UInventoryComponent::StaticClass()));
	FTransform spawnTfm(GetActorRotation(), SpawnPos);
	AProjectile* SpawnProjectile = Cast<AProjectile>(
		UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ProjectileObject, spawnTfm));
	if (SpawnProjectile)
	{
		//Projectile->GetInstigator();
		SpawnProjectile->SetOwner(GetOwner());
		if (inv->UpgradeType == EUpgradeType::Speed) SpawnProjectile->SpeedUp();
		UGameplayStatics::FinishSpawningActor(SpawnProjectile, spawnTfm);
	}

	FVector FireDirection = CursourPosition - SpawnPos;
	FireDirection.Normalize();
	SpawnProjectile->FireInDirection(FireDirection);

	switch (inv->UpgradeType)
	{
		case EUpgradeType::Range:
			SpawnProjectile->RangeUp();
			break;
		case EUpgradeType::Power:
			SpawnProjectile->PowerUp();
			break;
		default:
			break;
	}
}

FVector AProjectileLauncher::GetCursorFirePosition()
{
	if (HasAuthority()) return FVector::Zero();
	FVector mouseLocation, mouseDirection;
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);
	return mouseLocation + mouseDirection * 100000;
}

void AProjectileLauncher::StartDefaultFire()
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("ServerFire")));
	float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->TimeSeconds, 0.0f);
	GetWorldTimerManager().SetTimer(DefaultFireTimer, [&]()
	{
		ServerFireProjectile(GetCursorFirePosition(), false);
		LastFireTime = GetWorld()->TimeSeconds;
	}, FireRate, true, FirstDelay);
}

void AProjectileLauncher::StopDefaultFire()
{
	GetWorldTimerManager().ClearTimer(DefaultFireTimer);
}

void AProjectileLauncher::ClientBombFire_Implementation()
{
	ServerFireProjectile(GetCursorFirePosition(), true);
}

void AProjectileLauncher::ServerFireProjectile_Implementation(FVector TargetPos, bool isBomb)
{
	FireProjectile(isBomb ? BombProjectile : DefaultProjectile, TargetPos);
}
