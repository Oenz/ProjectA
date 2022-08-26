// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/ItemBase.h"
#include "BombItem.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API ABombItem : public AItemBase
{
	GENERATED_BODY()

	ABombItem();
	
public:
	virtual void UseItem(EBlendType type) override;

private:
	TSubclassOf<class AProjectile> BombProjectile;
};
