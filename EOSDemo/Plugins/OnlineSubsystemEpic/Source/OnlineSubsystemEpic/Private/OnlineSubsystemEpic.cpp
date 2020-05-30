#include "OnlineSubsystemEpic.h"
#include "Core.h"

#include "eos_sdk.h"


bool FOnlineSubsystemEpic::Init()
{
	if(IsSDKInitialized() == false)
	{
		// Attempt to initialize the SDK!
		if(GetConfigOptions() == false)
		{
			UE_LOG_ONLINE(Warning, TEXT("Could not gather EOS Config Options! Falling back to another OSS."));
			return false;
		}

		// Have config values, attempt to initialize the EOS SDK
		if(InitializeSDK() == false)
		{
			UE_LOG_ONLINE(Warning, TEXT("Could not initialize EOS SDK! Falling back to another OSS."));
			return false;
		}
	}

	// Initialized the SDK, attempt to get a Platform Handle
	if( CreatePlatformHandle() == false)
	{
		UE_LOG_ONLINE( Warning, TEXT("Could not create Platform Handle from EOS SDK! Falling back to another OSS."));
		return false;
	}

	// Instantiate Online Subsystem interfaces

	return true;
}

bool FOnlineSubsystemEpic::Shutdown()
{
	FOnlineSubsystemImpl::Shutdown();

	// Attempt to end any Async Processes.

#define DESTRUCT_INTERFACE(Interface) \
	if(Interface.IsValid()) \
	{ \
		ensure(Interface.IsUnique()); \
		Interface = nullptr; \
	}

	// Destroy Online Subsystem interfaces

	if(IsSDKInitialized() == true)
	{
		// Attempt to Shutdown the SDK.
		EOS_EResult ShutdownResult = EOS_Shutdown();

		if(ShutdownResult != EOS_EResult::EOS_Success)
		{
			UE_LOG_ONLINE(Warning, TEXT("EOS SDK Shutdown failed!"));
			return false;
		}

		bSDKInitialized = false;
	}

	return true;
}

bool FOnlineSubsystemEpic::Exec(class UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if(FOnlineSubsystemImpl::Exec(InWorld, Cmd, Ar))
	{
		return true;
	}

	return false;
}

bool FOnlineSubsystemEpic::IsEnabled() const
{
	return FOnlineSubsystemImpl::IsEnabled();
}

FString FOnlineSubsystemEpic::GetAppId() const
{
	return "";
}

FText FOnlineSubsystemEpic::GetOnlineServiceName() const
{
	return NSLOCTEXT("OnlineSubsystemEpic", "OnlineServiceName", "Epic");
}

bool FOnlineSubsystemEpic::Tick(float DeltaTime)
{
	if(IsSDKInitialized() == true)
	{
		if(PlatformHandle == nullptr)
		{
			UE_LOG_ONLINE(Warning, TEXT("Can Tick EOS, PlatformHandle is invalid."));
			bSDKInitialized = false;
			return false;
		}

		EOS_Platform_Tick(PlatformHandle);
	}

	return true;
}

bool FOnlineSubsystemEpic::GetConfigOptions()
{
	if(GConfig->GetString(TEXT("OnlineSubsystemEpic"), TEXT("ProductName"), ProductName, GEngineIni) == false)
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing ProductName key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	if(GConfig->GetString(TEXT("OnlineSubsystemEpic"), TEXT("ProductVersion"), ProductVersion, GEngineIni) == false)
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing ProductVersion key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	if(GConfig->GetString(TEXT("OnlineSubsystemEpic"), TEXT("ProductId"), ProductId, GEngineIni) == false)
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing ProductId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	if(GConfig->GetString(TEXT("OnlineSubsystemEpic"), TEXT("SandboxId"), SandboxId, GEngineIni) == false)
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing SandboxId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	if(GConfig->GetString(TEXT("OnlineSubsystemEpic"), TEXT("DeploymentId"), DeploymentId, GEngineIni) == false)
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing DeploymentId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	if(GConfig->GetString(TEXT("OnlineSubsystemEpic"), TEXT("ClientId"), ClientId, GEngineIni) == false)
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing ClientId key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	if(GConfig->GetString(TEXT("OnlineSubsystemEpic"), TEXT("ClientSecret"), ClientSecret, GEngineIni) == false)
	{
		UE_LOG_ONLINE(Warning, TEXT("Missing ClientSecret key in OnlineSubsystemEpic of DefaultEngine.ini"));
		return false;
	}

	UE_LOG_ONLINE(Warning, TEXT("EOS Config: Success."));

	return true;
}

