@echo off

REM == this is must to open cmd.exe with VS environment !!! ==
REM == %comspec% /k ""C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"" x86 ==
REM == C:\WINDOWS\system32\cmd.exe /k ""C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"" x86 ==

REM == consider to use tool below if you have multi-core in your machine ==
REM == ftp://ftp.qt.nokia.com/jom/jom.zip ==

REM == for CE6.0 platform sdk, use tool gen ~ ==
REM == buildsdk [MyOSDesign.pbxml] [MySDK.sdkcfg] ==

set VS_ENV_SETUP="C:\Program Files\Microsoft Visual Studio 8\VC\vcvarsall.bat"
set QT_BASE_PATH=D:\workshop\qt
set QT_VER_DIR=qt-everywhere-opensource-src-4.7.0
REM set QT_VER_DIR=qt-everywhere-opensource-src-4.8.0

REM == if you go with platform builder ==
REM == check path below ==
REM == C:\WINCE600\PUBLIC\COMMON\SDK\INC ==
REM == C:\WINCE600\PUBLIC\COMMON\SDK\LIB\ARMV4I\RETAIL ==

set QT_SDK_INC_PATH="C:\Program Files\Windows CE Tools\wce500\STANDARDSDK_500\Include\Armv4i"
set QT_SDK_LIB_PATH="C:\Program Files\Windows CE Tools\wce500\STANDARDSDK_500\Lib\ARMV4I"


REM == add binary path ==
set PATH=%QT_BASE_PATH%\%QT_VER_DIR%\bin;%PATH%

REM == add nmake path ==
REM set PATH=C:\Program Files\Microsoft Visual Studio 8\VC\bin;%PATH%
REM set PATH=C:\Program Files\Microsoft Visual Studio 8\SDK\v2.0\Bin;%PATH%
call %VS_ENV_SETUP%


@echo "cd %QT_BASE_PATH%\%QT_VER_DIR% ..."
cd %QT_BASE_PATH%\%QT_VER_DIR%


REM == get configure help ==
REM == from http://doc.qt.nokia.com/latest/configure-options.html ==
REM == and README file under Qt folder ==

REM configure.exe -platform win32-msvc2005 -xplatform wincewm50pocket-msvc2005
REM configure.exe -platform win32-msvc2005 -xplatform wince50standard-armv4i-msvc2005
REM configure.exe -opensource -platform win32-msvc2005 -xplatform wince50standard-armv4i-msvc2005
REM configure.exe -opensource -platform win32-msvc2005 -xplatform wince50standard-armv4i-msvc2005 -webkit -qt-zlib -qt-libpng -qt-libmng -qt-libtiff -qt-libjpeg -no-openssl -no-dbus -phonon -plugin-sql-sqlite
configure.exe -opensource -platform win32-msvc2005 -xplatform wince50standard-armv4i-msvc2005

REM Qt is now configured for building. To start the build 
REM run:setcepaths.bat wince50standard-armv4i-msvc2005 && nmake.
REM To reconfigure, run 'nmake confclean' and configure.
REM Environment Selection:STANDARDSDK_500 (ARMV4I)
REM Standard SDK for arm selected, environment is set up
REM pause
REM exit

REM == not use the script from Qt, setcepaths (setcepaths wincewm50pocket-msvc2005) ==
REM == we setup the envirnment ourself !! ==
REM set INCLUDE=C:\Program Files\Microsoft Visual Studio 8\VC\ce\include;%QT_SDK_INC_PATH%
REM set LIB=C:\Program Files\Microsoft Visual Studio 8\VC\ce\lib\armv4i;%QT_SDK_LIB_PATH%
REM set PATH=C:\Program Files\Microsoft Visual Studio 8\VC\ce\bin\x86_arm;%PATH%
setcepaths wince50standard-armv4i-msvc2005



@echo "INCLUDE=%INCLUDE%"
@echo "LIB=%LIB%"
@echo "PATH=%PATH%"

@echo "nmake ..."
nmake


@echo "cd %QT_BASE_PATH% ..."
cd %QT_BASE_PATH%
