// Copyright Epic Games, Inc. All Rights Reserved.


#include "EOSDemoGameModeBase.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "engine/world.h"

#include "OnlineSubsystemEpic.h"

FString AEOSDemoGameModeBase::GetCurrentSubsystem() {
	IOnlineSubsystem* subsystem = Online::GetSubsystem(GetWorld(), EPIC_SUBSYSTEM);
	if (subsystem == nullptr) {
		return FString("NONE");
	}
	auto name = subsystem->GetSubsystemName();
	return name.ToString();
}

bool AEOSDemoGameModeBase::IsEpicSubsystemLoaded()
{
	IOnlineSubsystem* subsystem = Online::GetSubsystem(GetWorld(), EPIC_SUBSYSTEM);
	return subsystem != nullptr;
}

void AEOSDemoGameModeBase::Login()
{
	IOnlineSubsystem* subsystem = Online::GetSubsystem(GetWorld(), EPIC_SUBSYSTEM);
	auto identity = subsystem->GetIdentityInterface();
	identity->AutoLogin(0);
}
