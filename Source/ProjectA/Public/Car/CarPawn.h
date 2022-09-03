// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Weapon/Projectile.h"
#include "CarPawn.generated.h"

UCLASS()
class PROJECTA_API ACarPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACarPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxCollider;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USceneComponent* DefaultSceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class UCarMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCarMovementReplicatorComponent* MovementReplicatorComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComponent;

	//fire
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//class UStaticMesh* Projectile;

	UPROPERTY(Replicated)
	class AProjectileLauncher* ProjectileLauncher;
	

	
	void Stan(float second);
	void EndStan();
	

	UPROPERTY(ReplicatedUsing=OnRep_FreezeMove)
	bool freezeMove = true;

	UFUNCTION()
	void OnRep_FreezeMove();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RightValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpValue;
	
private://Controllerに移行
	void MoveForward(float Value);

	void MoveRight(float Value);

	void MoveUp(float Value);

	void StartFire();

	void StopFire();
	
	void Use();

	void SwitchBlend();

	bool UsingController();
};
