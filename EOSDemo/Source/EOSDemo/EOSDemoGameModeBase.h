// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EOSDemoGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class EOSDEMO_API AEOSDemoGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, category = OnlineSubsystem)
		FString GetCurrentSubsystem();

	UFUNCTION(BlueprintCallable, BlueprintPure, category = OnlineSubsystem)
		bool IsEpicSubsystemLoaded();
};
