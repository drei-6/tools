@echo off
cd %~dp0

pushd %cd%
cd ..
set project_directory=%cd%
popd

rmdir /S /Q "%project_directory%\build"
mkdir "%project_directory%\build"

set common_compiler_flags=/MTd /nologo /Od /W4 /Z7 /wd4100 /wd4101 /wd4189 /wd4201 /I"%project_directory%\source"
set common_linker_flags=/DEBUG:FASTLINK /INCREMENTAL:NO /OPT:REF

pushd "%project_directory%\build"
cls
if %errorlevel% == 0 (cl %common_compiler_flags% /Fo"rpg.obj" "%project_directory%\source\rpg\windows_main.c" /link %common_linker_flags% "kernel32.lib")
popd