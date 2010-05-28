@echo on

goto _EXIT

cd Exe\spExeKeepCPUBusy
build -c
cd ..\..

cd Exe\spExeIllegalMemoryAccess
build -c
cd ..\..

:_EXIT

cd Dll\spDllAutoRunner
build -c
cd ..\..


cd Exe\spExeAutoRunner
build -c
cd ..\..
