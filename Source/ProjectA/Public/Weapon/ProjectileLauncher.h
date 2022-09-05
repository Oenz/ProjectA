// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileLauncher.generated.h"

UCLASS()
class PROJECTA_API AProjectileLauncher : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AProjectileLauncher();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<class AProjectile> DefaultProjectile;

	TSubclassOf<class AProjectile> BombProjectile;

	void FireProjectile(TSubclassOf<class AProjectile> ProjectileObject, FVector CursourPosition);

	FTimerHandle DefaultFireTimer;

	float FireRate = 0.3f;

	float LastFireTime = 0.0f;
	

	UFUNCTION()
	FVector GetCursorFirePosition();
	
	UFUNCTION()
	void StartDefaultFire();

	UFUNCTION()
	void StopDefaultFire();

	UFUNCTION(Server, Reliable)
	void ServerFireProjectile(FVector TargetPos, bool isBomb);

	UFUNCTION(Client, Reliable)
	void ClientBombFire();
};
