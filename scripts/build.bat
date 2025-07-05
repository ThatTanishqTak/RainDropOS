@echo off
REM Build and package RainDropOS
set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%.."
if "%1"=="" (
    make iso
) else (
    make %*
)

pause