// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/CarMovementComponent.h"

#include <nvtesslib/inc/nvtess.h>

// Sets default values for this component's properties
UCarMovementComponent::UCarMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCarMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
}

void UCarMovementComponent::SimulateMove(const FGoKartMove& Move)
{
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	//FVector UpForce = GetOwner()->GetActorUpVector() * MaxClimbForce * Move.Pitch;
	//FVector Force = ForwardForce + UpForce;
	//Force += FVector::DownVector * -GetWorld()->GetGravityZ();
	Force += GetAirResistance();
	Force += GetRollingResistance();

	FVector Acceleration = Force / Mass;

	Velocity = Velocity + Acceleration * Move.DeltaTime;

	ApplyRotation(Move.DeltaTime, Move.SteeringThrow, GetOwner()->GetActorForwardVector());
	ApplyRotation(Move.DeltaTime, Move.Pitch, GetOwner()->GetActorRightVector());
	UpdateLocationFromVelocity(Move.DeltaTime);
}

FGoKartMove UCarMovementComponent::CreateMove(float DeltaTime)
{
	FGoKartMove Move;
	Move.DeltaTime = DeltaTime;
	Move.SteeringThrow = SteeringThrow;
	Move.Throttle = Throttle;
	Move.Pitch = Pitch;
	Move.Time = GetWorld()->TimeSeconds;

	return Move;
}

FVector UCarMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UCarMovementComponent::GetRollingResistance()
{
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void UCarMovementComponent::ApplyRotation(float DeltaTime, float m_SteeringThrow, FVector direction)
{
	/*float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * m_SteeringThrow;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	GetOwner()->AddActorWorldRotation(RotationDelta);*/

	//FQuat RotationDelta(GetOwner()->GetActorRightVector(), 0.1f); //up down
	FQuat RotationDelta(-direction, MinTurningRadius * Velocity.Size() * m_SteeringThrow * DeltaTime);
	GetOwner()->AddActorWorldRotation(RotationDelta);
	
	//GetOwner()->AddActorWorldRotation(FRotator(5,0,0));


	
}

void UCarMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	FVector Translation = Velocity * 100 * DeltaTime;
	FHitResult Hit;
	GetOwner()->AddActorWorldOffset(Translation, true, &Hit);
	if (Hit.IsValidBlockingHit())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit"));
		Velocity = FVector::ZeroVector;
	}
}