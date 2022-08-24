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


	TArray<AActor*> AttachedActors; 
	GetOwner()->GetAttachedActors(AttachedActors);
	AProjectileLauncher* ProjectileLauncher = nullptr;
	for (AActor* Attached : AttachedActors)
	{
		ProjectileLauncher = Cast<AProjectileLauncher>(Attached);
		if (IsValid(ProjectileLauncher)) break;
	}
	if(!IsValid(ProjectileLauncher)) return;
	ProjectileLauncher->FireProjectile(BombProjectile);
	UE_LOG(LogTemp, Warning, TEXT("USE BOMB"));
}
