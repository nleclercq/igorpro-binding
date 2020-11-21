@ECHO off

:: ======== TANGO BINDING PATHS
::- git clone directory
set BINDING_PATH=C:\mystuffs\dev\bindings\igorpro-binding@github
::- runtime directory
set RUNTIME_PATH_X86=%BINDING_PATH%\runtime\windows
::- .h
set RUNTIME_INC_X86=%RUNTIME_PATH_X86%\include
::- .lib
set RUNTIME_LIB_X86=%RUNTIME_PATH_X86%\lib\x86\tango-9.2.2-vc10
::- .dll
set RUNTIME_BIN_X86=%RUNTIME_PATH_X86%\lib\x86\tango-9.2.2-vc10

:: ======== PATH
SET PATH=%RUNTIME_LIB_X86%;%PATH%

:: ======== OMNIORB
SET OMNIORB_ROOT=%RUNTIME_PATH_X86%
SET OMNIORB_INC=%RUNTIME_INC_X86%
SET OMNIORB_LIB=%RUNTIME_LIB_X86%
SET OMNIORB_LIB_LIST=omnithread40_rt.lib;omniORB421_rt.lib;COS421_rt.lib;omniDynamic421_rt.lib
SET OMNIORB_LIB_DEBUG_LIST=omnithread40_rtd.lib;omniORB421_rtd.lib;COS421_rtd.lib;omniDynamic421_rtd.lib

:: ======== LOG4TANGO
SET LOG4TANGO_ROOT=%RUNTIME_PATH_X86%
SET LOG4TANGO_INC=%RUNTIME_INC_X86%
SET LOG4TANGO_LIB=%RUNTIME_LIB_X86%
SET LOG4TANGO_LIB_LIST=log4tango.lib
SET LOG4TANGO_LIB_DEBUG_LIST=log4tangod.lib

:: ======== TANGO
SET TANGO_ROOT=%RUNTIME_PATH_X86%
SET TANGO_INC=%RUNTIME_INC_X86%
SET TANGO_LIB=%RUNTIME_LIB_X86%
SET TANGO_LIB_LIST=tango.lib
SET TANGO_LIB_DEBUG_LIST=tangod.lib

:: ======== YAT PATHS
set YAT_ROOT=%RUNTIME_PATH_X86%
set YAT_INC=%RUNTIME_INC_X86%
set YAT_LIB=%RUNTIME_LIB_X86%
set YAT_LIB_LIST=yat.lib
set YAT_LIB_DEBUG_LIST=yatd.lib

:: ======== YAT FOR TANGO PATHS
set YAT_FOR_TANGO_ROOT=%RUNTIME_PATH_X86%
set YAT_FOR_TANGO_INC=%RUNTIME_INC_X86%
set YAT_FOR_TANGO_LIB=%RUNTIME_LIB_X86%
set YAT_FOR_TANGO_LIB_LIST=yat4tango.lib
set YAT_FOR_TANGO_LIB_DEBUG_LIST=yat4tangod.lib

:: ======== IGOR PRO XOP
set XOP_TOOLKIT_ROOT=C:\mystuffs\dev\bindings\igorpro-binding@github\xop-toolkit-7.1
set XOP_INC=%XOP_TOOLKIT_ROOT%\include
set XOP_RC=%XOP_TOOLKIT_ROOT%\resource
set XOP_LIB=%XOP_TOOLKIT_ROOT%\lib
set XOP_LIB_LIST=igor.lib;XOPSupport32.lib
set XOP_LIB_DEBUG_LIST=igor.lib;XOPSupport32.lib

:: ======== MSVC12 (a.k.a 2013)
set IDE="C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\IDE\WDExpress"

%IDE% .\TangoBindingFroIgorPro.sln