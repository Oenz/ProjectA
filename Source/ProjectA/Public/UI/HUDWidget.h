// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API UHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnBlendSwitch();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnItemChanged();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnStan();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void OnEndStan();
};
