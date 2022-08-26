// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyWidget.h"

#include "Components/Button.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MatchingButton->OnClicked.AddDynamic(this, &ULobbyWidget::Matching);

}

void ULobbyWidget::Matching()
{
	FString command = "open 127.0.0.1";
	GetOwningPlayer()->ConsoleCommand(*command, true);
	if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *command);
	}
}
