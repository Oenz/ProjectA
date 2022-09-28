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
	ACarPawn();

	UFUNCTION(BlueprintCallable)
	void SetStan(float second);

	UFUNCTION(BlueprintCallable)
	void SetEndStan();

	void SetStart();
	
	void SetGoal();

	UFUNCTION(Client,Reliable)
	void ClientViewTargetChange();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
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
	class USceneComponent* CameraTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCameraComponent* CameraComponent;
	
	UPROPERTY(Replicated)
	class AProjectileLauncher* ProjectileLauncher;

	bool gamestart = false;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnRaceStart();
	

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnStan();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnEndStan();
	

	UPROPERTY(ReplicatedUsing=OnRep_FreezeMove, EditAnywhere, BlueprintReadOnly)
	bool freezeMove = true;

	UFUNCTION()
	void OnRep_FreezeMove();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ForwardValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RightValue;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float UpValue;
	
	
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnViewTargetChange();

	UPROPERTY(Replicated, BlueprintReadOnly)
	bool bGoal = false;
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	void MoveUp(float Value);

	void StartFire();

	void StopFire();
	
	void Use();

	void SelectUpgrade();

	bool UsingController();
};
