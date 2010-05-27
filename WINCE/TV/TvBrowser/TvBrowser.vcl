<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: TvBrowser - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/TvBrowser.res" /d UNDER_CE=420 /d _WIN32_WCE=420 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /r "D:\eVC\CENET42\TestTools\TvBrowser\TvBrowser.rc"" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP10.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/TvBrowser.pch" /Yu"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\eVC\CENET42\TestTools\TvBrowser\LogClass.cpp"
"D:\eVC\CENET42\TestTools\TvBrowser\SBuffer.cpp"
"D:\eVC\CENET42\TestTools\TvBrowser\TvBrowser.cpp"
"D:\eVC\CENET42\TestTools\TvBrowser\TvBrowserFun.cpp"
"D:\eVC\CENET42\TestTools\TvBrowser\TvOverlayFun.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP10.tmp" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP11.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/TvBrowser.pch" /Yc"stdafx.h" /Fo"ARMV4IRel/" /QRarch4T /QRinterwork-return /O2 /MC /c 
"D:\eVC\CENET42\TestTools\TvBrowser\StdAfx.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP11.tmp" 
Creating temporary file "C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP12.tmp" with contents
[
commctrl.lib coredll.lib uuid.lib dxguid.lib Ddraw.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/TvBrowser.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4IRel/TvBrowser.exe" /subsystem:windowsce,4.20 /MACHINE:THUMB 
.\ARMV4IRel\LogClass.obj
.\ARMV4IRel\SBuffer.obj
.\ARMV4IRel\StdAfx.obj
.\ARMV4IRel\TvBrowser.obj
.\ARMV4IRel\TvBrowserFun.obj
.\ARMV4IRel\TvOverlayFun.obj
.\ARMV4IRel\TvBrowser.res
]
Creating command line "link.exe @C:\DOCUME~1\spanliu\LOCALS~1\Temp\RSP12.tmp"
<h3>Output Window</h3>
Compiling resources...
Compiling...
StdAfx.cpp
Compiling...
LogClass.cpp
SBuffer.cpp
TvBrowser.cpp
TvBrowserFun.cpp
TvOverlayFun.cpp
Generating Code...
Linking...
LINK : warning LNK4089: all references to 'DDRAW.dll' discarded by /OPT:REF




<h3>Results</h3>
TvBrowser.exe - 0 error(s), 1 warning(s)
</pre>
</body>
</html>
