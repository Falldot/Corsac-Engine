@echo off

rem -------------------------------------
rem Corsac Manager Build
rem -------------------------------------

set "ManagerSrc=Engine\Source\Editor\Manager"
set "ManagerOut=Engine\Build\Editor\Manager"

if not exist "%ManagerOut%/x86" mkdir "%ManagerOut%/x86"
cmd /c "cd %ManagerOut%/x86 && cmake -A Win32 -S %CD%/%ManagerSrc% && cmake --build . --config Release

if not exist "%ManagerOut%/x64" mkdir "%ManagerOut%/x64"
cmd /c "cd %ManagerOut%/x64 && cmake -A x64 -S %CD%/%ManagerSrc% && cmake --build . --config Release

