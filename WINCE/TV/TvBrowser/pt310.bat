cd ARMV4IRel\
del TvBrowser_t.exe
ren TvBrowser.exe TvBrowser_t.exe
del C:\WINCE500\PLATFORM\PT310\Files\TvBrowser_t.exe
copy TvBrowser_t.exe C:\WINCE500\PLATFORM\PT310\Files\

del C:\WINCE500\PBWorkspaces\PT310\RelDir\PT310_ARMV4I_Release\TvBrowser_t.exe
copy TvBrowser_t.exe C:\WINCE500\PBWorkspaces\PT310\RelDir\PT310_ARMV4I_Release\
cd ..
pause