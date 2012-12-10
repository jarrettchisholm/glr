# IceEngine SConstruct file

import os

# Tell SCons to create our build files in the 'build' directory
VariantDir('build', 'src', duplicate=0)

# Set our source files
source_files = Glob('build/*.cpp', 'build/*.h')

source_files = source_files + Glob('build/common/compatibility/*.cpp', 'build/common/compatibility/*.h')
source_files = source_files + Glob('build/common/math/*.cpp', 'build/common/math/*.h')
source_files = source_files + Glob('build/common/utilities/*.cpp', 'build/common/utilities/*.h')
source_files = source_files + Glob('build/vmath/*.cpp', 'build/vmath/*.h')

source_files = source_files + Glob('build/engine/*.cpp', 'build/engine/*.h')
source_files = source_files + Glob('build/engine/gui/*.cpp', 'build/engine/gui/*.h')
source_files = source_files + Glob('build/linux/*.cpp', 'build/linux/*.h')
source_files = source_files + Glob('build/osx/*.cpp', 'build/osx/*.h')
source_files = source_files + Glob('build/windows/*.cpp', 'build/windows/*.h')

# Set our required libraries
libraries = [
'lwis', 
'GL',
'GLU',
'Xi',
'assimp',
'boost_log', 
'boost_date_time', 
'boost_thread'
]
library_paths = ['../lwis/build']

#env = Environment(ENV = {'PATH' : os.environ['PATH'], 'INCLUDE' : os.environ['INCLUDE']}, CCFLAGS=[])
env = Environment(ENV = os.environ, CCFLAGS=[]) 

# Set our g++ compiler flags
cpp_flags = [
'-std=gnu++0x',
'-I"../lwis/src/engine"',
#'-D_GLIBCXX_DEBUG'
]

if (os.name == "nt" or os.name == "win32"):
	cpp_flags.append( '-I"C:\lib\Assimp\include"' )
	#cpp_flags.append( '-I"C:\Program Files (x86)\Microsoft Visual Studio 10.0\VC\include"' )

env.Append( CPPFLAGS = cpp_flags )

# Tell SCons the program to build
#env.Program('build/iceengine', source_files, LIBS = libraries, LIBPATH = library_paths)
env.StaticLibrary('build/iceengine', source_files, LIBS = libraries, LIBPATH = library_paths)
