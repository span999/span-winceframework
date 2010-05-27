<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: GPIOview - Win32 (WCE ARMV4) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP337.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=420 /D "WIN32_PLATFORM_WFSP=200" /D "ARM" /D "_ARM_" /D "ARMV4" /D UNDER_CE=420 /D "UNICODE" /D "_UNICODE" /D "NDEBUG" /Fp"ARMV4Rel/GPIOview.pch" /YX /Fo"ARMV4Rel/" /O2 /MC /c 
"C:\xp-backup\eVC\test2003\GPIOview\GPIOview.cpp"
]
Creating command line "clarm.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP337.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP338.tmp" with contents
[
commctrl.lib coredll.lib aygshell.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"ARMV4Rel/GPIOview.pdb" /nodefaultlib:"libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib" /out:"ARMV4Rel/GPIOview.exe" /subsystem:windowsce,4.20 /align:"4096" /MACHINE:ARM 
".\ARMV4Rel\GPIOview.obj"
".\ARMV4Rel\misc.obj"
".\ARMV4Rel\GPIOview.res"
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP338.tmp"
<h3>Output Window</h3>
Compiling...
GPIOview.cpp
c:\xp-backup\evc\test2003\gpioview\gpioview.cpp(377) : warning C4101: 'iLoop' : unreferenced local variable
Linking...
Signing C:\xp-backup\eVC\test2003\GPIOview\ARMV4Rel\GPIOview.exe
Warning: This file is signed, but not timestamped.
Succeeded




<h3>Results</h3>
GPIOview.exe - 0 error(s), 1 warning(s)
</pre>
</body>
</html>
