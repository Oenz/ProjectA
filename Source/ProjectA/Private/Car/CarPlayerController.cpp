// Fill out your copyright notice in the Description page of Project Settings.


#include "Car/CarPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "UI/HUDWidget.h"

ACarPlayerController::ACarPlayerController()
{
	HUDClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/Widget/WBP_HUD.WBP_HUD_C"))).LoadSynchronous();
}

void ACarPlayerController::BeginPlay()
{
	Super::BeginPlay();
	UUserWidget* HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDClass);
		//CreateWidget(Cast<APawn>(GetOwner()), HUDClass, TEXT("HUD"));

	HUDWidget->AddToViewport();
	HUD = HUDWidget;
}

void ACarPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAction("SwitchBlend", IE_Pressed, this, &ACarPlayerController::SwitchBlend);
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


