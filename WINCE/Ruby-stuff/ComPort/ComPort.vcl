<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: ComPort - Win32 (WCE ARMV4) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4Rel/ComPort.res" /d UNDER_CE=420 /d _WIN32_WCE=420 /d "NDEBUG" /d "UNICODE" /d "_UNICODE" /d "WCE_PLATFORM_STANDARDSDK" /d "ARM" /d "_ARM_" /d "ARMV4" /r "C:\XP-BACKUP\EVC\CENET42\ComPort\ComPort.rc"" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP18E1.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "WCE_PLATFORM_STANDARDSDK" /D "ARM" /D "_ARM_" /D "ARMV4" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4Rel/ComPort.pch" /Yu"stdafx.h" /Fo"ARMV4Rel/" /O2 /MC /c 
"C:\XP-BACKUP\EVC\CENET42\ComPort\ComPort.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP18E1.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP18E2.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "WCE_PLATFORM_STANDARDSDK" /D "ARM" /D "_ARM_" /D "ARMV4" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4Rel/ComPort.pch" /Yc"stdafx.h" /Fo"ARMV4Rel/" /O2 /MC /c 
"C:\XP-BACKUP\EVC\CENET42\ComPort\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP18E2.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP18E3.tmp" with contents
[
commctrl.lib coredll.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4Rel/ComPort.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4Rel/ComPort.exe" /subsystem:windowsce,4.20 /align:"4096" /MACHINE:ARM 
".\ARMV4Rel\ComPort.obj"
".\ARMV4Rel\StdAfx.obj"
".\ARMV4Rel\ComPort.res"
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP18E3.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
ComPort.cpp
Linking...




<h3>Results</h3>
ComPort.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
