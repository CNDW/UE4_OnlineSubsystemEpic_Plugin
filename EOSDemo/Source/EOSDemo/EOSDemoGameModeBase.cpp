// Copyright Epic Games, Inc. All Rights Reserved.


#include "EOSDemoGameModeBase.h"
#include "OnlineSubsystem.h"

#include "OnlineSubsystemEpic.h"

FString AEOSDemoGameModeBase::GetCurrentSubsystem() {
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem == nullptr) {
		return FString("NONE");
	}
	auto name = subsystem->GetSubsystemName();
	return name.ToString();
}

bool AEOSDemoGameModeBase::IsEpicSubsystemLoaded()
{
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get(EPIC_ONLINE_SERVICES_SUBSYSTEM);
	return subsystem != nullptr;
}
