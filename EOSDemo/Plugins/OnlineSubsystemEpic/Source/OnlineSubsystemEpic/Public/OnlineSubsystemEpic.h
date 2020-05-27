#pragma once

#include "CoreMinimal.h"
#include "OnlineDelegateMacros.h"
#include "OnlineSubsystemImpl.h"

// EOS Includes
#include "eos_sdk.h"


// Subsystem Name
#ifndef EPIC_ONLINE_SERVICES_SUBSYSTEM
#define EPIC_ONLINE_SERVICES_SUBSYSTEM FName(TEXT("Epic"))
#endif

/**
 *	OnlineSubsystemSteam - Implementation of the online subsystem for EPIC ONLINE SERVICES (EOS)
 */
class ONLINESUBSYSTEMEPIC_API FOnlineSubsystemEpic : public FOnlineSubsystemImpl
{

public:

	virtual ~FOnlineSubsystemEpic()
	{
	}

	// IOnlineSubsystem
	virtual IOnlineSessionPtr GetSessionInterface() const override { return nullptr; }
	virtual IOnlineFriendsPtr GetFriendsInterface() const override { return nullptr; }
	virtual IOnlinePartyPtr GetPartyInterface() const override { return nullptr; }
	virtual IOnlineGroupsPtr GetGroupsInterface() const override { return nullptr; }
	virtual IOnlineSharedCloudPtr GetSharedCloudInterface() const override { return nullptr; }
	virtual IOnlineUserCloudPtr GetUserCloudInterface() const override { return nullptr; }
	virtual IOnlineLeaderboardsPtr GetLeaderboardsInterface() const override { return nullptr; }
	virtual IOnlineVoicePtr GetVoiceInterface() const override { return nullptr; }
	virtual IOnlineExternalUIPtr GetExternalUIInterface() const override { return nullptr; }
	virtual IOnlineTimePtr GetTimeInterface() const override { return nullptr; }
	virtual IOnlineIdentityPtr GetIdentityInterface() const override { return nullptr; }
	virtual IOnlineTitleFilePtr GetTitleFileInterface() const override { return nullptr; }
	virtual IOnlineEntitlementsPtr GetEntitlementsInterface() const override { return nullptr; }
	virtual IOnlineStoreV2Ptr GetStoreV2Interface() const override { return nullptr; }
	virtual IOnlinePurchasePtr GetPurchaseInterface() const override { return nullptr; }
	virtual IOnlineEventsPtr GetEventsInterface() const override { return nullptr; }
	virtual IOnlineAchievementsPtr GetAchievementsInterface() const override { return nullptr; }
	virtual IOnlineSharingPtr GetSharingInterface() const override { return nullptr; }
	virtual IOnlineUserPtr GetUserInterface() const override { return nullptr; }
	virtual IOnlineMessagePtr GetMessageInterface() const override { return nullptr; }
	virtual IOnlinePresencePtr GetPresenceInterface() const override { return nullptr; }
	virtual IOnlineChatPtr GetChatInterface() const override { return nullptr; }
	virtual IOnlineStatsPtr GetStatsInterface() const override { return nullptr; }
	virtual IOnlineTurnBasedPtr GetTurnBasedInterface() const override { return nullptr; }
	virtual IOnlineTournamentPtr GetTournamentInterface() const override { return nullptr; }
	virtual bool IsLocalPlayer(const FUniqueNetId& UniqueId) const override { return false; }
	virtual bool Init() override;
	virtual bool Shutdown() override;
	virtual bool Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	virtual bool IsEnabled() const override;
	virtual FString GetAppId() const override;
	virtual FText GetOnlineServiceName() const override;

	// FTickerObjectBase
	virtual bool Tick(float DeltaTime) override;

	/**
	* Checks whether the EOS SDK has been Initialized.
	*
	* @return bool True if has been initialized, otherwise false.
	*/
	bool IsSDKInitialized() { return bSDKInitialized; };

	/**
	* Returns the current Platform Handle.
	* Can be NULL if the SDK has not been initialized or failed to initialize.
	*
	* @return EOS_HPlatform The current EOS Platform Handle.
	*/
	EOS_HPlatform GetPlatformHandle() { return PlatformHandle; };

protected:

	// Attempt to gather the Config Options for the plugin
	bool GetConfigOptions();

	// Attempt to Initialize the SDK
	bool InitializeSDK();

	// Attempt to Create a valid Platform Handle from the SDK
	bool CreatePlatformHandle();


	/**
	 * Epic Online Services Configuration variables
	 */
	FString ProductName;
	FString ProductVersion;
	FString ProductId;
	FString SandboxId;
	FString DeploymentId;
	FString ClientId;
	FString ClientSecret;

	/// ---------------------------------------------------
	/// Subsystem Interfaces
	/// ---------------------------------------------------


	/// ---------------------------------------------------

PACKAGE_SCOPE :

	/** Only the factory makes instances */
	FOnlineSubsystemEpic() = delete;
	FOnlineSubsystemEpic(FName InInstanceName)
		: FOnlineSubsystemImpl(EPIC_ONLINE_SERVICES_SUBSYSTEM, InInstanceName)
		, ProductName("")
		, ProductVersion("")
		, ProductId("")
		, SandboxId("")
		, DeploymentId("")
		, ClientId("")
		, ClientSecret("")
		, bSDKInitialized(false)
		, PlatformHandle(nullptr)
	{}

private:

	bool bSDKInitialized;

	/** The EOS Platform Handle for Platform operations. */
	EOS_HPlatform PlatformHandle;

};

typedef TSharedPtr<FOnlineSubsystemEpic, ESPMode::ThreadSafe> FOnlineSubsystemEpicPtr;
