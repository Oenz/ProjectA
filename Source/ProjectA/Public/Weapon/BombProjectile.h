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
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
};