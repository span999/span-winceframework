<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: SystemTool - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/SystemTool.res" /d UNDER_CE=420 /d _WIN32_WCE=420 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /r "D:\eVC\CENET42\TestTools\SystemTool\SystemTool.rc"" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP9C.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/SystemTool.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\eVC\CENET42\TestTools\SystemTool\CPU_Dlg.cpp"
"D:\eVC\CENET42\TestTools\SystemTool\CUnit_Dlg.cpp"
"D:\eVC\CENET42\TestTools\SystemTool\IPC_Dlg.cpp"
"D:\eVC\CENET42\TestTools\SystemTool\OSBench_Dlg.cpp"
"D:\eVC\CENET42\TestTools\SystemTool\SYSPOWER_Dlg.cpp"
"D:\eVC\CENET42\TestTools\SystemTool\SystemTool.cpp"
"D:\eVC\CENET42\TestTools\SystemTool\TOOLHELP_Dlg.cpp"
"D:\eVC\CENET42\TestTools\SystemTool\WinOverlay_Dlg.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP9C.tmp" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP9D.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/SystemTool.pch" /Yc"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\eVC\CENET42\TestTools\SystemTool\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP9D.tmp" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP9E.tmp" with contents
[
commctrl.lib coredll.lib spIPClib.lib WinOverlay_LIB.lib CEDDK.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/SystemTool.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4IRel/SystemTool.exe" /subsystem:windowsce,4.20 /MACHINE:THUMB 
.\ARMV4IRel\CPU_Dlg.obj
.\ARMV4IRel\CUnit_Dlg.obj
.\ARMV4IRel\IPC_Dlg.obj
.\ARMV4IRel\OSBench_Dlg.obj
.\ARMV4IRel\StdAfx.obj
.\ARMV4IRel\SYSPOWER_Dlg.obj
.\ARMV4IRel\SystemTool.obj
.\ARMV4IRel\TOOLHELP_Dlg.obj
.\ARMV4IRel\WinOverlay_Dlg.obj
.\ARMV4IRel\SystemTool.res
]
Creating command line "link.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP9E.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
CPU_Dlg.cpp
CUnit_Dlg.cpp
IPC_Dlg.cpp
OSBench_Dlg.cpp
SYSPOWER_Dlg.cpp
SystemTool.cpp
TOOLHELP_Dlg.cpp
WinOverlay_Dlg.cpp
Generating Code...
Linking...




<h3>Results</h3>
SystemTool.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
