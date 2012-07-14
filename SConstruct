# IceEngine SConstruct file

# Tell SCons to create our build files in the 'build' directory
VariantDir('build', 'src', duplicate=0)

# Set our source files
source_files = Glob('build/*.cpp', 'build/*.h')

source_files = source_files + Glob('build/common/compatibility/*.cpp', 'build/common/compatibility/*.h')
source_files = source_files + Glob('build/common/math/*.cpp', 'build/common/math/*.h')
source_files = source_files + Glob('build/common/utilities/*.cpp', 'build/common/utilities/*.h')
source_files = source_files + Glob('build/vmath/*.cpp', 'build/vmath/*.h')

source_files = source_files + Glob('build/engine/*.cpp', 'build/engine/*.h')
source_files = source_files + Glob('build/linux/*.cpp', 'build/linux/*.h')
source_files = source_files + Glob('build/osx/*.cpp', 'build/osx/*.h')
source_files = source_files + Glob('build/windows/*.cpp', 'build/windows/*.h')

# Set our required libraries
libraries = ['lwinputsystem', 'GL', 'GLU', 'Xi', 'boost_log', 'boost_date_time', 'boost_thread']
library_paths = ['../lwinputsystem/build']

env = Environment()

# Set our g++ compiler flags
env.Append( CPPFLAGS=['-I"../lwinputsystem/src/engine"'] )

# Tell SCons the program to build
env.Program('build/iceengine', source_files, LIBS = libraries, LIBPATH = library_paths)
