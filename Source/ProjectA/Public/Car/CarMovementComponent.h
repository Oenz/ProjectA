// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CarMovementComponent.generated.h"

USTRUCT()
struct FGoKartMove
{
	GENERATED_BODY();

	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float Pitch;

	UPROPERTY()
	float DeltaTime;

	UPROPERTY()
	float Time;

	bool IsValid() const 
	{
	 	return FMath::Abs(Throttle) <= 1 && 
	 		FMath::Abs(SteeringThrow) <= 1; 
	 }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UCarMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCarMovementComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SimulateMove(const FGoKartMove& Move);

	FVector GetVelocity() { return Velocity; };

	void SetVelocity(FVector Val) { Velocity = Val; };
	void SetThrottle(float Val) { Throttle = Val; };
	void SetSteeringThrow(float Val) { SteeringThrow = Val; };
	void SetPitch(float Val) { Pitch = Val;};

	FGoKartMove GetLastMove()
	{
		return LastMove;
	};

private:
	FGoKartMove CreateMove(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float Mass = 100;

	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;

	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 100000;

	UPROPERTY(EditAnywhere)
	float MaxClimbForce = 1000;

	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90;

	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 0.05f;

	FVector Velocity;

	float Throttle;

	float SteeringThrow;

	float Pitch;

	FVector GetAirResistance();

	FVector GetRollingResistance();

	void ApplyRotation(float DeltaTime, float m_SteeringThrow, FVector direction);

	void UpdateLocationFromVelocity(float DeltaTime);

	FGoKartMove LastMove;
		
};
