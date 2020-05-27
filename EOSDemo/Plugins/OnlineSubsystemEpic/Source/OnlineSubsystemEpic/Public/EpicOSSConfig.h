#pragma once

// Engine Includes
#include "UObject/Object.h"
#include "Engine/DeveloperSettings.h"

#include "EpicOSSConfig.generated.h"

/**
* Config class for Epic Online Services Subsystem settings.
* Allows for the settings to be displayed in the Project Settings.
* Also saves settings to a file.
*/
UCLASS(config = Engine, defaultconfig, meta = (DisplayName = "Online Subsystem Epic"))
class ONLINESUBSYSTEMEPIC_API UEpicOSSConfig : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UEpicOSSConfig();

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString ProductName;

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString ProductVersion;

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString ProductId;

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString SandboxId;

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString DeploymentId;

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString SupportTicketingKey;

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString SupportTicketingURL;

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString ClientId;

	UPROPERTY(config, EditAnywhere, Category = "Epic Online Subsystem Plugin")
		FString ClientSecret;
};
