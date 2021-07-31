// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FirstPrototypeCPP : ModuleRules
{
	public FirstPrototypeCPP(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
