// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Items/BombItem.h"

#include "Weapon/BombProjectile.h"
#include "Weapon/Projectile.h"
#include "Weapon/ProjectileLauncher.h"

ABombItem::ABombItem()
{
	BombProjectile = TSoftClassPtr<AProjectile>(
		FSoftObjectPath(TEXT("/Game/Blueprint/BP_BombProjectile.BP_BombProjectile_C"))).LoadSynchronous();

}

void ABombItem::UseItem(EBlendType type)
{
	Super::UseItem(type);
	UE_LOG(LogTemp, Warning, TEXT("USE"));
	TArray<AActor*> AttachedActors;
	GetOwner()->GetAttachedActors(AttachedActors);
	AProjectileLauncher* ProjectileLauncher = nullptr;
	for (AActor* Attached : AttachedActors)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *Attached->GetName());
		if (AProjectileLauncher* temp = Cast<AProjectileLauncher>(Attached))
		{
			ProjectileLauncher = temp;
			break;
		}
		
	}
	if(ProjectileLauncher == nullptr) return;
	AProjectile* Projectile = ProjectileLauncher->FireProjectile(BombProjectile);
	if(Projectile == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BombProjectile NULL"));
		return;
	}
	
	switch (type)
	{
	case EBlendType::Speed:
		Projectile->SpeedUp();
		break;
	case EBlendType::Range:
		Projectile->RangeUp();
		break;
	case EBlendType::Power:
		Projectile->PowerUp();
		break;
	default:
		break;
	}
	Destroy();
}


