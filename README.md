Graphics Language Renderer
==========================

[![Build Status](https://travis-ci.org/jarrettchisholm/glr.png)](https://travis-ci.org/jarrettchisholm/glr)

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
-------
You will need several pre-requisites installed before compilation will work.  For an automated setup, check out
the [Automatic Building](https://github.com/jarrettchisholm/glr/wiki/Automatic-Building) section of the wiki.

**Windows**

These instructions assume you are using the Git Bash terminal. You can, however, use a more visual tool for cloning if you wish.

Furthermore, for compilation, you will need to use the Developer Command Prompt for VS2013.

Assuming you have all the pre-requisites ready to go, you can run the automatic setup by running

    python setup.py release

from the terminal (or the developer Command Prompt for VS2013 on Windows).

**Note**: debug setup is not yet implemented.  Please see issue [#30](https://github.com/jarrettchisholm/glr/issues/30).

Once setup is complete, you can compile by running

    scons

from the terminal (or the Developer Command Prompt for VS2013 on Windows).

For more details about compiling, check out the [wiki](https://github.com/jarrettchisholm/glr/wiki).
