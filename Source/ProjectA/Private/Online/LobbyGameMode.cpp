// Fill out your copyright notice in the Description page of Project Settings.


#include "Online/LobbyGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ALobbyGameMode::ALobbyGameMode()
{
	WidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(TEXT("/Game/Widget/WBP_Lobby.WBP_Lobby_C"))).LoadSynchronous();
}

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PC->SetInputMode(FInputModeUIOnly());
	PC->bShowMouseCursor = true;
	UUserWidget* Widget = CreateWidget(GetWorld(), WidgetClass);
	Widget->AddToViewport();
}
