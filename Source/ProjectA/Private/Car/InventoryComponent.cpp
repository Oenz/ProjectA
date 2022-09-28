// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/InventoryComponent.h"

#include "Car/CarPlayerController.h"
#include "Weapon/Items/BombItem.h"
#include "Net/UnrealNetwork.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UInventoryComponent::OnRep_CurrentItem()
{
	ACarPlayerController* PC = Cast<ACarPlayerController>(GetOwner()->GetInstigatorController());
	PC->OnItemChanged();
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UInventoryComponent::UseCurrentItem_Implementation()
{
	if(!GetOwner()->HasAuthority()) return;
	if (!IsValid(CurrentItem)) return;
	CurrentItem->UseItem();
	CurrentItem->Destroy();
	CurrentItem = nullptr;
}

void UInventoryComponent::EquipItem(AItemBase* item)
{
	if (CurrentItem != nullptr)
	{
		CurrentItem->Destroy();
	}
	item->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	item->SetOwner(GetOwner());
	CurrentItem = item;
}

void UInventoryComponent::ChangeUpgrade()
{
	UpgradeType = static_cast<EUpgradeType>((static_cast<int>(UpgradeType) + 1) % 3);
	ServerSetUpgradeType(static_cast<int>(UpgradeType));
}

void UInventoryComponent::ServerSetUpgradeType_Implementation(int type)
{
	UpgradeType = static_cast<EUpgradeType>(static_cast<int>(type));
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, CurrentItem, COND_OwnerOnly);
}
