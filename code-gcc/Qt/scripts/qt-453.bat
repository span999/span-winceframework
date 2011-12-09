@echo off

REM == this is must to open cmd.exe with VS environment !!! ==
REM == %comspec% /k ""C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"" x86 ==
REM == C:\WINDOWS\system32\cmd.exe /k ""C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"" x86 ==
REM ==  ==



set QT_BASE_PATH=D:\workshop\qt
set QT_VER_DIR=qt-embedded-wince-opensource-src-4.5.3

REM == add binary path ==
set PATH=%QT_BASE_PATH%\%QT_VER_DIR%\bin;%PATH%

REM == add nmake path ==
set PATH=C:\Program Files\Microsoft Visual Studio 8\VC\bin;%PATH%
REM set PATH=C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\Bin;%PATH%


@echo "cd %QT_BASE_PATH%\%QT_VER_DIR% ..."
cd %QT_BASE_PATH%\%QT_VER_DIR%


REM == get configure help ==
REM == from http://doc.qt.nokia.com/latest/configure-options.html ==
REM == and README file under Qt folder ==

REM configure.exe -platform win32-msvc2005 -xplatform wincewm50pocket-msvc2005
REM configure.exe -platform win32-msvc2005 -xplatform wince50standard-armv4i-msvc2005
REM configure.exe -opensource -platform win32-msvc2005 -xplatform wince50standard-armv4i-msvc2005
configure.exe -opensource -platform win32-msvc2005 -xplatform wince50standard-armv4i-msvc2005 -webkit -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg -no-openssl -no-dbus -phonon -plugin-sql-sqlite

pause


REM == not use the script from Qt, setcepaths (setcepaths wincewm50pocket-msvc2005) ==
REM == we setup the envirnment ourself !! ==
set INCLUDE=C:\Program Files\Microsoft Visual Studio 8\VC\ce\include;C:\Program Files\Windows CE Tools\wce500\STANDARDSDK_500\Include\Armv4i
set LIB=C:\Program Files\Microsoft Visual Studio 8\VC\ce\lib\armv4i;C:\Program Files\Windows CE Tools\wce500\STANDARDSDK_500\Lib\ARMV4I
set PATH=C:\Program Files\Microsoft Visual Studio 8\VC\ce\bin\x86_arm;%PATH%
REM set PATH=C:\Program Files\Microsoft Visual Studio 8\VC\bin;%PATH%


@echo "PATH=%PATH%"

@echo "nmake ..."
nmake


@echo "cd %QT_BASE_PATH% ..."
cd %QT_BASE_PATH%
