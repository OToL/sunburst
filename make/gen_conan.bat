@echo off

where /q conan || echo Cound not find 'conan' in environment PATH. && goto:EOF

set build_folder=%~1

if "%build_folder%" == "" set build_folder=vs2022-win64

set curr_dir=%cd%
set build_dir=%curr_dir%\..\build\%build_folder%\

if NOT exist "%build_dir%" mkdir %build_dir%
cd %build_dir%

conan.exe install %curr_dir%

echo Conan generation succeeded
cd %curr_dir%
