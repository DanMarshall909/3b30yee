@echo off
REM Installation script for TB-303 Emulator (Windows)
REM Usage: install.bat [Debug|Release]

setlocal EnableDelayedExpansion

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%build

echo Installing TB-303 Emulator (%BUILD_TYPE%)...

REM Check if build exists
if not exist "%BUILD_DIR%\TB303Emulator_artefacts\%BUILD_TYPE%" (
    echo Build not found. Please run build.bat first.
    exit /b 1
)

set VST3_SRC=%BUILD_DIR%\TB303Emulator_artefacts\%BUILD_TYPE%\VST3\TB303Emulator.vst3
set VST3_DEST=C:\Program Files\Common Files\VST3\

echo Installing VST3 to: %VST3_DEST%

REM Check if we can write to Program Files (requires admin)
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
if '%errorlevel%' NEQ '0' (
    echo Requesting administrator privileges...
    goto UACPrompt
) else (
    goto gotAdmin
)

:UACPrompt
echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
echo UAC.ShellExecute "%~s0", "", "", "runas", 1 >> "%temp%\getadmin.vbs"
"%temp%\getadmin.vbs"
del "%temp%\getadmin.vbs"
exit /B

:gotAdmin
pushd "%CD%"
CD /D "%~dp0"

REM Create VST3 directory if it doesn't exist
if not exist "%VST3_DEST%" mkdir "%VST3_DEST%"

REM Copy VST3 plugin
if exist "%VST3_SRC%" (
    xcopy /E /I /Y "%VST3_SRC%" "%VST3_DEST%TB303Emulator.vst3\"
    echo VST3 installed successfully!
) else (
    echo VST3 build not found: %VST3_SRC%
    exit /b 1
)

echo.
echo Installation completed!
echo Restart your DAW to detect the new plugin.

endlocal
pause