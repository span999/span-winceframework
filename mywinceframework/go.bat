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

cd Lib\spLibTouchHook
build -c
cd ..\..

cd Exe\spExeTouchHook
build -c
cd ..\..

cd Lib\spLibNumPadParser
build -c
cd ..\..

cd Dll\spDllTouchHook
build -c
cd ..\..

cd Dll\spDllTouchHookExt
build -c
cd ..\..

cd Exe\spExeTouchHook
build -c
cd ..\..

cd Lib\spLibARFileParser
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

cd Lib\spLibDataXferEvent
build
cd ..\..

cd Exe\spExeDataXferEvent
build
cd ..\..

:_EXIT


cd Lib\spLibRegNotify
build
cd ..\..

cd Exe\spExeRegNotify
build
cd ..\..
