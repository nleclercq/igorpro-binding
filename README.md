[![logo](http://www.tango-controls.org/static/tango/img/logo_tangocontrols.png)](http://www.tango-controls.org)

# About the Igor Pro binding for Tango

This is the [Igor Pro](http://wavemetrics.com) client API for [Tango](http://tango-controls.org). 

# Documentation

See  `/tutorial/tutorial.ipf`

# How to build on Windows using Microsoft Visual C++ 
- software requirements 
  * Microsoft Visual C++ >= 2013 (a.k.a msvc12) 
  * Igor Pro x64 >= 7.0
  * WaveMetrics' XOP Toolkit for Igor Pro >= 7.0
  
- clone the github repository in a directory of your choice (here we use c:\github)
```
cd c:\github 
git clone git@github.com:tango-controls/igorpro-binding.git
```

- edit the `compil-env-setup.bat` file
```
{edit} c:\github\igorpro-binding\windows\msvcxx\compil-env-setup.bat
```

- set BINDING_PATH to the igorpro-binding clone location  
```
set BINDING_PATH=c:\github\igorpro-binding 
```

- set XOP Toolkit paths according to your local Igor Pro installation
```
set XOP_TOOLKIT_ROOT=C:\xop-toolkit-7
set XOP_INC=%XOP_TOOLKIT_ROOT%\include
set XOP_RC=%XOP_TOOLKIT_ROOT%\resource
set XOP_LIB=%XOP_TOOLKIT_ROOT%\lib
```

- set the XOP Toolkit libraries names according to your local configuration
```
set XOP_LIB_LIST=igor64.lib;XOPSupport64.lib
set XOP_LIB_DEBUG_LIST=igor64.lib;XOPSupport64.lib
```

- save the `compil-env-setup.bat` file then open the Visual C++ project
```
{double-click-on} c:\github\igorpro-binding\windows\msvcxx\run-me-to-compile.bat
```

- select the configuration you want to compile then build the solution
```
{select one of the following combination} [release or debug] / [x64]
{build}
```

- the compilation will hopefully succeed
```
{safely ignore any warning for the linker}
{check for presence of} c:\github\igorpro-binding\src\tango_binding_x64.xop
```

- you can optionally edit the provided Igor Pro launcher and modify the PATH env. variable
```
{install xop and procedures} see c:\github\install\install.txt

{edit} c:\github\igorpro-binding\launcher\windows\launch-igorpro.bat

set PATH=c:\github\igorpro-binding\runtime\windows\lib\x64\tango-9.2.2-vc12;%PATH%

{save then double-click on} c:\github\igorpro-binding\launcher\windows\launch-igorpro.bat
```

- in case you want to use your own Tango runtime, just edit the `compil-env-setup.bat`, modify the paths then rebuild the solution


# Links

[Tango Controls home page](http://tango-controls.org)

[WaveMetrics home page](http://wavemetrics.com)

[WaveMetrics XOP Toolkit home page](http://wavemetrics.com/products/xoptoolkit/xoptoolkit.htm)
