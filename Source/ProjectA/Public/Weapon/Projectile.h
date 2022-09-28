// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Projectile.generated.h"

UCLASS()
class PROJECTA_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

	UFUNCTION()
	virtual void SpeedUp();

	UFUNCTION()
	virtual void RangeUp();

	UFUNCTION()
	virtual void PowerUp();

	UFUNCTION(BlueprintCallable)
	void FireInDirection(const FVector& ShootDirection);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* CollisionComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UParticleSystemComponent* HitParticle;
	
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category="Projectile")
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StanTime = 3;
};
