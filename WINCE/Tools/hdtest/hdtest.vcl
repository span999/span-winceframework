<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: hdtest - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/hdtest.res" /d UNDER_CE=400 /d _WIN32_WCE=400 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /d "_AFXDLL" /r "D:\Tool\EVC4\Common\EVC\MyProjects\hdtest\hdtest.rc"" 
Creating temporary file "C:\DOCUME~1\SNOW~1.LIA\LOCALS~1\Temp\RSP2A2E.tmp" with contents
[
/nologo /W3 /D "ARM" /D "_ARM_" /D "ARMV4I" /D UNDER_CE=400 /D _WIN32_WCE=400 /D "WCE_PLATFORM_STANDARDSDK" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /D "_AFXDLL" /FR"ARMV4IRel/" /Fp"ARMV4IRel/hdtest.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\Tool\EVC4\Common\EVC\MyProjects\hdtest\hdtest.cpp"
"D:\Tool\EVC4\Common\EVC\MyProjects\hdtest\hdtestDlg.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\SNOW~1.LIA\LOCALS~1\Temp\RSP2A2E.tmp" 
Creating temporary file "C:\DOCUME~1\SNOW~1.LIA\LOCALS~1\Temp\RSP2A2F.tmp" with contents
[
/nologo /W3 /D "ARM" /D "_ARM_" /D "ARMV4I" /D UNDER_CE=400 /D _WIN32_WCE=400 /D "WCE_PLATFORM_STANDARDSDK" /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /D "_AFXDLL" /FR"ARMV4IRel/" /Fp"ARMV4IRel/hdtest.pch" /Yc"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\Tool\EVC4\Common\EVC\MyProjects\hdtest\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\SNOW~1.LIA\LOCALS~1\Temp\RSP2A2F.tmp" 
Creating temporary file "C:\DOCUME~1\SNOW~1.LIA\LOCALS~1\Temp\RSP2A30.tmp" with contents
[
/nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"wWinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/hdtest.pdb" /out:"ARMV4IRel/hdtest.exe" /subsystem:windowsce,4.00 /MACHINE:THUMB 
.\ARMV4IRel\hdtest.obj
.\ARMV4IRel\hdtestDlg.obj
.\ARMV4IRel\StdAfx.obj
.\ARMV4IRel\hdtest.res
]
Creating command line "link.exe @C:\DOCUME~1\SNOW~1.LIA\LOCALS~1\Temp\RSP2A30.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
hdtest.cpp
hdtestDlg.cpp
Generating Code...
D:\Tool\EVC4\Common\EVC\MyProjects\hdtest\hdtestDlg.cpp(338) : warning C4700: local variable 'sg' used without having been initialized
Linking...
Creating command line "bscmake.exe /nologo /o"ARMV4IRel/hdtest.bsc"  .\ARMV4IRel\StdAfx.sbr .\ARMV4IRel\hdtest.sbr .\ARMV4IRel\hdtestDlg.sbr"
Creating browse info file...
<h3>Output Window</h3>




<h3>Results</h3>
hdtest.exe - 0 error(s), 1 warning(s)
</pre>
</body>
</html>
