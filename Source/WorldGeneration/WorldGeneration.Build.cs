using UnrealBuildTool; 

public class WorldGeneration: ModuleRules { 
	public WorldGeneration(ReadOnlyTargetRules Target) : base(Target) {       
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "Voronoi", "GameplayTags", "UMG"});
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	} 
}