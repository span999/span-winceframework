@echo on

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


:_EXIT

cd Lib\spLibSysInfoMemory
build -c
cd ..\..


cd Exe\spExeCPUUtilization
build -c
cd ..\..
