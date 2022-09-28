// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GoalArea.generated.h"

UCLASS()
class PROJECTA_API AGoalArea : public AActor
{
	GENERATED_BODY()

public:
	void SetVisiable(bool isVisiable);

	int GetCheckPointNumber() const {return CheckPointNumber;}
	
protected:	
	AGoalArea();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* MeshComponent;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int CheckPointNumber = 1;
	
	bool bVisiable = true;
	
	virtual void BeginPlay() override;


	virtual void Tick(float DeltaTime) override;

};
