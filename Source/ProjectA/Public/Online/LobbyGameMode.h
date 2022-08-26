// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTA_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
	ALobbyGameMode();
	
public:
	
	virtual void BeginPlay() override;

	TSubclassOf<UUserWidget> WidgetClass;
};