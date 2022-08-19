// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/CarMovementReplicatorComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UCarMovementReplicatorComponent::UCarMovementReplicatorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicated(true);
	// ...
}


// Called when the game starts
void UCarMovementReplicatorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	MovementComponent = GetOwner()->FindComponentByClass<UCarMovementComponent>();

}


// Called every frame
void UCarMovementReplicatorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (MovementComponent == nullptr) return;
	
	FGoKartMove LastMove = MovementComponent->GetLastMove();

	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(LastMove);
		Server_SendMove(LastMove);
	}

	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UpdateServerState(LastMove);
	}

	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		ClientTick(DeltaTime);
	}

	//UE_LOG(LogTemp, Warning, TEXT("%d"), UnacknowledgedMoves.Num());
}

void UCarMovementReplicatorComponent::UpdateServerState(const FGoKartMove& Move)
{
	ServerState.LastMove = Move;
	ServerState.Transform = GetOwner()->GetActorTransform();
	ServerState.Velocity = MovementComponent->GetVelocity();
}

void UCarMovementReplicatorComponent::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;

	if (ClientTimeBetweenLastUpdate < KINDA_SMALL_NUMBER) return;
	if (MovementComponent == nullptr) return;

	float LerpRadio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdate;
	FHermiteCubicSpline Spline = CreateSpline();
	InterpolateLocation(Spline, LerpRadio);
	InterpolateVelocity(Spline, LerpRadio);
	InterpolateRotation(LerpRadio);
}

FHermiteCubicSpline UCarMovementReplicatorComponent::CreateSpline()
{
	FHermiteCubicSpline Spline;
	Spline.TargetLocation = ServerState.Transform.GetLocation();
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * VelocityToDerivate();
	Spline.TargetDerivative = ServerState.Velocity * VelocityToDerivate();
	return Spline;
}

void UCarMovementReplicatorComponent::InterpolateLocation(const FHermiteCubicSpline& Spline, float LerpRadio)
{
	FVector NewLocation = Spline.InterpolateLocation(LerpRadio);
	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldLocation(NewLocation);
	}
}

void UCarMovementReplicatorComponent::InterpolateVelocity(const FHermiteCubicSpline& Spline, float LerpRadio)
{
	FVector NewDerivative = Spline.InterpolateDerivative(LerpRadio);
	FVector NewVelocity = NewDerivative / VelocityToDerivate();
	MovementComponent->SetVelocity(NewVelocity);
}

void UCarMovementReplicatorComponent::InterpolateRotation(float LerpRadio)
{
	FQuat TargetQuat = ServerState.Transform.GetRotation();
	FQuat StartQuat = ClientStartTransform.GetRotation();
	FQuat NewQuat = FQuat::Slerp(StartQuat, TargetQuat, LerpRadio);

	if (MeshOffsetRoot != nullptr)
	{
		MeshOffsetRoot->SetWorldRotation(NewQuat);
	}
}

float UCarMovementReplicatorComponent::VelocityToDerivate()
{
	return ClientTimeBetweenLastUpdate * 100;
}

void UCarMovementReplicatorComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCarMovementReplicatorComponent, ServerState);
}

void UCarMovementReplicatorComponent::OnRep_ServerState()
{
	switch (GetOwnerRole())
	{
	case ROLE_AutonomousProxy:
		AutonomousProxy_OnRep_ServerState();
		break;
	case ROLE_SimulatedProxy:
		SimulatedProxy_OnRep_ServerState();
		break;
	default:
		break;
	}
}

void UCarMovementReplicatorComponent::AutonomousProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;

	GetOwner()->SetActorTransform(ServerState.Transform);
	MovementComponent->SetVelocity(ServerState.Velocity);

	ClearAcknowledgeMoves(ServerState.LastMove);

	for (const FGoKartMove& Move : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Move);
	}
}

void UCarMovementReplicatorComponent::SimulatedProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;

	ClientTimeBetweenLastUpdate = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;

	if (MeshOffsetRoot != nullptr)
	{
		ClientStartTransform.SetLocation(MeshOffsetRoot->GetComponentLocation());
		ClientStartTransform.SetRotation(MeshOffsetRoot->GetComponentQuat());
	}
	ClientStartVelocity = MovementComponent->GetVelocity();

	GetOwner()->SetActorTransform(ServerState.Transform);
}

void UCarMovementReplicatorComponent::ClearAcknowledgeMoves(FGoKartMove LastMove)
{
	TArray<FGoKartMove> NewMoves;

	for (const FGoKartMove& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
			NewMoves.Add(Move);
	}

	UnacknowledgedMoves = NewMoves;
}

void UCarMovementReplicatorComponent::Server_SendMove_Implementation(FGoKartMove Move)
{
	if (MovementComponent == nullptr) return;

	ClientSimulatedTime += Move.DeltaTime;
	MovementComponent->SimulateMove(Move);

	UpdateServerState(Move);
}

bool UCarMovementReplicatorComponent::Server_SendMove_Validate(FGoKartMove Move)
{
	float ProposedTime = ClientSimulatedTime + Move.DeltaTime;
	bool ClientNotRunningAhead = ProposedTime < GetWorld()->TimeSeconds;

	if(!ClientNotRunningAhead)
	{
		UE_LOG(LogTemp, Error, TEXT("Client is running fast."));
		return false;
	}
	if (!Move.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Received invalid move."));
		return false;
	}

	return true;
}

