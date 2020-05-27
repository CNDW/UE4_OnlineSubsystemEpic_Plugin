# Epic Online Subsystem
A plugin for implementing the Epic Online Services into your game

This plugin does not include the EOS (Epic Online Services) SDK.  You will have to register and download this separately.  Then copy the Bin, Include and Lib folders to the Plugins/OnlineSubsystemEpic/Source/ThirdParty/EOSSDK directory.

- UE4 version: **4.25.0**
- Epic Online Services SDK version: **v1.6**

If you download and run the sample project, you will need to:
- Have registered your project on the EOS site (Dev Portal)
- Retrieved your ProductId, SandboxId, DepolymentId, ClientId and Client Secret
- Add the ProductId, SandboxId, DeploymentId, ClientId and Client Secret to **DefaultEngine.ini** (see the appropriate area)
