@echo off
setlocal

where /q cmake || echo Error: Cound not find 'cmake' in environment PATH. && goto:EOF

set build_folder=win64-vs2022-msvc

set curr_dir=%CD%
set batch_dir=%~dp0
set build_dir=%batch_dir%\..\..\build\%build_folder%\
set make_dir=%batch_dir%\..\..\make
if NOT EXIST "%build_dir%" mkdir %build_dir%

if NOT EXIST %build_dir% mkdir %build_dir%
cd %build_dir%

call %batch_dir%\gen_conan.bat %build_folder% windows
cmake ../../ -G "Visual Studio 17 2022" -DCMAKE_EXPORT_COMPILE_COMMANDS=1 --no-warn-unused-cli

cd %curr_dir%
