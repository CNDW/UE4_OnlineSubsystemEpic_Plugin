$scriptDir = Split-Path -Path $MyInvocation.MyCommand.Definition -Parent
$projectDir = Join-Path -Path $scriptDir -ChildPath ".."

Write-Output "Cleaning visual studio project files..."
Remove-Item -path "$projectDir\Intermediate\" -recurse
Remove-Item -path "$projectDir\Plugins\**\Intermediate" -recurse
Remove-Item -path "$projectDir\*.sln"
Write-Output "Project file clean complete"

$ue4ProjectFile = Get-ChildItem -Path "$projectDir\*.uproject"
$ue4Project = Get-Content -Raw -Path "$ue4ProjectFile" | ConvertFrom-Json
$engineVersion = $ue4Project.EngineAssociation
$buildToolDir = "D:\unreal4\Epic Games\UE_$engineVersion\Engine\Binaries\DotNET"

. $buildToolDir\UnrealBuildTool.exe -projectfiles -project="$ue4ProjectFile" -game -rocket -progress
