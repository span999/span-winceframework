<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: StorageTest - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/StorageTest.res" /d UNDER_CE=400 /d _WIN32_WCE=400 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /r "G:\xp-backup\eVC\CENET42\StorageTest\StorageTest.rc"" 
Creating temporary file "C:\DOCUME~1\span.liu\LOCALS~1\Temp\RSP1A28.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=400 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=400 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /FR"ARMV4IRel/" /Fp"ARMV4IRel/StorageTest.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"G:\xp-backup\eVC\CENET42\StorageTest\HDDtest_Dlg.cpp"
"G:\xp-backup\eVC\CENET42\StorageTest\QLPerf_Dlg.cpp"
"G:\xp-backup\eVC\CENET42\StorageTest\StorageTest.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\span.liu\LOCALS~1\Temp\RSP1A28.tmp" 
Creating temporary file "C:\DOCUME~1\span.liu\LOCALS~1\Temp\RSP1A29.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=400 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=400 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /FR"ARMV4IRel/" /Fp"ARMV4IRel/StorageTest.pch" /Yc"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"G:\xp-backup\eVC\CENET42\StorageTest\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\span.liu\LOCALS~1\Temp\RSP1A29.tmp" 
Creating temporary file "C:\DOCUME~1\span.liu\LOCALS~1\Temp\RSP1A2A.tmp" with contents
[
Coredll.lib commctrl.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/StorageTest.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4IRel/StorageTest.exe" /subsystem:windowsce,4.00 /MACHINE:THUMB 
".\ARMV4IRel\HDDtest_Dlg.obj"
".\ARMV4IRel\QLPerf_Dlg.obj"
".\ARMV4IRel\StdAfx.obj"
".\ARMV4IRel\StorageTest.obj"
".\ARMV4IRel\StorageTest.res"
]
Creating command line "link.exe @C:\DOCUME~1\span.liu\LOCALS~1\Temp\RSP1A2A.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
HDDtest_Dlg.cpp
QLPerf_Dlg.cpp
StorageTest.cpp
Generating Code...
Linking...
Creating command line "bscmake.exe /nologo /o"ARMV4IRel/StorageTest.bsc"  ".\ARMV4IRel\StdAfx.sbr" ".\ARMV4IRel\HDDtest_Dlg.sbr" ".\ARMV4IRel\QLPerf_Dlg.sbr" ".\ARMV4IRel\StorageTest.sbr""
Creating browse info file...
<h3>Output Window</h3>




<h3>Results</h3>
StorageTest.exe - 0 error(s), 0 warning(s)
</pre>
</body>
</html>
