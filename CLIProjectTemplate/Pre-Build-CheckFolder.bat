@echo off
REM ------------------------
REM C-Hook Add-In Pre-Build
REM ------------------------
setlocal enabledelayedexpansion
REM ----------------------------------------------------------
REM The Pre-Build Event in the Visual Studio project will be.
REM call Pre-Build-CheckFolder.bat "$(SDKDir)"
REM ----------------------------------------------------------
REM echo --- DEBUG - show command arguments ---
REM echo arg1 -^> %1

REM Display the name of the batch file.
echo.
set _PREBUILD_=%0
echo %_PREBUILD_:.bat=%

REM Using the "~" format to strip any enclosing quotes.
set _SDK_DIR_=%~1

echo Checking SDK folder-^> !_SDK_DIR_!
call :IsDirectory "!_SDK_DIR_!"
if not [!_Is_Folder_!] == ["true"] (
echo ==========================================================
echo Pre-Build-CheckFolder : error : SDK folder does not exist^^!
echo %1
echo Check the 'SDKDir' path in MastercamSDK.props
echo ==========================================================
echo.
exit /b -1
)

set _LAST_CHAR_=!_SDK_DIR_:~-1!
if not "!_LAST_CHAR_!"=="\" (
echo ========================================================================
echo Pre-Build-CheckFolder : error : SDK folder path does not end with a '\'.
echo %1
echo Check the 'SDKDir' path in MastercamSDK.props
echo ========================================================================
echo.
exit /b -1
)

if not exist "!_SDK_DIR_!m_mastercam.h" (
echo ===============================================================================
echo Pre-Build-CheckFolder : error : SDK folder path does not contain 'm_mastercam.h'
echo %1
echo Check that the 'SDKDir' path in MastercamSDK.props points to a valid SDK.
echo ===============================================================================
echo.
exit /b -1
 )

REM Exit - Success
echo.
exit /b 0

REM ---------------------------------------------------
REM Check that the supplied path is an existing folder.
REM ---------------------------------------------------
:IsDirectory
set _Attributes_=%~a1
set _Dir_Attribute_=!_Attributes_:~0,1!
if /I "!_Dir_Attribute_!"=="d" (
set _Is_Folder_="true"
) else (
set _Is_Folder_="false"
)
exit /b
