echo off
echo ========= reset folder =========

set STOAGEBASE=d:\workshop\storge
set RESETFOLDERNAME=%1

if "%RESETFOLDERNAME%" == "" (
    set RESETFOLDERNAME=STA2064
    )

echo we are going to reset "%RESETFOLDERNAME%" folder
echo from base %STOAGEBASE%


rmdir /q /s %RESETFOLDERNAME%
7z x %STOAGEBASE%\%RESETFOLDERNAME%.zip

REM rmdir /q /s Hammer
REM 7z x Hammer.zip


echo ========= done !!!!!!! =========
