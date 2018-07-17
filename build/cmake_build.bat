@echo off

@set IDL_ROOT=%~dp0..
@set IDL_TMP=%IDL_ROOT%\tmp

@set BOOST_ROOT=P:\workplace\code\boost_1_67_0
@set BOOST_LIB_VERSION=1_67

call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" x86

path=%UNM_ROOT%\..\Build\srctools\win32_vc9_x86\cmake;%path%

title 生成IDL的VC2008工程
echo 开始生成IDL工程...
set MAKE_START_TIME=%time%
cd /d "%IDL_ROOT%"
cmake -G "Visual Studio 9 2008" -H. -B%IDL_TMP%
set MAKE_STOP_TIME=%time%
echo %MAKE_START_TIME% 开始CMAKE，%MAKE_STOP_TIME% 完成CMAKE
echo 按任意键编译IDL工程
pause

set BUILD_EXE=BuildConsole.exe
set BUILD_TYPE=/build
set BUILDCFG=Debug

title %title% IDL
echo 开始编译IDL...
set BUILD_START_TIME=%time%
%BUILD_EXE% %IDL_TMP%\IDL.sln %BUILD_TYPE% %BUILDCFG%^|Win32 
echo %MAKE_START_TIME% 开始MAKE，%MAKE_STOP_TIME% 完成MAKE
echo %BUILD_START_TIME% 开始编译，%time% 完成编译
pause