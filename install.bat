@echo off
cls
echo ===========================================
echo     INSTALADOR DEL SISTEMA DE TURNOS
echo ===========================================

:: Crear carpeta bin si no existe
if not exist "bin" mkdir bin
if not exist "bin\backups" mkdir bin\backups

echo.
echo Compilando programa...
g++ -o bin\main.exe ^
src\main.cpp src\Menu.cpp src\ListaDoble.cpp src\listaPacientes.cpp ^
src\FechaHora.cpp src\Paciente.cpp src\Turno.cpp ^
-Iinclude ^
-Lbin -lValidarEntrada -lvalidarFeriado

if %errorlevel% neq 0 (
    echo ERROR: Fallo la compilacion.
    pause
    exit /b
)

echo.
echo Instalacion completada con exito.
echo.

echo ===========================================
echo      CODIGOS DE LICENCIA GENERADOS
echo ===========================================
echo.

setlocal EnableDelayedExpansion
set "chars=0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

> bin\codigos_licencia.txt (
    echo ===== CODIGOS DE LICENCIA =====
)

for /L %%j in (1,1,5) do (
    set "part1="
    set "part2="
    set "part3="

    for /L %%i in (1,1,4) do (
        set /a "rand=!random! %% 36"
        for %%r in (!rand!) do set "part1=!part1!!chars:~%%r,1!"
    )
    for /L %%i in (1,1,4) do (
        set /a "rand=!random! %% 36"
        for %%r in (!rand!) do set "part2=!part2!!chars:~%%r,1!"
    )
    for /L %%i in (1,1,4) do (
        set /a "rand=!random! %% 36"
        for %%r in (!rand!) do set "part3=!part3!!chars:~%%r,1!"
    )

    set "codigo=!part1!-!part2!-!part3!"
    echo Codigo %%j: !codigo!
    echo !codigo! >> bin\codigos_licencia.txt
)

echo.
echo ===========================================
echo Guarda tus codigos para activacion/registro.
echo (tambien se han guardado en bin\codigos_licencia.txt)
echo ===========================================
echo.

:: Ejecutar el programa si se generó correctamente
if exist "bin\main.exe" (
    echo Ejecutando el sistema...
    start "" bin\main.exe
) else (
    echo ERROR: El ejecutable no se encontro.
)

cd bin
cd ..
pause
