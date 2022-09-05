// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/Projectile.h"
#include "BombProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API ABombProjectile : public AProjectile
{
	GENERATED_BODY()
	
public:
	FTimerHandle _timer;
	
	virtual void BeginPlay() override;
	
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void Explosion();
	
	virtual void RangeUp() override { ExplosionRadius *= 2;};

	virtual void PowerUp() override {StanTime *= 2;}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionRadius = 50000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float lifetime = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnExplosion)
	float ParticleScale = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StanTime = 7;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnExplosion();

};
