; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "YaFlight"
!define PRODUCT_VERSION "0.99.21"
!define PRODUCT_PUBLISHER "YaFlight"
!define PRODUCT_WEB_SITE "http://yaflight.altervista.org"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\yaflight.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"

SetCompressor lzma

; MUI 1.67 compatible ------
!include "MUI.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "..\workspace\yaflight\icons\yaflight-logo2013-32px.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Language Selection Dialog Settings
!define MUI_LANGDLL_REGISTRY_ROOT "${PRODUCT_UNINST_ROOT_KEY}"
!define MUI_LANGDLL_REGISTRY_KEY "${PRODUCT_UNINST_KEY}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "NSIS:Language"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "PRE_INSTALL_INFO.txt.txt"
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_RUN "$INSTDIR\yaflight.exe"
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.md"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "Czech"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "French"
!insertmacro MUI_LANGUAGE "Italian"

; Reserve files
!insertmacro MUI_RESERVEFILE_INSTALLOPTIONS

; MUI end ------

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "Setup.exe"
InstallDir "$PROGRAMFILES\YaFlight"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails show
ShowUnInstDetails show

Function .onInit
  !insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

Section "SezionePrincipale" SEC01
  SetOutPath "$INSTDIR"
  SetOverwrite ifnewer
  File "..\workspace\yaflight\release\yaflight.exe"
  CreateDirectory "$SMPROGRAMS\YaFlight"
  CreateShortCut "$SMPROGRAMS\YaFlight\YaFlight.lnk" "$INSTDIR\yaflight.exe"
  CreateShortCut "$DESKTOP\YaFlight.lnk" "$INSTDIR\yaflight.exe"
  File "yaflight-dlls\zlibwapi.dll"
  File "yaflight-dlls\ssleay32.dll"
  File "yaflight-dlls\QtXml4.dll"
  File "yaflight-dlls\QtWebKit4.dll"
  File "yaflight-dlls\QtSvg4.dll"
  File "yaflight-dlls\QtNetwork4.dll"
  File "yaflight-dlls\QtGui4.dll"
  File "yaflight-dlls\QtCore4.dll"
  File "yaflight-dlls\mingwm10.dll"
  File "yaflight-dlls\libssl32.dll"
  File "yaflight-dlls\libgcc_s_dw2-1.dll"
  File "yaflight-dlls\libeay32.dll"
  SetOverwrite try
  File /r "yaflight-dlls\iconengines"
  File /r "yaflight-dlls\imageformats"
  File /r "yaflight-dlls\languages"
  SetOverwrite ifnewer
  File "yaflight-dlls\yalib.dll"
  File /oname=$OUTDIR\README.txt "..\workspace\yaflight\README.md"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\YaFlight\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\YaFlight\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\yaflight.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\yaflight.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"
SectionEnd


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "$(^Name) � stato completamente rimosso dal tuo computer."
FunctionEnd

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Sei sicuro di voler completamente rimuovere $(^Name) e tutti i suoi componenti?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\README.md"
  Delete "$INSTDIR\yalib.dll"
  Delete "$INSTDIR\languages\*.qm"
  RMDir "$INSTDIR\languages"
  Delete "$INSTDIR\iconengines\*.dll"
  RMDir "$INSTDIR\iconengines"
  Delete "$INSTDIR\imageformats\*.dll"
  RMDir "$INSTDIR\imageformats"
  Delete "$INSTDIR\libeay32.dll"
  Delete "$INSTDIR\libgcc_s_dw2-1.dll"
  Delete "$INSTDIR\libssl32.dll"
  Delete "$INSTDIR\mingwm10.dll"
  Delete "$INSTDIR\QtCore4.dll"
  Delete "$INSTDIR\QtGui4.dll"
  Delete "$INSTDIR\QtNetwork4.dll"
  Delete "$INSTDIR\QtSvg4.dll"
  Delete "$INSTDIR\QtWebKit4.dll"
  Delete "$INSTDIR\QtXml4.dll"
  Delete "$INSTDIR\ssleay32.dll"
  Delete "$INSTDIR\zlibwapi.dll"
  Delete "$INSTDIR\yaflight.exe"

  Delete "$SMPROGRAMS\YaFlight\Uninstall.lnk"
  Delete "$SMPROGRAMS\YaFlight\Website.lnk"
  Delete "$DESKTOP\YaFlight.lnk"
  Delete "$SMPROGRAMS\YaFlight\YaFlight.lnk"

  RMDir "$SMPROGRAMS\YaFlight"
  RMDir "$INSTDIR"

  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd
