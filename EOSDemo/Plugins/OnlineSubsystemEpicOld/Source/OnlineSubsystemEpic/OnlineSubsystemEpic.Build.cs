
using UnrealBuildTool;
using System.IO;

public class OnlineSubsystemEpic : ModuleRules
{
    public OnlineSubsystemEpic( ReadOnlyTargetRules Target ) : base( Target )
    {
        PublicDefinitions.Add( "ONLINESUBSYSTEMEPIC_PACKAGE=1" );
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "OnlineSubsystemUtils",
                "EOSSDK"
            }
        );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "NetCore",
                "Engine",
                "Sockets",
                "Voice",
                "AudioMixer",
                "OnlineSubsystem",
                "Json",
                "PacketHandler",
                "Projects",
            }
        );
    }
}
