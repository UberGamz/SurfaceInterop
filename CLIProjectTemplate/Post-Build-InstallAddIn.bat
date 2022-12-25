@echo off
setlocal enabledelayedexpansion
REM ------------------------
REM C-Hook Add-In Post-Build
REM ------------------------
REM ----------------------------------------------------------------------------
REM The Post-Build Event in the Visual Studio project should be like this.
REM call Post-Build-Install.bat "$(ProjectName)" "$(Configuration)" "$(MastercamDir)"
REM ----------------------------------------------------------------------------
REM ------------------------------------------------------------------
REM Set the following line to "true" to enable script debug outputs.
REM ====================================
set _Post_Build_Debug_="false"
REM ====================================
REM ------------------------------------------------------------------
REM This copies the files from the project to the Mastercam folders.
REM The default destination starting with Mastercam 2022 is the user's
REM "My Mastercam 20##\Mastercam\Add-Ins" folder.
REM Or you can have this post-build use the "Shared Mastercam 20##Add-Ins"
REM folder if desired. See the _UseSharedAddInsFolder_ switch below.
REM Or you can have the post-build use the "chooks" folder if desired.
REM See the _UseCHooksFolder_ switch below.
REM ------------------------------------------------------------------
REM Depending on the _UseSharedAddInsFolder_ setting.
REM We need to locate the user's "My Mastercam 20##" folder,
REM or the Shared Mastercam 20## folder.
REM First, we look at the supplied MastercamDir path to find the "20##" version number.
REM This is used to build up the "path" for the Registry search that is
REM used to find the user's My Mastercam "Add-Ins" folder.
REM If we do not find what we are looking for in the MastercamDir path,
REM the setting _Mastercam_Version_ here will be used as a fall-back.
REM ====================================
set _Mastercam_Version_="Mastercam 2024"
REM ====================================
REM ------------------------------------------------------------------
REM If you wish to force use the "chooks" folder as the destination,
REM set this _UseCHooksFolder switch to "true".
REM If the found or set Mastercam_Version is less than '2022',
REM this script automatically forces _UseCHooksFolder_="true"
REM NOTE: Copying to the 'chooks' folder may fail!
REM If administrator privileges are required and
REM you do not have those privileges, or
REM Visual Studio is not running "as admin".
REM ====================================
set _UseCHooksFolder_="false"
REM ====================================
REM ------------------------------------------------------------------
REM If not using the "chooks" folder as the destination,
REM you can choose between the "Shared Mastercam 20##" folder,
REM or the "My Mastercam 20##\Mastercam\Add-Ins" folder.
REM ====================================
set _UseSharedAddInsFolder_="true"
REM ====================================
REM ------------------------------------------------------------------
REM If you do want to place your add-in files in a "project name"
REM sub-folder of the destination folder, set this option to "true".
REM NOTE: If this is "true" you may need to adjust the paths to your
REM add-in's DLL in the (FT) Function Table file of your add-in.
REM ====================================
set _UseSubFolder_="false"
REM ====================================

REM Display the name of the batch file.
echo.
set _POST_BUILD_=%0
echo !_POST_BUILD_:.bat=!

REM Using the "~" format to strip any enclosing quotes.
set _ProjectName_=%~1
set _Configuration_=%~2
set _MastercamDir_=%~3

if [!_Post_Build_Debug_!] == ["true"] (
echo.
echo *DEBUG* --- Command arguments --- *DEBUG*
echo arg1 -^> !_ProjectName_!
echo arg2 -^> !_Configuration_!
echo arg3 -^> !_MastercamDir_!
echo.
)

REM Look at the MastercamDir path to find the 20## version number.
call :FindVersion "!_MastercamDir_!"
if not ["!_McVersion_!"] == [""] (
set _Mastercam_Version_=!_McVersion_!
if [!_Post_Build_Debug_!] == ["true"] (
echo *DEBUG* - Found Version -^> !_Mastercam_Version_!
)
)

REM If the version < 2022, set the UseCHooksFolder option to be "true"
if !_Mastercam_Version_! lss 2022 (
set _UseCHooksFolder_="true"
echo Mastercam Version less than 2022, forcing _UseCHooksFolder_ option to be "true"
)

if [!_UseCHooksFolder_!] == ["true"] (
set _Mastercam_AddIns_Path_="!_MastercamDir_!chooks\"
) else (
REM If we are not using the CHooks folder,
REM look into the Registry to find the path to either the user's
REM My Mastercam 202# folder, or the Shared Mastercam 202# folder.
if !_Mastercam_Version_! lss 2024 (
set _Mastercam_RegKey_="HKEY_CURRENT_USER\SOFTWARE\CNC Software, Inc.\Mastercam !_Mastercam_Version_:"=!"
) else (
    set _Mastercam_RegKey_="HKEY_CURRENT_USER\SOFTWARE\CNC Software\Mastercam !_Mastercam_Version_:"=!"
)
)

if [!_UseSharedAddInsFolder_!] == ["true"] (
set _ValueName_="SharedDir"
) else (
set _ValueName_="UserDir"
)
)

if [!_Post_Build_Debug_!] == ["true"] (
echo *DEBUG* - Searching: !_Mastercam_RegKey_!
echo *DEBUG* - For Value: !_ValueName_!
)

