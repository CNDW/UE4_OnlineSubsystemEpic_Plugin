#include "EpicOSSConfig.h"

UEpicOSSConfig::UEpicOSSConfig()
	: ProductName("Online Subsystem Epic Plugin")
	, ProductVersion("1.0")
	, ProductId("")
	, SandboxId("")
	, DeploymentId("")
	, ClientId("")
	, ClientSecret("")
{
	CategoryName = FName("Plugins");
	SectionName = FName("OnlineSubsystemEpic");
}