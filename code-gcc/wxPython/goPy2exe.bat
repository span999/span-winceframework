REM
REM usage : goPy2exe.bat xxxx.py
REM

@echo on

set _PythonDir=c:\Python27\
set _PythonExe=python.exe
set _FILE=%1

echo "%_PythonDir%%_PythonExe% %_FILE%"

rmdir /S /Q build
rmdir /S /Q dist
del /S /Q target.py
copy %_FILE% target.py
%_PythonDir%%_PythonExe% goPy2exe.py py2exe
