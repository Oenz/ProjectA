// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Projectile.h"

#include "Car/CarPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Weapon/ItemBase.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	CollisionComponent->InitSphereRadius(60);
	CollisionComponent->SetCollisionProfileName("Projectile");
	CollisionComponent->BodyInstance.bNotifyRigidBodyCollision = true;

	RootComponent = CollisionComponent;
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComponent->SetupAttachment(RootComponent);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(
		TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
	ProjectileMovementComponent->InitialSpeed = 10000.0f;
	ProjectileMovementComponent->MaxSpeed = 10000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	CollisionComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
}


// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(5);
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                        FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Hit"));
	
	if(AItemBase* item = Cast<AItemBase>(OtherActor))
	{
		item->HitProjectile(GetOwner());
	}

	if(ACarPawn* player = Cast<ACarPawn>(OtherActor))
	{
		player->SetStan(StanTime);
	}
	Destroy();
}

void AProjectile::SpeedUp()
{
	ProjectileMovementComponent->InitialSpeed *= 2;
	ProjectileMovementComponent->MaxSpeed *= 2;
}

void AProjectile::RangeUp()
{
	CollisionComponent->SetRelativeScale3D(CollisionComponent->GetRelativeScale3D() * 2);
	MeshComponent->SetRelativeScale3D(MeshComponent->GetRelativeScale3D() * 2);
}

void AProjectile::PowerUp()
{
	StanTime *= 2;
}
