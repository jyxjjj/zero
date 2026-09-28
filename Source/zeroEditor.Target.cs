using UnrealBuildTool;

[SupportedPlatforms("Win64")]
public class zeroEditorTarget : TargetRules {
    public zeroEditorTarget(TargetInfo Target) : base(Target) {
        Type                 = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        ExtraModuleNames.Add("zero");
    }
}
