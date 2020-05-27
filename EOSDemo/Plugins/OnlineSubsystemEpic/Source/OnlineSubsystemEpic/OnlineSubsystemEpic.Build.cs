
using UnrealBuildTool;
using System.IO;

public class OnlineSubsystemEpic : ModuleRules
{
    public OnlineSubsystemEpic( ReadOnlyTargetRules Target ) : base( Target )
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDefinitions.Add( "ONLINESUBSYSTEMEPIC_PACKAGE=1" );

        if( Target.Configuration != UnrealTargetConfiguration.Shipping )
        {
            OptimizeCode = CodeOptimization.Never;
        }

        PublicIncludePaths.AddRange(
            new string[] {
                Path.Combine( ModuleDirectory, "Public" )
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                Path.Combine( ModuleDirectory, "Private" )
            }
            );


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


        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );
    }
}
