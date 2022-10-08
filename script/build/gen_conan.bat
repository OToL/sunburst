@echo off
setlocal

where /q conan || echo Error: Cound not find 'conan' in environment PATH. && goto:EOF

set build_folder=%~1
set profile_file_name=%~2

if "%build_folder%" == "" echo Error: Build forder is not specified && goto:EOF
if "%profile_file_name%" == "" echo Error: Profile file name not specified && goto:EOF

set curr_dir=%cd%
set build_dir=%curr_dir%\..\..\build\%build_folder%\
set make_dir=%curr_dir%\..\..\make
if NOT exist "%build_dir%" mkdir %build_dir%
cd %build_dir%

conan.exe install %make_dir% -pr=%make_dir%\conan_profile_%profile_file_name%.txt 

echo Conan generation succeeded
cd %curr_dir%

