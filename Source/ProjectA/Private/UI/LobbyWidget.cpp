// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LobbyWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void ULobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MatchingButton->OnClicked.AddDynamic(this, &ULobbyWidget::Matching);

}

void ULobbyWidget::Matching()
{
	FString command = "open " + TextBox->GetText().ToString();
	GetOwningPlayer()->ConsoleCommand(*command, true);
	/*if (GEngine) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, *command);
	}*/
}
