// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Car/InventoryComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class PROJECTA_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:
	AItemBase();

	virtual void UseItem();

	void HitProjectile(AActor* Player);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(ReplicatedUsing=OnRep_Equiped, EditAnywhere, BlueprintReadOnly)
	bool bEquiped = false;

	UFUNCTION()
	void OnRep_Equiped();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnEquiped();
	
	
//	UFUNCTION()
//	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
