<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: IPCtest - Win32 (WCE emulator) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"emulatorRel/IPCtest.res" /d UNDER_CE=420 /d _WIN32_WCE=420 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "_X86_" /d "x86" /d "_i386_" /r "D:\eVC\CENET42\TestTools\IPCtest\IPCtest.rc"" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP5.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "WCE_PLATFORM_STANDARDSDK" /D "_i386_" /D UNDER_CE=420 /D "i_386_" /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "NDEBUG" /Fp"emulatorRel/IPCtest.pch" /Yu"stdafx.h" /Fo"emulatorRel/" /Gs8192 /GF /O2 /c 
"D:\eVC\CENET42\TestTools\IPCtest\IPCtest.cpp"
]
Creating command line "cl.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP5.tmp" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP6.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "WCE_PLATFORM_STANDARDSDK" /D "_i386_" /D UNDER_CE=420 /D "i_386_" /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "NDEBUG" /Fp"emulatorRel/IPCtest.pch" /Yc"stdafx.h" /Fo"emulatorRel/" /Gs8192 /GF /O2 /c 
"D:\eVC\CENET42\TestTools\IPCtest\StdAfx.cpp"
]
Creating command line "cl.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP6.tmp" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP7.tmp" with contents
[
commctrl.lib coredll.lib corelibc.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"emulatorRel/IPCtest.pdb" /nodefaultlib:"OLDNAMES.lib" /nodefaultlib:libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /out:"emulatorRel/IPCtest.exe" /subsystem:windowsce,4.20 /MACHINE:IX86 
.\emulatorRel\IPCtest.obj
.\emulatorRel\StdAfx.obj
.\emulatorRel\IPCtest.res
]
Creating command line "link.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP7.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
IPCtest.cpp
Linking...




<h3>Results</h3>
IPCtest.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
