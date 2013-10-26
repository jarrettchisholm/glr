Graphics Language Renderer
==========================

Overview
--------
A graphics rendering engine written in C++ (using C++11) and OpenGL (targetting 3.2 and above).

Goals
-----
The goals for this project is to create a graphics rendering engine library that is:
- Fast
- Easy to use
- Robust

Furthermore, the goals for the code are for it to be:
- Documented
- Clean

Compile
=======
To compile, run 'scons' from the command line.

**Note**: You must have a 'lib' directory with the files:

**Linux**

-  libcef.so
-  libffmpegsumo.so
-  libcef_dll_wrapper.lib
-  libcef_dll_wrapper

**Windows**

-  libcef.dll
-  libcef.lib
-  libffmpegsumo.dll
-  libcef_dll_wrapper.lib

The libcef_dll_wrapper is the directory that contains all of the cef_dll object files (within their folder heirarchy).

For more details about compiling, check out the [[wiki|https://github.com/jarrettchisholm/glr/wiki]].
