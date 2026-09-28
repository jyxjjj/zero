using UnrealBuildTool;

[SupportedPlatforms("Win64")]
public class zeroTarget : TargetRules {
    public zeroTarget(TargetInfo Target) : base(Target) {
        Type                 = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        ExtraModuleNames.Add("zero");
    }
}
