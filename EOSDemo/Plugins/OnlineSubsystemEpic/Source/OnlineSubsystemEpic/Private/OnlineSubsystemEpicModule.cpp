
#include "OnlineSubsystemEpicModule.h"

// UE4 Includes
#include "Core.h"
#include "Interfaces/IPluginManager.h"
#include "OnlineSubsystemModule.h"
#include "OnlineSubsystemNames.h"
#include "OnlineSubsystem.h"

// Plugin Includes
#include "OnlineSubsystemEpic.h"


#if WITH_EDITOR
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#endif

#define LOCTEXT_NAMESPACE "FOnlineSubsystemEpicModule"

#ifndef EOS_SDK_INSTALLED
#error Epic Online Services SDK not located! Expected to be found in <Your Project>/Plugins/OnlineSubsystemEpic/Source/ThirdParty
#endif // EOS_SDK_INSTALLED

//HACKTASTIC (Needed to keep delete function from being stripped out and crashing when protobuffers deallocate memory)
void* HackDeleteFunctionPointer = (void*)(void(*)(void*))(::operator delete[]);

/**
 * Class responsible for creating instance(s) of the subsystem
 */
class FOnlineFactoryEpic : public IOnlineFactory
{

public:

	FOnlineFactoryEpic() {}
	virtual ~FOnlineFactoryEpic()
	{
		DestroySubsystem();
	}

	virtual IOnlineSubsystemPtr CreateSubsystem(FName InstanceName)
	{
		if(!Singleton.IsValid())
		{
			Singleton = MakeShared<FOnlineSubsystemEpic, ESPMode::ThreadSafe>(InstanceName);
			if(Singleton->IsEnabled())
			{
				if(!Singleton->Init())
				{
					UE_LOG_ONLINE(Warning, TEXT("EOS API failed to initialize!"));
					DestroySubsystem();
				}
			}
			else
			{
				UE_LOG_ONLINE(Warning, TEXT("EOS API disabled!"));
				DestroySubsystem();
			}

			return Singleton;
		}

		UE_LOG_ONLINE(Warning, TEXT("Can't create more than one instance of EOS online subsystem!"));
		return nullptr;
	}

private:

	/** Single instantiation of the Epic Online Services interface */
	static IOnlineSubsystemPtr Singleton;

	virtual void DestroySubsystem()
	{
		if(Singleton.IsValid())
		{
			Singleton->Shutdown();
			Singleton = nullptr;
		}
	}
};

IOnlineSubsystemPtr FOnlineFactoryEpic::Singleton = nullptr;

/**
 * Module Definitions
 */
void FOnlineSubsystemEpicModule::StartupModule()
{
#if !PLATFORM_LINUX
	// Get the base directory of this plugin
	FString BaseDir = "";
	TSharedPtr<IPlugin> Plugin = IPluginManager::Get().FindPlugin("OnlineSubsystemEpic");

	if(Plugin != nullptr)
	{
		BaseDir = Plugin->GetBaseDir();
	}

	const FString SDKDir = FPaths::Combine(*BaseDir, TEXT("Source"), TEXT("ThirdParty"), TEXT("EOSSDK"));

#if PLATFORM_WINDOWS

#if PLATFORM_32BITS
	const FString LibName = TEXT("EOSSDK-Win32-Shipping");
#else
	const FString LibName = TEXT("EOSSDK-Win64-Shipping");
#endif
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Bin"));
#elif PLATFORM_MAC
	const FString LibName = TEXT("libEOSSDK-Mac-Shipping");
	const FString LibDir = FPaths::Combine(*SDKDir, TEXT("Bin"));
#endif // WINDOWS/MAC

	if(LoadDependency(LibDir, LibName, EOSSDKHandle) == false)
	{
		FMessageDialog::Open(EAppMsgType::Ok, LOCTEXT(LOCTEXT_NAMESPACE, "Failed to load EOS SDK. Plugin will not be functional."));
		FreeDependency(EOSSDKHandle);

		return;
	}

	// Create and register our singleton factory with the main online subsystem for easy access
	Factory = new FOnlineFactoryEpic();

	FOnlineSubsystemModule& OSS = FModuleManager::GetModuleChecked<FOnlineSubsystemModule>("OnlineSubsystem");
	OSS.RegisterPlatformService(EPIC_ONLINE_SERVICES_SUBSYSTEM, Factory);

#endif // NOT LINUX
}

void FOnlineSubsystemEpicModule::ShutdownModule()
{
	// Free the dll handle
#if !PLATFORM_LINUX
	FreeDependency(EOSSDKHandle);
#endif
}

bool FOnlineSubsystemEpicModule::LoadDependency(const FString& Dir, const FString& Name, void*& Handle)
{
	FString Lib = Name + TEXT(".") + FPlatformProcess::GetModuleExtension();
	FString Path = Dir.IsEmpty() ? *Lib : FPaths::Combine(*Dir, *Lib);

	Handle = FPlatformProcess::GetDllHandle(*Path);

	if(Handle == nullptr)
	{
		return false;
	}

	return true;
}

void FOnlineSubsystemEpicModule::FreeDependency(void*& Handle)
{
	if(Handle != nullptr)
	{
		FPlatformProcess::FreeDllHandle(Handle);
		Handle = nullptr;
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FOnlineSubsystemEpicModule, OnlineSubsystemEpic)

DEFINE_LOG_CATEGORY(OnlineSubsystemEpicLog);
