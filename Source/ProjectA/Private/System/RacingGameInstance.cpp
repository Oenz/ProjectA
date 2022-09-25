// Fill out your copyright notice in the Description page of Project Settings.


#include "System/RacingGameInstance.h"
//#include "OnlineSubsystem.h"
//#include "Interfaces/OnlineSessionInterface.h"
//#include "Kismet/KismetSystemLibrary.h"
//#include "Kismet/GameplayStatics.h"
//#include "OnlineSessionSettings.h"

URacingGameInstance::URacingGameInstance()
{   //
	//IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();
	//IOnlineSessionPtr SessionInterface = SubSystem->GetSessionInterface();
	//UGameInstance* Instance = Cast<UGameInstance>(this);
	//if(!Instance->IsDedicatedServerInstance()) return;
	//UE_LOG(LogTemp, Warning, TEXT("Creating Session"));
	//
	//if (SessionInterface.IsValid())
	//{
	//	FOnlineSessionSettings SessionSettings;
	//	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	//	
	//	SessionSettings.NumPublicConnections = 16;
	//	SessionSettings.NumPrivateConnections = 0;
	//	
	//	SessionSettings.bAllowJoinInProgress = true;
	//	SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;
	//	SessionSettings.bAllowJoinInProgress = false;
	//	SessionSettings.bAllowJoinViaPresence = true;
	//	SessionSettings.bAllowInvites = true;
	//	
	//	SessionSettings.bIsDedicated = true;
	//	SessionSettings.bIsLANMatch = false;  //testlan
	//	SessionSettings.bUsesPresence = false;
	//	SessionSettings.bShouldAdvertise = true;
	//	SessionSettings.BuildUniqueId = 1;
	//	SessionSettings.Set(SETTING_MAPNAME, FString("TestMap1"), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	//
	//
	//
	//	SessionInterface->CreateSession(0, FName("RacingGame"), SessionSettings);
	//}
}   