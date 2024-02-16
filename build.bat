@echo off

:menu
echo Iniciando o build da aplicacao
echo 0. iniciar voxels
echo 1. load Monkey.obj
echo 2. load Axis.obj
echo 3. load Block.obj
echo 4. load Icosphere.obj
echo 5. load Mineterrain.obj
echo 6. load Montain.obj

set /p option=opcao:

if "%option%"== "0" (
    cmake -DLOADOBJ=0 -B build
) else if "%option%" == "1" (
    cmake -DLOADOBJ=1 -B build
) else if "%option%" == "2" (
    cmake -DLOADOBJ=2 -B build
) else if "%option%" == "3" (
    cmake -DLOADOBJ=3 -B build
) else if "%option%" == "4" (
    cmake -DLOADOBJ=4 -B build
) else if "%option%" == "5" (
    cmake -DLOADOBJ=5 -B build
) else if "%option%" == "6" (
    cmake -DLOADOBJ=6 -B build
)

cmake --build build

pause
