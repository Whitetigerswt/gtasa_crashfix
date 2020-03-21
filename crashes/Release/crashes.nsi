;NSIS Modern User Interface
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

  !include "MUI2.nsh"
  !include 'nsdialogs.nsh'

!define MUI_ABORTWARNING_TEXT "Are you sure you wish to abort installation?"

SetCompressor /SOLID lzma

;--------------------------------
;General

  ;Name and file
  !define VERSION "v2.52"
  !define AUTHOR "Whitetiger"
  Name "GTA:SA Crash Fix-${VERSION}"
  OutFile "crashes-${VERSION}.exe"

  ;Default installation folder
  InstallDir "$PROGRAMFILES\Rockstar Games\GTA San Andreas"

  ;Get installation folder from registry if available
  InstallDirRegKey HKCU "Software\crashesasi" ""

  ;Request application privileges for Windows Vista
  RequestExecutionLevel admin

;--------------------------------
;Interface Settings

  !define MUI_ABORTWARNING

;--------------------------------
;Pages

  !insertmacro MUI_PAGE_LICENSE "License.txt"
  !insertmacro MUI_PAGE_COMPONENTS
  !define MUI_PAGE_CUSTOMFUNCTION_PRE DetermineGTADir
  !define MUI_PAGE_CUSTOMFUNCTION_LEAVE MyFuncDir
  !insertmacro MUI_PAGE_DIRECTORY
  !insertmacro MUI_PAGE_INSTFILES

  !insertmacro MUI_UNPAGE_CONFIRM
  !insertmacro MUI_UNPAGE_INSTFILES

;--------------------------------
;Languages

  !insertmacro MUI_LANGUAGE "English"

Function MyFuncDir

IfFileExists "$INSTDIR\gta_sa.exe" Exists DoesntExist

DoesntExist:
messagebox MB_YESNO|MB_ICONQUESTION "It doesn't look like GTA:SA is installed in this folder, $\nDo you want to continue with the installation anyway?" IDYES Exists

abort

Exists:

FunctionEnd

Function DetermineGTADir

ReadRegStr $0  HKCU "SOFTWARE\SAMP" 'gta_sa_exe'
Push "gta_sa.exe"
Push $0
call Slice
Pop $R0
StrCpy $INSTDIR $R0

FunctionEnd

;--------------------------------
;Installer Sections

!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\$(^Name)"
!define PRODUCT_WEBSITE "https://github.com/Whitetigerswt/gtasa_crashfix"

Section "Main Components" SecMain

	SectionIn 1 2 RO

  SetOutPath "$INSTDIR"

  File "crashes.asi"

  ;Store installation folder
  WriteRegStr HKCU "Software\crashesasi" "" $INSTDIR

  ;Create uninstaller
  WriteUninstaller "$INSTDIR\uninstall_crashes.exe"

  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninstall_crashes.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\uninstall_crashes.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEBSITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${AUTHOR}"

SectionEnd

Section "ASI Loader" SecASILoader

  SetOutPath "$INSTDIR"

  ;ADD YOUR OWN FILES HERE...
  File "vorbisHooked.dll"
  File "vorbisFile.dll"


SectionEnd

;--------------------------------
;Descriptions

  ;Language strings
  LangString DESC_SecMain ${LANG_ENGLISH} "All the required files."
  LangString DESC_SecASILoader ${LANG_ENGLISH} "ASI Loader."

  ;Assign language strings to sections
  !insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecMain} $(DESC_SecMain)
	!insertmacro MUI_DESCRIPTION_TEXT ${SecASILoader} $(DESC_SecASILoader)
  !insertmacro MUI_FUNCTION_DESCRIPTION_END


;--------------------------------
;Uninstaller Section

Section "Uninstall"

  Delete "$INSTDIR\crashes.asi"
  Delete "$INSTDIR\crashrpt_lang.ini"
  Delete "$INSTDIR\CrashRpt1402.dll"
  Delete "$INSTDIR\dbghelp.dll"
  Delete "$INSTDIR\CrashSender1402.exe"

  Delete "$INSTDIR\uninstall_crashes.exe"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"

  DeleteRegKey /ifempty HKCU "Software\crashesasi"

SectionEnd

Function Slice
 Exch $R0 ; input string
 Exch
 Exch $R1 ; to cut
 Push $R2
 Push $R3
 Push $R4
 Push $R5

 StrLen $R3 $R1
 StrCpy $R4 -1
 StrCpy $R5 0

 Loop:

  IntOp $R4 $R4 + 1
  StrCpy $R2 $R0 $R3 $R4
  StrCmp $R2 "" Done
  StrCmp $R2 $R1 0 Loop

   StrCpy $R5 1

   StrCmp $R4 0 0 +3
    StrCpy $R1 ""
    Goto +2
   StrCpy $R1 $R0 $R4
   StrLen $R2 $R0
   IntOp $R4 $R2 - $R4
   IntOp $R4 $R4 - $R3
   IntCmp $R4 0 0 0 +3
    StrCpy $R2 ""
    Goto +2
   StrCpy $R2 $R0 "" -$R4
   StrCpy $R0 $R1$R2

 Done:
 StrCpy $R1 $R5

 Pop $R5
 Pop $R4
 Pop $R3
 Pop $R2
 Exch $R1 ; slice? 0/1
 Exch
 Exch $R0 ; output string
FunctionEnd