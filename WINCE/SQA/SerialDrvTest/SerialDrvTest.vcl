<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: SerialDrvTest - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/SerialDrvTest.res" /d UNDER_CE=420 /d _WIN32_WCE=420 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /r "D:\Xp-backup\eVC\CENET42\TestTools\SerialDrvTest\SerialDrvTest.rc"" 
Creating temporary file "C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSP3C.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/SerialDrvTest.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\Xp-backup\eVC\CENET42\TestTools\SerialDrvTest\ComTestClass.cpp"
"D:\Xp-backup\eVC\CENET42\TestTools\SerialDrvTest\SerialDrvTest.cpp"
"D:\Xp-backup\eVC\CENET42\TestTools\SerialDrvTest\spComXClass.cpp"
"D:\Xp-backup\eVC\CENET42\TestTools\SerialDrvTest\TestClass.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSP3C.tmp" 
Creating temporary file "C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSP3D.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/SerialDrvTest.pch" /Yc"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\Xp-backup\eVC\CENET42\TestTools\SerialDrvTest\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSP3D.tmp" 
Creating temporary file "C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSP3E.tmp" with contents
[
commctrl.lib coredll.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/SerialDrvTest.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4IRel/SerialDrvTest.exe" /subsystem:windowsce,4.20 /MACHINE:THUMB 
".\ARMV4IRel\ComTestClass.obj"
".\ARMV4IRel\SerialDrvTest.obj"
".\ARMV4IRel\spComXClass.obj"
".\ARMV4IRel\StdAfx.obj"
".\ARMV4IRel\TestClass.obj"
".\ARMV4IRel\SerialDrvTest.res"
]
Creating command line "link.exe @C:\DOCUME~1\Span.Liu\LOCALS~1\Temp\RSP3E.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
ComTestClass.cpp
SerialDrvTest.cpp
spComXClass.cpp
TestClass.cpp
Generating Code...
Linking...




<h3>Results</h3>
SerialDrvTest.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
