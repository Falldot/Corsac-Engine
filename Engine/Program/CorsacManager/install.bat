@echo off

set "Corsac=%ProgramFiles%/Corsac"

if not exist %Corsac% mkdir %Corsac%

if not exist "%Corsac%/bin" mkdir "%Corsac%/bin"
copy "Engine\Build\Editor\Manager\x64\Release\corsac.exe" "%Corsac%/bin"
copy "Engine\Build\Editor\Manager\x86\Release\corsac.exe" "%Corsac%/bin/corsac32.exe"

rem Устанавливаем заголовочные файлы STL

set "from=Engine\Source\Developer\STL\include\Corsac"
set "where=%Corsac%\include\Corsac"
if not exist %where% mkdir %where%
xcopy /seyr "%from%" "%where%\"

rem Устанавливаем заголовочные файлы SDL

set "from=Engine\Source\Dependencies\SDL\include"
set "where=%Corsac%\include\SDL"
if not exist %where% mkdir %where%
xcopy /seyr "%from%" "%where%\"

rem Устанавливаем SDL для x86 архитектур

set "from=Engine\Build\Dependencies\SDL\x86\Release"
set "where=%Corsac%\lib\x86\SDL"
if not exist "%where%" mkdir "%where%
xcopy /seyr "%from%" "%where%\"

rem Устанавливаем SDL для x86 архитектур

set "from=Engine\Build\Dependencies\SDL\x64\Release"
set "where=%Corsac%\lib\x64\SDL"
if not exist "%where%" mkdir "%where%
xcopy /seyr "%from%" "%where%\"

