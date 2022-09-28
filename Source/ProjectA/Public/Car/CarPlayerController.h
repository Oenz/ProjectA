// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CarPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API ACarPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ACarPlayerController();

	UFUNCTION()
	void OnUpgradeChanged();

	void OnItemChanged();

	UFUNCTION(Reliable, Client)
	void ClientGameStart(APawn* PossessCarPawn);
	
	UFUNCTION(Reliable, Client)
	void ClientRaceStart();
	
	UFUNCTION(Reliable, Client)
	void ClientRaceEnd();

	UUserWidget* GetHUD() {return HUD;};

	void PossessCarPawn(APawn* PossessPawn);
	
protected:
	TSubclassOf<class UUserWidget> SpectorHUDClass;
	
	TSubclassOf<class UUserWidget> GameHUDClass;

	TSubclassOf<class UUserWidget> WaitingHUDClass;

	TSubclassOf<class UUserWidget> GoalHUDClass;

	UUserWidget* HUD;
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void SetHUD(TSubclassOf<class UUserWidget> HUDClass);
	
	void Ready();
	

};
