@echo off

rem -------------------------------------
rem SDL Build
rem -------------------------------------

set "SDLSrc=Engine\Source\Dependencies\SDL"
set "SDLOut=Engine\Build\Dependencies\SDL"

rem Собираем SDL для x86 архитектур
if not exist "%SDLOut%/x86" mkdir "%SDLOut%/x86"
cmd /c "cd %SDLOut%/x86 && cmake -A Win32 -S %CD%/%SDLSrc% && cmake --build . --config Release"

rem Собираем SDL для x64 архитектур
if not exist "%SDLOut%/x64" mkdir "%SDLOut%/x64"
cmd /c "cd %SDLOut%/x64 && cmake -A x64 -S %CD%/%SDLSrc% && cmake --build . --config Release"