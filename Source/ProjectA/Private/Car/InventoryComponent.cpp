// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/InventoryComponent.h"

#include "Weapon/Items/BombItem.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	/*CurrentItem = GetWorld()->SpawnActor<ABombItem>(GetOwner()->GetActorLocation(), GetOwner()->GetActorRotation() ); 
	CurrentItem->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	CurrentItem->SetOwner(GetOwner());*/
	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::UseCurrentItem()
{
	if(!IsValid(CurrentItem)) return;
	CurrentItem->UseItem();
	
}

void UInventoryComponent::EquipItem(AItemBase* item)
{
	item->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	item->SetOwner(GetOwner());
	CurrentItem = item;
}
