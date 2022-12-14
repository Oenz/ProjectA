// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Buttons", meta = (BindWidget))
	class UButton* MatchingButton;
	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Buttons", meta = (BindWidget))
	class UButton* AutoMatchingButton;
	
	UPROPERTY(EditDefaultsOnly, BluePrintReadWrite, Category = "Text", meta = (BindWidget))
	class UEditableTextBox* TextBox;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void Matching();

	UFUNCTION()
	void AutoMatching();
};
