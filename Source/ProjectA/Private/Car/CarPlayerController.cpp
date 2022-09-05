// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/CarPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Car/CarPawn.h"
#include "UI/HUDWidget.h"

class UHUDWidget;

ACarPlayerController::ACarPlayerController()
{
	SpectorHUDClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/Widget/WBP_SpectorHUD.WBP_SpectorHUD_C"))).LoadSynchronous();
	GameHUDClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/Widget/WBP_GameHUD.WBP_GameHUD_C"))).LoadSynchronous();
	WaitingHUDClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/Widget/WBP_WaitingHUD.WBP_WaitingHUD_C"))).LoadSynchronous();
	GoalHUDClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/Widget/WBP_GoalHUD.WBP_GoalHUD_C"))).LoadSynchronous();
}

void ACarPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeUIOnly());
	this->bShowMouseCursor = true;
	SetHUD(SpectorHUDClass);
}

void ACarPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("SwitchBlend", IE_Pressed, this, &ACarPlayerController::SwitchBlend);
}

void ACarPlayerController::SetHUD(TSubclassOf<UUserWidget> HUDClass)
{
	if(IsValid(HUD))
	{
		HUD->RemoveFromParent();
	}
	UUserWidget* HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDClass);
	HUDWidget->AddToViewport();
	HUD = HUDWidget;
}

void ACarPlayerController::OnSwitchBlend()
{
	UHUDWidget* hud = Cast<UHUDWidget>(HUD);
	if(hud == nullptr) return;
	hud->OnBlendSwitch();
}

void ACarPlayerController::OnItemChange()
{
	UHUDWidget* hud = Cast<UHUDWidget>(HUD);
	if(hud == nullptr) return;
	hud->OnItemChanged();
}

void ACarPlayerController::CarPossessPawn(APawn* PossessPawn)
{
	
	SetInputMode(FInputModeGameOnly());
	this->bShowMouseCursor = false;
	if(HasAuthority())
	{
		this->UnPossess();
		this->Possess(PossessPawn);
		ClientGameStart(PossessPawn);
	}
}


void ACarPlayerController::ClientGameStart_Implementation(APawn* PossessPawn)
{
	SetHUD(WaitingHUDClass);
	CarPossessPawn(PossessPawn);
}

void ACarPlayerController::ClientRaceStart_Implementation()
{
	SetHUD(GameHUDClass);
}

void ACarPlayerController::ClientRaceEnd_Implementation()
{
	SetHUD(GoalHUDClass);
}

