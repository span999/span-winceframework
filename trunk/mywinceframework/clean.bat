
echo off

echo "Clean redundant files"

del *.bak /s
del *.bif /s

del Build.log /s
del Build.wrn /s
del Build.err /s


REM use below for command line input
REM FOR /f "tokens=*" %i IN ('dir /b /ad /s *.svn*') DO rd /s /q %i
REM

FOR /f "tokens=*" %%i IN ('dir /b /ad /s *.svn*') DO rd /s /q %%i

FOR /f "tokens=*" %%i IN ('dir /b /ad /s *obj*') DO rd /s /q %%i