<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: battDrvTest - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/battDrvTest.res" /d UNDER_CE=420 /d _WIN32_WCE=420 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /r "D:\Xp-backup\eVC\CENET42\battDrvTest\battDrvTest.rc"" 
Creating temporary file "C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSPD2.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/battDrvTest.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\Xp-backup\eVC\CENET42\battDrvTest\battDrvTest.cpp"
"D:\Xp-backup\eVC\CENET42\battDrvTest\BattTestClass.cpp"
"D:\Xp-backup\eVC\CENET42\battDrvTest\TestClass.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSPD2.tmp" 
Creating temporary file "C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSPD3.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/battDrvTest.pch" /Yc"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\Xp-backup\eVC\CENET42\battDrvTest\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSPD3.tmp" 
Creating temporary file "C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSPD4.tmp" with contents
[
commctrl.lib coredll.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/battDrvTest.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4IRel/battDrvTest.exe" /subsystem:windowsce,4.20 /MACHINE:THUMB 
".\ARMV4IRel\battDrvTest.obj"
".\ARMV4IRel\BattTestClass.obj"
".\ARMV4IRel\StdAfx.obj"
".\ARMV4IRel\TestClass.obj"
".\ARMV4IRel\battDrvTest.res"
]
Creating command line "link.exe @C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSPD4.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
battDrvTest.cpp
BattTestClass.cpp
TestClass.cpp
Generating Code...
Linking...




<h3>Results</h3>
battDrvTest.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
