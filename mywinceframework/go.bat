@echo off

call makeVersion.bat


goto _EXIT

cd Exe\spExeKeepCPUBusy
build -c
cd ..\..

cd Exe\spExeIllegalMemoryAccess
build -c
cd ..\..

cd Dll\spDllAutoRunner
build -c
cd ..\..

cd Exe\spExeAutoRunner
build -c
cd ..\..

cd Lib\spLibSysInfoMemory
build -c
cd ..\..

cd Exe\spExeCPUUtilization
build -c
cd ..\..

cd Exe\spExeAutoRunner
build -c
cd ..\..

cd Dll\spDllPreRunner
build -c
cd ..\..

cd Dll\spDllAutoRunner
build -c
cd ..\..


:_EXIT

cd Dll\spDllTouchHook
build -c
cd ..\..

