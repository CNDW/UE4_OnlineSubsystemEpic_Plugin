#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FOnlineSubsystemEpicModule : public IModuleInterface
{

public:
	FOnlineSubsystemEpicModule()
        : Factory(NULL)
	{}

	virtual ~FOnlineSubsystemEpicModule() {}

	// IModuleInterface

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	virtual bool SupportsDynamicReloading() override { return false; }
	virtual bool SupportsAutomaticShutdown() override { return false; }
	virtual bool AreEOSDllsLoaded() { return EOSSDKHandle != nullptr; }

	static inline bool IsAvailable() { return FModuleManager::Get().IsModuleLoaded("OnlineSubsystemEOS"); }
	static inline class FUEOSModule& Get() { return FModuleManager::LoadModuleChecked<class FUEOSModule>("OnlineSubsystemEOS"); }
protected:

private:
	void* EOSSDKHandle;
	/** Class responsible for creating instance(s) of the subsystem */
	class FOnlineFactoryEpic* Factory;

	bool LoadDependency(const FString& Dir, const FString& Name, void*& Handle);
	void FreeDependency(void*& Handle);
};

DECLARE_LOG_CATEGORY_EXTERN(OnlineSubsystemEpicLog, Log, All);
