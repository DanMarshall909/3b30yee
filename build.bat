@echo off
REM Cross-platform build script for TB-303 Emulator (Windows)
REM Usage: build.bat [Debug|Release]

setlocal EnableDelayedExpansion

set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release

set SCRIPT_DIR=%~dp0
set BUILD_DIR=%SCRIPT_DIR%build

echo Building TB-303 Emulator (%BUILD_TYPE%)...
echo Platform: Windows

REM Create build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
cd /d "%BUILD_DIR%"

REM Check for Visual Studio
where cl >nul 2>nul
if %ERRORLEVEL% neq 0 (
    echo Visual Studio not found in PATH. Please run from Developer Command Prompt.
    echo Or install Visual Studio with C++ workload.
    exit /b 1
)

REM Configure
echo Configuring CMake...
cmake .. -G "Visual Studio 16 2019" -A x64 -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    exit /b 1
)

REM Build
echo Building...
cmake --build . --config %BUILD_TYPE% --parallel
if %ERRORLEVEL% neq 0 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build completed successfully!
echo.
echo VST3 plugin: %BUILD_DIR%\TB303Emulator_artefacts\%BUILD_TYPE%\VST3\TB303Emulator.vst3
echo Standalone: %BUILD_DIR%\TB303Emulator_artefacts\%BUILD_TYPE%\Standalone\TB303Emulator.exe
echo.
echo To install VST3, copy to: C:\Program Files\Common Files\VST3\

endlocal