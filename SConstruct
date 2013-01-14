# IceEngine SConstruct file

import os

# Tell SCons to create our build files in the 'build' directory
VariantDir('build', 'src', duplicate=0)

# Set our source files
source_files = Glob('build/*.cpp', 'build/*.h')

source_files = source_files + Glob('build/common/compatibility/*.cpp', 'build/common/compatibility/*.h')
source_files = source_files + Glob('build/common/math/*.cpp', 'build/common/math/*.h')
source_files = source_files + Glob('build/common/utilities/*.cpp', 'build/common/utilities/*.h')
source_files = source_files + Glob('build/common/io/*.cpp', 'build/common/io/*.h')
#source_files = source_files + Glob('build/vmath/*.cpp', 'build/vmath/*.h')

source_files = source_files + Glob('build/engine/*.cpp', 'build/engine/*.h')
source_files = source_files + Glob('build/engine/gui/*.cpp', 'build/engine/gui/*.h')
source_files = source_files + Glob('build/engine/models/*.cpp', 'build/engine/models/*.h')
source_files = source_files + Glob('build/engine/shaders/*.cpp', 'build/engine/shaders/*.h')

env = Environment(ENV = os.environ, CCFLAGS=[]) 


# Set our required libraries
libraries = [
'GL',
'GLU',
'GLEW',
'Xi',
'sfml-window',
'assimp',
'boost_log', 
'boost_date_time', 
'boost_thread'
]

library_paths = ['../lwis/build']

# Set our g++ compiler flags
cpp_flags = [
'-std=gnu++0x',
'-Wall',
#'-D_GLIBCXX_DEBUG'
]

cpp_defines = []

if (os.name == "nt" or os.name == "win32"):
	cpp_flags.append( '-I"C:\lib\Assimp\include"' )


debug = ARGUMENTS.get('debug', 0)
if int(debug):
	cpp_flags.append('-g')
	cpp_defines.append('DEBUG')
else:
	cpp_defines.append('NDEBUG')
	
	
	
#for key, value in ARGLIST:
#	if key == 'define':
#		cppdefines.append(value)
       

env.Append( CPPFLAGS = cpp_flags )
env.Append( CPPDEFINES = cpp_defines )

# Tell SCons the program to build
#env.Program('build/iceengine', source_files, LIBS = libraries, LIBPATH = library_paths)
env.StaticLibrary('build/iceengine', source_files, LIBS = libraries, LIBPATH = library_paths)
