@echo off

set VER_DATE=%date:~0,4%%date:~5,2%%date:~8,2%
set VER_TIME=%time:~0,2%%time:~3,2%

del Inc\Version_new
echo #define STRFILEVER "1, 0, %VER_DATE%, %VER_TIME%\0" > Inc\Version_new
del Inc\Version_pro
echo #define STRPRODUCTVER  "1, 0, %_TGTPLAT%, %_TGTPROJ%\0" > Inc\Version_pro

copy Inc\spVersion_1 + Inc\Version_new + Inc\Version_pro + Inc\spVersion_2 Inc\spVersion.h