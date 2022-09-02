// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/InventoryComponent.h"

#include "Car/CarPlayerController.h"
#include "Weapon/Items/BombItem.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
	SetIsReplicatedByDefault(true);
}


void UInventoryComponent::OnRep_CurrentItem()
{
	ACarPlayerController* PC = Cast<ACarPlayerController>(GetOwner()->GetInstigatorController());
	PC->OnItemChange();
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
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::UseCurrentItem_Implementation()
{
	if(!GetOwner()->HasAuthority()) return;
	if (!IsValid(CurrentItem)) return;
	CurrentItem->UseItem();
	CurrentItem->Destroy();
	CurrentItem = nullptr;
	//ACarPlayerController* PC = Cast<ACarPlayerController>(GetOwner()->GetInstigatorController());
	//PC->OnItemChange();
	//CurrentItem->Destroy();
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

	/*if(GetOwner()->HasAuthority()) return;
	ACarPlayerController* PC = Cast<ACarPlayerController>(GetOwner()->GetInstigatorController());
	PC->OnItemChange();*/
}

void UInventoryComponent::SwitchBlend()
{
	BlendType = static_cast<EBlendType>((static_cast<int>(BlendType) + 1) % 3);
	ServerSetBlendType(static_cast<int>(BlendType));
}

void UInventoryComponent::ServerSetBlendType_Implementation(int type)
{
	BlendType = static_cast<EBlendType>(static_cast<int>(type));
}

void UInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(UInventoryComponent, CurrentItem, COND_OwnerOnly);
}
