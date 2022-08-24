// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Items/BombItem.h"

#include "Weapon/Projectile.h"
#include "Weapon/ProjectileLauncher.h"

ABombItem::ABombItem()
{
	BombProjectile = TSoftClassPtr<AProjectile>(
		FSoftObjectPath(TEXT("/Game/Blueprint/BP_BombProjectile.BP_BombProjectile_C"))).LoadSynchronous();

}

void ABombItem::UseItem()
{
	Super::UseItem();
	UE_LOG(LogTemp, Warning, TEXT("USE"));
	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);
	for (AActor* Attached : AttachedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Attached->GetName());
		if (AProjectileLauncher* ProjectileLauncher = Cast<AProjectileLauncher>(Attached))
		{
			ProjectileLauncher->FireProjectile(BombProjectile);
			UE_LOG(LogTemp, Warning, TEXT("BOMB"));
			break;
		}
		
	}
	Destroy();
}
