@echo off

@set IDL_ROOT=%~dp0..
@set IDL_TMP=%IDL_ROOT%\tmp

call "%VS90COMNTOOLS%\..\..\VC\vcvarsall.bat" x86

path=%UNM_ROOT%\..\Build\srctools\win32_vc9_x86\cmake;%path%

title ����IDL��VC2008����
echo ��ʼ����IDL����...
set MAKE_START_TIME=%time%
cd /d "%IDL_ROOT%"
cmake -G "Visual Studio 9 2008" -H. -B%IDL_TMP%
set MAKE_STOP_TIME=%time%
echo %MAKE_START_TIME% ��ʼCMAKE��%MAKE_STOP_TIME% ���CMAKE
echo �����������IDL����
pause

set BUILD_EXE=BuildConsole.exe
set BUILD_TYPE=/build
set BUILDCFG=Debug

title %title% IDL
echo ��ʼ����IDL...
set BUILD_START_TIME=%time%
%BUILD_EXE% %IDL_TMP%\IDL.sln %BUILD_TYPE% %BUILDCFG%^|Win32 
echo %MAKE_START_TIME% ��ʼMAKE��%MAKE_STOP_TIME% ���MAKE
echo %BUILD_START_TIME% ��ʼ���룬%time% ��ɱ���
pause