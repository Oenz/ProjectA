// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/CarPlayerController.h"
#include "Online/CarPlayerState.h"
#include "Blueprint/UserWidget.h"
#include "Car/CarPawn.h"
#include "UI/HUDWidget.h"
#include "Online/RacingGameMode.h"
#include "UI/SpectorWidget.h"

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
	SetInputMode(FInputModeGameOnly());
	this->bShowMouseCursor = true;
	SetHUD(SpectorHUDClass);
}

void ACarPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("Ready", IE_Pressed,this, &ACarPlayerController::Ready);
}

void ACarPlayerController::Ready()
{
	UE_LOG(LogTemp, Warning, TEXT("Ready"));
	ACarPlayerState* CPS = GetPlayerState<ACarPlayerState>();
	if (!IsValidChecked(CPS)) return;
	CPS->ServerPlayerReady(!CPS->isReady);
	CPS->isReady = !CPS->isReady;
	if(USpectorWidget* SpectorWidget = Cast<USpectorWidget>(GetHUD()))
	{
		if(SpectorWidget == nullptr) return;
		SpectorWidget->OnReadyChange(CPS->isReady);
	}
	
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

void ACarPlayerController::OnUpgradeChanged()
{
	UHUDWidget* hud = Cast<UHUDWidget>(HUD);
	if(hud == nullptr) return;
	hud->OnUpgradeChanged();
}

void ACarPlayerController::OnItemChanged()
{
	UHUDWidget* hud = Cast<UHUDWidget>(HUD);
	if(hud == nullptr) return;
	hud->OnItemChanged();
}

void ACarPlayerController::PossessCarPawn(APawn* PossessPawn)
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
	PossessCarPawn(PossessPawn);
}

void ACarPlayerController::ClientRaceStart_Implementation()
{
	SetHUD(GameHUDClass);
}

void ACarPlayerController::ClientRaceEnd_Implementation()
{
	SetHUD(GoalHUDClass);
}

