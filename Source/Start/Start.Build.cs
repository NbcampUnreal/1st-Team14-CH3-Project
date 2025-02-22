// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Start : ModuleRules
{
	public Start(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// 이 프로젝트에서 사용하는 공개된 종속 모듈
		PublicIncludePaths.Add(ModuleDirectory);
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", // 코어 엔진의 기본 기능
			"CoreUObject", // 리플렉션 시스템
			"Engine",  // 게임 엔진의 주요 기능
			"InputCore", // 입력 시스템
			"EnhancedInput", // EnhancedInputSystem 관련
			"UMG", "AIModule", // *UI 위젯*
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
