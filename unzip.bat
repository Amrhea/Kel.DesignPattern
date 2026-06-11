@echo off
setlocal enabledelayedexpansion

set ZIP=
set DEST=

:loop
if "%~1"=="" goto run
if "%~1"=="-d" (
    set DEST=%~2
    shift
    shift
    goto loop
)
if "%~1"=="-qo" (
    set ZIP=%~2
    shift
    shift
    goto loop
)
if "!ZIP!"=="" (
    set ZIP=%~1
) else (
    if "!DEST!"=="" (
        set DEST=%~1
    )
)
shift
goto loop

:run
powershell -NoProfile -Command "Expand-Archive -Path '!ZIP!' -DestinationPath '!DEST!' -Force"
