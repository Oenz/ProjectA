// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ProjectileLauncher.h"

#include "Kismet/GameplayStatics.h"
#include "Weapon/Projectile.h"

// Sets default values
AProjectileLauncher::AProjectileLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	UStaticMesh* LoadBody = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Assets/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid"), NULL, LOAD_None, NULL);
	Mesh->SetStaticMesh(LoadBody);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

	//UE_LOG(LogTemp, Warning, TEXT("%s"), *GetOwner()->GetName());

}

AProjectile* AProjectileLauncher::FireProjectile( TSubclassOf< AProjectile > ProjectileObject )
{
	FVector SpawnPos = GetActorLocation();// + GetActorForwardVector() * 100;
	AProjectile* SpawnProjectile =  GetWorld()->SpawnActor<AProjectile>(ProjectileObject, SpawnPos, GetActorRotation());
	SpawnProjectile->SetOwner(GetOwner());
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if(PlayerController == nullptr) return nullptr;
	FVector mouseLocation, mouseDirection;
	//GetWorld()->GetFirstPlayerController();
	PlayerController->DeprojectMousePositionToWorld(mouseLocation, mouseDirection);

	FVector EndPoint = mouseLocation + mouseDirection * 10000;
	FVector FireDirection = EndPoint - SpawnPos;
	FireDirection.Normalize();
	SpawnProjectile->FireInDirection(FireDirection);
	return SpawnProjectile;
}

