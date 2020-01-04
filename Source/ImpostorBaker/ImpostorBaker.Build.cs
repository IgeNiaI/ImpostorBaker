

namespace UnrealBuildTool.Rules
{
    public class ImpostorBaker : ModuleRules
    {
        public ImpostorBaker(ReadOnlyTargetRules Target) : base(Target)
        {
            //PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

            PublicDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "MeshDescription",
                    "ProceduralMeshComponent"
                }
                );
        }
    }
}
