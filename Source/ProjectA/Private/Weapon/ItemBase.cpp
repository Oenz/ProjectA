// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ItemBase.h"

#include "Car/InventoryComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxComponent->SetCollisionProfileName("HitOnlyProjectile");
	BoxComponent->BodyInstance.bNotifyRigidBodyCollision = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(BoxComponent);
	//UStaticMesh* LoadBody = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Assets/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid"), NULL, LOAD_None, NULL);
	//MeshComponent->SetStaticMesh(LoadBody);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	//BoxComponent->OnComponentHit.AddDynamic(this, &AItemBase::OnHit);

}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::UseItem()
{
	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("UseItem")));
}

void AItemBase::HitProjectile(AActor* Player)
{
	if(!HasAuthority()) return;
	BoxComponent->SetCollisionProfileName("NoCollision");
	BoxComponent->BodyInstance.bNotifyRigidBodyCollision = false;

	bEquiped = true;
	
	UE_LOG(LogTemp, Warning, TEXT("Hit Item"));
	
	UActorComponent* AC = Player->GetComponentByClass(UInventoryComponent::StaticClass());
	if(!IsValid(AC))
	{
		UE_LOG(LogTemp, Warning, TEXT("PL NULL"));
		return;
	}

	UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(AC);
	
	InventoryComponent->EquipItem(this);


}

void AItemBase::OnRep_Equiped()
{
	OnEquiped();
	this->SetHidden(true);
}

void AItemBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItemBase, bEquiped);
}

