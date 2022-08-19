// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CarMovementComponent.h"
#include "CarMovementReplicatorComponent.generated.h"

USTRUCT()
struct FGoKartState
{
	GENERATED_BODY();

	UPROPERTY()
	FTransform Transform;

	UPROPERTY()
	FVector Velocity;

	UPROPERTY()
	FGoKartMove LastMove;
};

struct FHermiteCubicSpline
{
	FVector StartLocation, StartDerivative, TargetLocation, TargetDerivative;

	FVector InterpolateLocation(float LerpRadio) const
	{ return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRadio); }

	FVector InterpolateDerivative(float LerpRadio) const
	{ return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRadio); }
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTA_API UCarMovementReplicatorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCarMovementReplicatorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
private:
	void ClearAcknowledgeMoves(FGoKartMove LastMove);

	void UpdateServerState(const FGoKartMove& Move);

	void ClientTick(float DeltaTime);

	FHermiteCubicSpline CreateSpline();

	void InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRadio);
	void InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRadio);
	void InterpolateRotation(float LerpRadio);

	float VelocityToDerivate();

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)
	FGoKartState ServerState;

	UFUNCTION()
		void OnRep_ServerState();

	UFUNCTION(Server, Reliable, WithValidation)
		void Server_SendMove(FGoKartMove Move);

	void AutonomousProxy_OnRep_ServerState();
	void SimulatedProxy_OnRep_ServerState();

	FTransform ClientStartTransform;

	TArray<FGoKartMove> UnacknowledgedMoves;

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdate;

	FVector ClientStartVelocity;

	float ClientSimulatedTime;

	UPROPERTY()
	UCarMovementComponent* MovementComponent;

	UPROPERTY()
	USceneComponent* MeshOffsetRoot;

	UFUNCTION(BlueprintCallable)
		void SetMeshOffsetRoot(USceneComponent* Root) { MeshOffsetRoot = Root; }

};
