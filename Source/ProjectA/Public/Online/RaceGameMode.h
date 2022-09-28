// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RaceGameMode.generated.h"

UCLASS()
class PROJECTA_API ARaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

	protected:
	ARaceGameMode();

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

};
