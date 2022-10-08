@echo off
setlocal

where /q cmake || echo Error: Cound not find 'cmake' in environment PATH. && goto:EOF

set build_folder=win64-ninja-clang

set curr_dir=%CD%
set batch_dir=%~dp0
set build_dir=%batch_dir%\..\..\build\%build_folder%\
set make_dir=%batch_dir%\..\..\make
if NOT exist "%build_dir%" mkdir %build_dir%

if not exist %build_dir% mkdir %build_dir%
cd %build_dir%

call %batch_dir%\gen_conan.bat %build_folder% windows
cmake ../../ -G Ninja -DCMAKE_TOOLCHAIN_FILE=%make_dir%\cmake_toolchain_win64_clang.cmake --no-warn-unused-cli

cd %curr_dir%

