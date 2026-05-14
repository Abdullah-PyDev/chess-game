[Setup]
AppName=Ultimate Chess
AppVersion=1.0
AppPublisher=Abdullah Shafiq
DefaultDirName={pf}\Ultimate Chess
DefaultGroupName=Ultimate Chess
OutputDir=Installer
OutputBaseFilename=UltimateChessSetup
Compression=lzma
SolidCompression=yes
WizardStyle=modern
SetupIconFile=C:\Users\Laptop house\source\repos\chess-game\horse.ico

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "Create a desktop shortcut"; GroupDescription: "Additional tasks:"; Flags: unchecked

[Files]
Source: "x64\Release\chess.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "x64\Release\raylib.dll"; DestDir: "{app}"; Flags: ignoreversion

Source: "Assets\*"; DestDir: "{app}\Assets"; Flags: ignoreversion recursesubdirs createallsubdirs

[Icons]
Name: "{group}\Ultimate Chess"; Filename: "{app}\chess.exe"

Name: "{autodesktop}\Ultimate Chess"; Filename: "{app}\chess.exe"; Tasks: desktopicon

[Run]
Filename: "{app}\chess.exe"; Description: "Launch Ultimate Chess"; Flags: nowait postinstall skipifsilent