<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: SystemInfo - Win32 (WCE ARMV4I) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"ARMV4IRel/SysInfo.res" /d UNDER_CE=420 /d _WIN32_WCE=420 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "THUMB" /d "_THUMB_" /d "ARM" /d "_ARM_" /d "ARMV4I" /r -n "N:\MIO-3G\SystemInfo\SysInfo.rc"" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP948.tmp" with contents
[
/nologo /W3 /GX- /Oxs /D _WIN32_WCE=420 /D "ARM" /D "_ARM_" /D "WCE_PLATFORM_STANDARDSDK" /D "ARMV4I" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4IRel/SystemInfo.pch" /YX /Fo"ARMV4IRel/" /Gs8192 /GF /c 
"N:\MIO-3G\SystemInfo\SysInfo.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP948.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP949.tmp" with contents
[
commctrl.lib coredll.lib corelibc.lib aygshell.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4IRel/SystemInfo.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4IRel/SystemInfo.exe" /subsystem:windowsce,4.20 /MACHINE:IX86 
".\ARMV4IRel\SysInfo.obj"
".\ARMV4IRel\SysInfo.res"
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP949.tmp"
<h3>Output Window</h3>
Compiling resources...
N:\MIO-3G\SystemInfo\newres.h(48) : fatal error RC1015: cannot open include file 'winver.h'.
Error executing rc.exe.



<h3>Results</h3>
SystemInfo.exe - 1 error(s), 0 warning(s)
</pre>
</body>
</html>
