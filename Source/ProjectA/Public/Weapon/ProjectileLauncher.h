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

	UFUNCTION(Client, Reliable)
	void ClientBombFire();

	UFUNCTION()
	void StartDefaultFire();

	UFUNCTION()
	void StopDefaultFire();
	
protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	TSubclassOf<class AProjectile> DefaultProjectile;

	TSubclassOf<class AProjectile> BombProjectile;

	void FireProjectile(TSubclassOf<class AProjectile> ProjectileObject, FVector CursourPosition);

	FTimerHandle DefaultFireTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LastFireTime = 0.0f;
	
	UFUNCTION()
	FVector GetCursorFirePosition();

	UFUNCTION(Server, Reliable)
	void ServerFireProjectile(FVector TargetPos, bool isBomb);


};