bool FOnlineSubsystemEpic::InitializeSDK()
{
	FTCHARToUTF8 ProductNameStr(*ProductName);
	FTCHARToUTF8 ProductVersionStr(*ProductVersion);

	// Init EOS SDK
	EOS_InitializeOptions SDKOptions;
	SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	SDKOptions.AllocateMemoryFunction = nullptr;
	SDKOptions.ReallocateMemoryFunction = nullptr;
	SDKOptions.ReleaseMemoryFunction = nullptr;
	SDKOptions.ProductName = ProductNameStr.Get();
	SDKOptions.ProductVersion = ProductVersionStr.Get();
	SDKOptions.Reserved = nullptr;
	SDKOptions.SystemInitializeOptions = nullptr;

	EOS_EResult InitResult = EOS_Initialize(&SDKOptions);

	if(InitResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Initialization: FAILED! - %s"), ANSI_TO_TCHAR(EOS_EResult_ToString(InitResult)));
		return false;
	}

	bSDKInitialized = true;
	UE_LOG_ONLINE(Warning, TEXT("EOS SDK Initialization: Success!"));
	return true;
}

bool FOnlineSubsystemEpic::CreatePlatformHandle()
{
	// Create platform instance
	EOS_Platform_Options PlatformOptions;
	PlatformOptions.ApiVersion = EOS_PLATFORM_OPTIONS_API_LATEST;
	PlatformOptions.bIsServer = EOS_FALSE;
	PlatformOptions.EncryptionKey = nullptr;
	PlatformOptions.OverrideCountryCode = nullptr;
	PlatformOptions.OverrideLocaleCode = nullptr;
	// The EncryptionKey is a 64 ones
	static const char EncryptionKey[65] = "1111111111111111111111111111111111111111111111111111111111111111";
	PlatformOptions.EncryptionKey = EncryptionKey;
	PlatformOptions.Flags = 0;

	FString TempPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectSavedDir() + "/Temp/");
	if(FPlatformFileManager::Get().GetPlatformFile().DirectoryExists(*TempPath) == false)
	{
		if(FPlatformFileManager::Get().GetPlatformFile().CreateDirectoryTree(*TempPath) == false)
		{
			UE_LOG_ONLINE(Warning, TEXT("EOS SDK Failed to create Cache Directory: %s."), *TempPath);
			return false;
		}
	}

	PlatformOptions.CacheDirectory = TCHAR_TO_UTF8(*TempPath);

	FTCHARToUTF8 ProductIdStr(*ProductId);
	FTCHARToUTF8 SandboxIdStr(*SandboxId);
	FTCHARToUTF8 DeploymentIdStr(*DeploymentId);

	if(ProductIdStr.Length() == 0)
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Product Id is invalid."));
		return false;
	}

	if(SandboxIdStr.Length() == 0)
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Sandbox Id is invalid."));
		return false;
	}

	if(DeploymentIdStr.Length() == 0)
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Deployment Id is invalid."));
		return false;
	}

	PlatformOptions.ProductId = ProductIdStr.Get();
	PlatformOptions.SandboxId = SandboxIdStr.Get();
	PlatformOptions.DeploymentId = DeploymentIdStr.Get();

	FTCHARToUTF8 ClientIdStr(*ClientId);
	FTCHARToUTF8 ClientSecretStr(*ClientSecret);

	PlatformOptions.ClientCredentials.ClientId = ClientIdStr.Get();
	PlatformOptions.ClientCredentials.ClientSecret = ClientSecretStr.Get();

	PlatformOptions.Reserved = NULL;

	PlatformHandle = EOS_Platform_Create(&PlatformOptions);

	if(PlatformHandle == nullptr)
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Failed to create platform."));
		return false;
	}

	UE_LOG_ONLINE(Warning, TEXT("EOS SDK Platform: Success."));

	return true;
}
