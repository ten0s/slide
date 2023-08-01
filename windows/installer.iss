#define Version "{{version}}"

[Setup]
AppName=Slide
AppId=ten0s/Slide
AppVersion={#Version}
AppPublisher=Dmitry Klionsky
AppPublisherURL=https://github.com/ten0s/slide
AppSupportURL=https://github.com/ten0s/slide
AppUpdatesURL=https://github.com/ten0s/slide
VersionInfoVersion={#Version}
VersionInfoCompany=Dmitry Klionsky
VersionInfoCopyright=GNU General Public License v3
VersionInfoDescription=AutoCAD slide command line utilities
LicenseFile=LICENSE
ArchitecturesAllowed=x64
ArchitecturesInstallIn64BitMode=x64
DefaultDirName={autopf}\Slide
DefaultGroupName=Slide
Compression=lzma2
SolidCompression=yes
UninstallDisplayIcon={app}\slide.exe
WizardStyle=modern
ChangesEnvironment=true

[Files]
;Source: "slide-{#Version}-win-x64\*"; DestDir: "{app}"; Flags: recursesubdirs ignoreversion

[Icons]
Name: "{group}\Slide Website"; Filename: "https://github.com/ten0s/slide"
Name: "{group}\Uninstall";     Filename: "{uninstallexe}"

[Tasks]
Name: envPath; Description: "Add to PATH variable"

#include "windows/installer-libpath.iss"
[Code]
procedure CurStepChanged(CurStep: TSetupStep);
begin
    if (CurStep = ssPostInstall) and WizardIsTaskSelected('envPath')
    then EnvAddPath(ExpandConstant('{app}'));
end;

procedure CurUninstallStepChanged(CurUninstallStep: TUninstallStep);
begin
    if CurUninstallStep = usPostUninstall
    then EnvRemovePath(ExpandConstant('{app}'));
end;
