@echo off
setlocal

where /q cmake || echo Error: Cound not find 'cmake' in environment PATH. && goto:EOF

set build_folder=win64-ninja-clang

set curr_dir=%CD%
set batch_dir=%~dp0
set clangd_file=%batch_dir%\..\..\.clangd
set build_dir=%batch_dir%\..\..\build\%build_folder%\
set make_dir=%batch_dir%\..\..\make
if NOT EXIST "%build_dir%" mkdir %build_dir%

if NOT EXIST %build_dir% mkdir %build_dir%
cd %build_dir%

call %batch_dir%\gen_conan.bat %build_folder% windows
cmake ../../ -G Ninja -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_TOOLCHAIN_FILE=%make_dir%\cmake_toolchain_win64_clang.cmake --no-warn-unused-cli

cd %curr_dir%

if EXIST %batch_dir%\..\..\.clangd DEL /F %batch_dir%\..\..\.clangd

echo CompileFlags: > %clangd_file%
echo    CompilationDatabase: build/%build_folder% >> %clangd_file%
echo    Add: [-Wno-unused-macros] >> %clangd_file% 
