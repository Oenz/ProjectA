// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Items/BombItem.h"

#include "Weapon/BombProjectile.h"
#include "Weapon/Projectile.h"
#include "Weapon/ProjectileLauncher.h"

ABombItem::ABombItem()
{

}

void ABombItem::UseItem()
{
	Super::UseItem();
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
	 ProjectileLauncher->ClientBombFire();
	//AProjectile* Projectile =
	/*if(Projectile == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BombProjectile NULL"));
		return;
	}
	
	switch (type)
	{
	case EUpgradeType::Speed:
		Projectile->SpeedUp();
		break;
	case EUpgradeType::Range:
		Projectile->RangeUp();
		break;
	case EUpgradeType::Power:
		Projectile->PowerUp();
		break;
	default:
		break;
	}*/
	Destroy();
}


