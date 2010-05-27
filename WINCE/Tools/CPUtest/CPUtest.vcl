<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: CPUtest - Win32 (WCE emulator) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"emulatorDbg/CPUtest.res" /d "WCE_PLATFORM_STANDARDSDK" /d UNDER_CE=400 /d _WIN32_WCE=400 /d "UNICODE" /d "_UNICODE" /d "DEBUG" /d "_X86_" /d "x86" /d "_i386_" /r "D:\mitac\Ruby\SW\app\CPUtest\CPUtest.rc"" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSPF4.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D "_i386_" /D UNDER_CE=400 /D _WIN32_WCE=400 /D "WCE_PLATFORM_STANDARDSDK" /D "i_386_" /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /Fp"emulatorDbg/CPUtest.pch" /Yu"stdafx.h" /Fo"emulatorDbg/" /Fd"emulatorDbg/" /Gs8192 /GF /c 
"D:\mitac\Ruby\SW\app\CPUtest\CPUtest.cpp"
]
Creating command line "cl.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSPF4.tmp" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSPF5.tmp" with contents
[
/nologo /W3 /Zi /Od /D "DEBUG" /D "_i386_" /D UNDER_CE=400 /D _WIN32_WCE=400 /D "WCE_PLATFORM_STANDARDSDK" /D "i_386_" /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /Fp"emulatorDbg/CPUtest.pch" /Yc"stdafx.h" /Fo"emulatorDbg/" /Fd"emulatorDbg/" /Gs8192 /GF /c 
"D:\mitac\Ruby\SW\app\CPUtest\StdAfx.cpp"
]
Creating command line "cl.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSPF5.tmp" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSPF6.tmp" with contents
[
commctrl.lib coredll.lib corelibc.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:yes /pdb:"emulatorDbg/CPUtest.pdb" /debug /nodefaultlib:"OLDNAMES.lib" /nodefaultlib:libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /out:"emulatorDbg/CPUtest.exe" /subsystem:windowsce,4.00 /MACHINE:IX86 
.\emulatorDbg\CPUtest.obj
.\emulatorDbg\StdAfx.obj
.\emulatorDbg\CPUtest.res
]
Creating command line "link.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSPF6.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
CPUtest.cpp
Linking...




<h3>Results</h3>
CPUtest.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
