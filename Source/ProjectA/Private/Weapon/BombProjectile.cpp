// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/BombProjectile.h"
#include "DrawDebugHelpers.h"
#include "Car/CarPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Particles/ParticleSystemComponent.h"


void ABombProjectile::BeginPlay()
{
	Super::BeginPlay();


	GetWorldTimerManager().SetTimer(Timer, this, &ABombProjectile::Explosion, lifetime, false);
}

void ABombProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                            FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	if(!HasAuthority()) return;
	Explosion();
}

void ABombProjectile::Explosion()
{
	UE_LOG(LogTemp, Warning, TEXT("Explosion"));
	//DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 50, FColor::Purple, true, 3);

	CollisionComponent->SetCollisionProfileName("NoCollision");
	GetWorld()->GetTimerManager().ClearTimer(Timer);
	/*if(ExplosionParticle != nullptr)
	{
		UParticleSystemComponent* _PSC = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticle,GetActorLocation(), FRotator::ZeroRotator, FVector::One() *	ParticleScale, true);
		_PSC->SetIsReplicated(true);
	}*/
	ParticleScale = ExplosionRadius;
	TArray<FHitResult> OutHits;
	// check if something got hit in the sweep
	FCollisionShape MyColSphere = FCollisionShape::MakeSphere(ExplosionRadius);
	bool isHit = GetWorld()->SweepMultiByChannel(OutHits, GetActorLocation(), GetActorLocation(), FQuat::Identity, ECC_WorldStatic, MyColSphere);

	if (isHit)
	{
		// loop through TArray
		for (auto& hit : OutHits)
		{
			AActor* HitObject = hit.GetActor();
			if(HitObject == nullptr) break;
			/*if (GEngine) 
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Hit Result: %s"), *HitObject->GetName()));
			}*/

			if(ACarPawn* Player =  Cast<ACarPawn>(HitObject))
			{
				if(HitObject == GetOwner()) break;
				Player->SetStan(StanTime);
			}
		}
	}
	SetLifeSpan(lifetime);
}

void ABombProjectile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABombProjectile, ParticleScale);
}