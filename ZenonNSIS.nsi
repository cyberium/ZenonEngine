; The name of the installer
Name "ZenonEngineInstaller"

; The file to write
OutFile "ZenonEngineInstaller.exe"

; Build Unicode installer
Unicode True

; The default installation directory
InstallDir $PROGRAMFILES\ZenonEngine

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\ZenonEngine" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "Zenon Engine"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File "bin_x64\Release\*.dll"
  File "bin_x64\Release\*.exe"
  
  SetOutPath $INSTDIR\gamedata
  File /nonfatal /r "gamedata\"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\ZenonEngine "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ZenonEngine" "DisplayName" "Zenon Engine"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ZenonEngine" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ZenonEngine" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ZenonEngine" "NoRepair" 1
  WriteUninstaller "$INSTDIR\uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\Zenon Engine"
  CreateShortcut "$SMPROGRAMS\Zenon Engine\Uninstall.lnk" "$INSTDIR\uninstall.exe"
  CreateShortcut "$SMPROGRAMS\Zenon Engine\Editor.lnk" "$INSTDIR\Editor.exe"
  CreateShortcut "$SMPROGRAMS\Zenon Engine\Game.lnk" "$INSTDIR\Game.exe"
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\ZenonEngine"
  DeleteRegKey HKLM SOFTWARE\ZenonEngine

  ; Remove files and uninstaller
  Delete $INSTDIR\*.exe
  Delete $INSTDIR\*.dll
  Delete $INSTDIR\*.txt
  RMDir /r $INSTDIR\gamedata
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\Zenon Engine\*.lnk"

  ; Remove directories
  RMDir "$SMPROGRAMS\Zenon Engine"
  RMDir "$INSTDIR"

SectionEnd
