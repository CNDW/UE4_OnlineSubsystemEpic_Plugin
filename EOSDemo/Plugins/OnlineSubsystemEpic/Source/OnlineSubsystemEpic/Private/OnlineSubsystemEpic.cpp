#include "OnlineSubsystemEpic.h"


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
	std::string ProductNameStr = TCHAR_TO_UTF8(*ProductName);
	std::string ProductVersionStr = TCHAR_TO_UTF8(*ProductVersion);

	// Init EOS SDK
	EOS_InitializeOptions SDKOptions;
	SDKOptions.ApiVersion = EOS_INITIALIZE_API_LATEST;
	SDKOptions.AllocateMemoryFunction = nullptr;
	SDKOptions.ReallocateMemoryFunction = nullptr;
	SDKOptions.ReleaseMemoryFunction = nullptr;
	SDKOptions.ProductName = ProductNameStr.c_str();
	SDKOptions.ProductVersion = ProductVersionStr.c_str();
	SDKOptions.Reserved = nullptr;
	SDKOptions.SystemInitializeOptions = nullptr;

	EOS_EResult InitResult = EOS_Initialize(&SDKOptions);

	if(InitResult != EOS_EResult::EOS_Success)
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Initialization: FAILED!"));
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
	static std::string EncryptionKey(64, '1');
	PlatformOptions.EncryptionKey = EncryptionKey.c_str();
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

	std::string ProductIdStr = TCHAR_TO_UTF8(*ProductId);
	std::string SandboxIdStr = TCHAR_TO_UTF8(*SandboxId);
	std::string DeploymentIdStr = TCHAR_TO_UTF8(*DeploymentId);

	if(ProductIdStr.empty())
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Product Id is invalid."));
		return false;
	}

	if(SandboxIdStr.empty())
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Sandbox Id is invalid."));
		return false;
	}

	if(DeploymentIdStr.empty())
	{
		UE_LOG_ONLINE(Warning, TEXT("EOS SDK Deployment Id is invalid."));
		return false;
	}

	PlatformOptions.ProductId = ProductIdStr.c_str();
	PlatformOptions.SandboxId = SandboxIdStr.c_str();
	PlatformOptions.DeploymentId = DeploymentIdStr.c_str();

	std::string ClientIdStr = TCHAR_TO_UTF8(*ClientId);
	std::string ClientSecretStr = TCHAR_TO_UTF8(*ClientSecret);

	PlatformOptions.ClientCredentials.ClientId = (ClientIdStr.empty()) ? nullptr : ClientIdStr.c_str();
	PlatformOptions.ClientCredentials.ClientSecret = (ClientSecretStr.empty()) ? nullptr : ClientSecretStr.c_str();

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
