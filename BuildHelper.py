import subprocess, sys, os
import platform
import multiprocessing
import shlex

from SCons.Script import *

### Establish our system
isLinux = platform.system() == 'Linux'
isWindows = os.name == 'nt'
isMac = platform.system() == 'Darwin'

buildFlags = {}
buildFlags['debug'] = True # True by default (at least for now)
buildFlags['useCef'] = True
buildFlags['beautify'] = False
buildFlags['clean'] = False
buildFlags['build'] = 'debug'
buildFlags['compiler'] = 'default'
buildFlags['num_jobs'] = multiprocessing.cpu_count()

dependenciesDirectory = 'deps/'

cpp_paths = []
cpp_defines = []
cpp_flags = []
link_flags = []
library_paths = []
libraries = []



def setup(ARGUMENTS):	
	### Error check our platform type
	if (not isLinux and not isWindows and not isMac):
		print("Sorry, but it appears your platform is not recognized.")
		sys.exit(1)
	
	AddOption('--beautify', dest='beautify', action='store_true', help='Will \'beautify\' the source code using uncrustify.')
	AddOption('--without-cef', dest='without-cef', action='store_true', help='Will compile glr without using Chromium Embedded Framework as the html gui system.')
	AddOption('--build', dest='build', type='string', nargs=1, action='store', help='Set the build to compile:  release, debug (default)')
	AddOption('--compiler', dest='compiler', type='string', nargs=1, action='store', help='Set the compiler to use.')
	
	global buildFlags
	
	### Set and error check our build flags
	if (GetOption('without-cef') is True):
		buildFlags['useCef'] = False
	if (GetOption('beautify') is True):
		buildFlags['beautify'] = True
	if (GetOption('clean') is True):
		buildFlags['clean'] = True
	if (GetOption('build') is not None):
		buildFlags['build'] = GetOption('build')
		if (buildFlags['build'] != 'release' and buildFlags['build'] != 'debug'):
			print( "Invalid build: '{0}'".format(buildFlags['build']) )
			sys.exit(1)
	if (GetOption('compiler') is not None):
		buildFlags['compiler'] = GetOption('compiler')
	# TODO: num_jobs seems to default to 1 - how can I know if the user is actually setting this value?
	if (GetOption('num_jobs') > 1):
		buildFlags['num_jobs'] = GetOption('num_jobs')
	
	# TODO: num_jobs seems to default to 1 - how can I know if the user is actually setting this value?
	if (GetOption('num_jobs') > 1):
		buildFlags['num_jobs'] = GetOption('num_jobs')
	
	# Override - useful for limiting the number of cpus used for CI builds (i.e. travis ci)
	if (os.environ.get('SCONS_NUM_JOBS')):
		buildFlags['num_jobs'] = os.environ.get('SCONS_NUM_JOBS')
	
	if (buildFlags['useCef']):
		cpp_defines.append('USE_CEF')
	if (buildFlags['build'] == 'debug'):
		cpp_defines.append('DEBUG')
	
	if (buildFlags['compiler'] is None or buildFlags['compiler'] == ''):
		buildFlags['compiler'] = 'default'
	if (buildFlags['compiler'] == 'gcc' and isWindows):
		buildFlags['compiler'] = 'mingw'
	if (buildFlags['compiler'] == 'msvc' and isWindows):
		buildFlags['compiler'] = 'default'
	
	### Error check compiler
	if (buildFlags['compiler'] == 'msvc' and not isWindows):
		print( "Cannot use msvc in this environment!" )
		sys.exit(1)
	
	# TODO: Properly include header files
		
	# For cef_client compilation
	cpp_paths.append('../' + dependenciesDirectory + 'boost/include')
	
	if (buildFlags['useCef']):
		cpp_paths.append('../' + dependenciesDirectory + 'cef3/')
		cpp_paths.append('../' + dependenciesDirectory + 'cef3/include')
	
	includeDirectories = []
	includeDirectories.append('freeimage/include')
	includeDirectories.append('boost/include')
	includeDirectories.append('assimp/include')
	includeDirectories.append('glm/include')
	includeDirectories.append('glew/include')
	includeDirectories.append('sfml/include')
	
	# For glr compilation
	cpp_paths.append('include')
	cpp_paths.append('src') # Need 'src' for submodules
	relativeDirectory = ''
	for d in includeDirectories:
		cpp_paths.append(relativeDirectory + dependenciesDirectory + d)

	if (buildFlags['useCef']):
		cpp_paths.append(dependenciesDirectory + 'cef3/')
		cpp_paths.append(dependenciesDirectory + 'cef3/include')
	
	# For glr tests compilation
	relativeDirectory = '../'
	cpp_paths.append(relativeDirectory + 'include')
	cpp_paths.append(relativeDirectory + 'src') # Need 'src' for submodules
	for d in includeDirectories:
		cpp_paths.append(relativeDirectory + dependenciesDirectory + d)
	
	# For glr samples compilation
	relativeDirectory = '../../'
	cpp_paths.append(relativeDirectory + 'include')
	cpp_paths.append(relativeDirectory + 'src') # Need 'src' for submodules
	for d in includeDirectories:
		cpp_paths.append(relativeDirectory + dependenciesDirectory + d)
	
	### Set our OS specific compiler variables
	if (not isWindows):
		if (buildFlags['compiler'] == 'gcc' or (buildFlags['compiler'] == 'default' and isLinux)):
			if (buildFlags['debug']):
				cpp_flags.append('-g')
				cpp_flags.append('-O0') # optimization level 0
				cpp_flags.append('-pg') # profiler
			else:
				cpp_flags.append('-O3') # optimization level 3
			
			cpp_flags.append('-std=c++11')
			cpp_flags.append('-pedantic-errors')
			#cpp_flags.append('-Wall')
			#cpp_flags.append('-Werror')
			
		# Dynamically link to boost log
		cpp_defines.append('BOOST_LOG_DYN_LINK')
		
		# For some reason, on windows we need to use boost::phoenix version 3 with boost::log
		cpp_defines.append('BOOST_SPIRIT_USE_PHOENIX_V3')
	else:
		if isWindows:
			if (buildFlags['compiler'] == 'default'):
				cpp_flags.append('/w') # disables warnings (Windows)
				cpp_flags.append('/wd4350') # disables the specific warning C4350
				cpp_flags.append('/EHsc') # Enable 'unwind semantics' for exception handling (Windows)
				cpp_flags.append('/MD')
				
				if (buildFlags['debug']):
					cpp_flags.append('/Zi') # Produces a program database (PDB) that contains type information and symbolic debugging information for use with the debugger.
					cpp_flags.append('/FS') # Allows multiple cl.exe processes to write to the same .pdb file
					link_flags.append('/DEBUG') # Enable debug during linking
					cpp_flags.append('/Od') # Disables optimization
				else:
					cpp_flags.append('/Ox') # Full optimization
			elif (buildFlags['compiler'] == 'mingw'):
				if (buildFlags['debug']):
					cpp_flags.append('-g')
					cpp_flags.append('-O0') # optimization level 0
					cpp_flags.append('-pg') # profiler
				else:
					cpp_flags.append('-O3') # optimization level 3
				
				cpp_flags.append('-std=c++11')
				cpp_flags.append('-pedantic-errors')
				#cpp_flags.append('-Wall')
				#cpp_flags.append('-Werror')
			
			# For some reason, on windows we need to use boost::phoenix version 3 with boost::log
			cpp_defines.append('BOOST_SPIRIT_USE_PHOENIX_V3')

def beautifyCode():
	fileList = []
	
	for r,d,f in os.walk("."):
		for files in f:
			if files.endswith(".h") or files.endswith(".cpp"):
				fileList.append( os.path.join(r,files) )
	
	for f in fileList:
		subprocess.call( shlex.split("uncrustify --mtime --no-backup -q -c xsupplicant.cfg "+f) )
		
def clear():
	if (isWindows):
		os.system('cls')
	else:
		os.system('clear')

def exitOnError(returnCode):
	if ( returnCode != 0):
		print( "Script halted due to error(s)!" )
		sys.exit(1)

