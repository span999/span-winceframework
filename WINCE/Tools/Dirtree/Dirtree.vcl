<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: Dirtree - Win32 (WCE emulator) Release--------------------
</h3>
<h3>Command Lines</h3>
Creating command line "rc.exe /l 0x409 /fo"emulatorRel/treedir.res" /d UNDER_CE=400 /d _WIN32_WCE=400 /d "UNICODE" /d "_UNICODE" /d "NDEBUG" /d "WCE_PLATFORM_STANDARDSDK" /d "_X86_" /d "x86" /d "_i386_" /r "C:\xp-backup\eVC\CENET42\Dirtree\treedir.rc"" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP1871.tmp" with contents
[
/nologo /W3 /D _WIN32_WCE=400 /D "WCE_PLATFORM_STANDARDSDK" /D "_i386_" /D UNDER_CE=400 /D "i_386_" /D "UNICODE" /D "_UNICODE" /D "_X86_" /D "x86" /D "NDEBUG" /Fp"emulatorRel/Dirtree.pch" /YX /Fo"emulatorRel/" /Gs8192 /GF /O2 /c 
"C:\xp-backup\eVC\CENET42\Dirtree\dir.cpp"
"C:\xp-backup\eVC\CENET42\Dirtree\initproc.cpp"
"C:\xp-backup\eVC\CENET42\Dirtree\main.cpp"
"C:\xp-backup\eVC\CENET42\Dirtree\treeview.cpp"
"C:\xp-backup\eVC\CENET42\Dirtree\wndproc.cpp"
]
Creating command line "cl.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP1871.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP1872.tmp" with contents
[
corelibc.lib commctrl.lib coredll.lib aygshell.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"WinMainCRTStartup" /incremental:no /pdb:"emulatorRel/Dirtree.pdb" /nodefaultlib:"OLDNAMES.lib" /nodefaultlib:libc.lib /nodefaultlib:libcd.lib /nodefaultlib:libcmt.lib /nodefaultlib:libcmtd.lib /nodefaultlib:msvcrt.lib /nodefaultlib:msvcrtd.lib /out:"emulatorRel/Dirtree.exe" /subsystem:windowsce,4.00 /MACHINE:IX86 
".\emulatorRel\dir.obj"
".\emulatorRel\initproc.obj"
".\emulatorRel\main.obj"
".\emulatorRel\treeview.obj"
".\emulatorRel\wndproc.obj"
".\emulatorRel\treedir.res"
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP1872.tmp"
<h3>Output Window</h3>
Compiling resources...
C:\xp-backup\eVC\CENET42\Dirtree\newres.h(32) : fatal error RC1015: cannot open include file 'winver.h'.
Error executing rc.exe.



<h3>Results</h3>
Dirtree.exe - 1 error(s), 0 warning(s)
</pre>
</body>
</html>
