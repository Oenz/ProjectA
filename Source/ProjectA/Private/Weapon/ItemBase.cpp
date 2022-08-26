// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/ItemBase.h"

#include "Car/InventoryComponent.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	BoxComponent->SetCollisionProfileName("BlockAll");
	BoxComponent->BodyInstance.bNotifyRigidBodyCollision = true;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(BoxComponent);
	UStaticMesh* LoadBody = LoadObject<UStaticMesh>(NULL, TEXT("/Game/Assets/StarterContent/Shapes/Shape_TriPyramid.Shape_TriPyramid"), NULL, LOAD_None, NULL);
	MeshComponent->SetStaticMesh(LoadBody);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	BoxComponent->OnComponentHit.AddDynamic(this, &AItemBase::OnHit);

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

void AItemBase::UseItem(EBlendType type)
{
	
}

void AItemBase::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	BoxComponent->SetCollisionProfileName("NoCollision");
	BoxComponent->BodyInstance.bNotifyRigidBodyCollision = false;
	UE_LOG(LogTemp, Warning, TEXT("Hit Item"));

	if(!IsValid(OtherActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor NULL"));
		return;
	}
	AActor* Player = OtherActor->GetOwner();
	if(!IsValid(Player))
	{
		UE_LOG(LogTemp, Warning, TEXT("Owner NULL"));
		return;
	}
	UActorComponent* AC = Player->GetComponentByClass(UInventoryComponent::StaticClass());
	if(!IsValid(AC))
	{
		UE_LOG(LogTemp, Warning, TEXT("PL NULL"));
		return;
	}

	UInventoryComponent* InventoryComponent = Cast<UInventoryComponent>(AC);
		
	InventoryComponent->EquipItem(this);
}