reg query !_Mastercam_RegKey_! /v !_ValueName_! >nul 2>&1
if !ERRORLEVEL! == 0  (
for /F "usebackq tokens=2,* skip=2" %%L in (
     `reg query !_Mastercam_RegKey_! /v !_ValueName_!`
     ) do set "_Mastercam_AddIns_Path_=%%M"
)

REM If we did not find the path in the Registry, default to using the CHooks folder.
if ["!_Mastercam_AddIns_Path_!"] == [""] (
set _Mastercam_AddIns_Path_="!_MastercamDir_!chooks\"
set _UseCHooksFolder_="true"
echo ***********************
echo Post-Build-InstallAddIn : warning : Value not found in Registry^^! -^> !_Mastercam_RegKey_:"=!\!_ValueName_:"=!
echo Forcing the _UseCHooksFolder_ option to "true"
echo Now using path -^> !_Mastercam_AddIns_Path_!
echo ***********************
)

set _TargetFolder_=!_Mastercam_AddIns_Path_!

REM Verify the 'base' folder.
call :IsDirectory "!_TargetFolder_!"
if not [!_Is_Folder_!] == ["true"] (
echo ***********************
echo Post-Build-InstallAddIn : error : Invalid Target Folder^^! -^> !_TargetFolder_!
echo ***********************
REM exit with error code
echo.
exit /b 1
)

REM If NOT using the chooks folder, append the 'Add-ins' folder to the path.
if not [!_UseCHooksFolder_!] == ["true"] (
set _TargetFolder_="!_TargetFolder_!Add-Ins\"
)

REM Append the sub-folder if requested.
if [!_UseSubFolder_!] == ["true"] (
set _TargetFolder_=!_TargetFolder_:"=!!_ProjectName_!
)

if [!_Post_Build_Debug_!] == ["true"] (
echo *DEBUG* - _TargetFolder_ -^> !_TargetFolder_!
)

if not exist !_TargetFolder_! (
mkdir !_TargetFolder_!
if !ERRORLEVEL! == 0 (
echo Created folder: !_TargetFolder_!
) else (
echo ***********************
echo Post-Build-InstallAddIn : error : Create folder failed^^! -^> !_TargetFolder_!
echo ***********************
REM exit with error code
echo.
exit /b 1
)
)

REM Copy the project's DLL file.
set _DLL_SOURCE_PATH_="x64\!_Configuration_!\!_ProjectName_!.dll"
set _DLL_TARGET_PATH_="!_TargetFolder_:"=!!_ProjectName_!.dll"
copy !_DLL_SOURCE_PATH_! !_DLL_TARGET_PATH_! > nul 2>&1
if !ERRORLEVEL! == 0 (
echo Copied: !_DLL_SOURCE_PATH_! -^> !_DLL_TARGET_PATH_!
) else (
echo ***********************
echo Post-Build-InstallAddIn : error : Copy Failed^^! !_DLL_SOURCE_PATH_! -^> !_DLL_TARGET_PATH_!
echo ***********************
REM exit with error code
echo.
exit /b 1
)

REM Copy the project's FT file. (if one exists)
if exist "!_ProjectName_!.ft" (
set _FT_TARGET_PATH_="!_TargetFolder_:"=!!_ProjectName_!.ft"
copy "!_ProjectName_!.ft" !_FT_TARGET_PATH_! > nul 2>&1
if !ERRORLEVEL! == 0 (
echo Copied: "!_ProjectName_!.ft" -^> !_FT_TARGET_PATH_!
) else (
echo ***********************
echo Post-Build-InstallAddIn : error : Copy Failed^^! "!_ProjectName_!.ft" -^> !_FT_TARGET_PATH_!
echo ***********************
REM exit with error code
echo.
exit /b 1
)
)

REM Exit - Success
echo.
exit /b 0

REM ----------------------------------------------------------
REM Search through the path to find the "20##" version number.
REM ----------------------------------------------------------
:FindVersion

set _PathString_=%~1

:TokenLoop
for /f "tokens=1,* delims=\" %%G in ("!_PathString_!") do (
set "_PathString_=%%H"
set _Token_=%%G
echo.!_Token_! | findstr /R /C:"Mastercam 20[0-9][0-9]" >nul 2>&1 && (
set _McVersion_=!_Token_!_McVersion_=!_Token_!
set _McVersion_=!_McVersion_:~-4!
exit /b
)
)
if defined _PathString_ ( goto :TokenLoop )

if [!_Post_Build_Debug_!] == ["true"] (
echo ---- FindVersion ----
echo *DEBUG* - Searching -^> !_PathString_!
echo *DEBUG* - Found version -^> !_McVersion_!
echo ---------------------
exit /b
REM End of 'FindVersion' sub

REM --------------------------------------------------------
REM Check that the supplied path is an existing folder.
REM --------------------------------------------------------
:IsDirectory
if [!_Post_Build_Debug_!] == ["true"] (
echo ---- IsDirectory ----
echo IsDirectory is -^> "%~1"
echo ---------------------
)
set _Attributes_=%~a1
set _Dir_Attribute_=!_Attributes_:~0,1!
if /I "!_Dir_Attribute_!"=="d" (
set _Is_Folder_="true"
) else (
set _Is_Folder_="false"
)
exit /b
REM End of 'IsDirectory' sub
