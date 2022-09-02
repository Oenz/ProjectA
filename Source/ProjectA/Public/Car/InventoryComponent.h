// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

UENUM(BlueprintType)
enum class EBlendType : uint8
{
	Speed,
	Range,
	Power
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnRep_CurrentItem)
	class AItemBase* CurrentItem;

	UFUNCTION()
	void OnRep_CurrentItem();
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable)
	void UseCurrentItem();

	void EquipItem(class AItemBase* item);

	void SwitchBlend();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBlendType BlendType;

	UFUNCTION(Unreliable, Server)
	void ServerSetBlendType(int type);
};
