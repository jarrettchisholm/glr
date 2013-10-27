import subprocess, sys, os
import platform
import multiprocessing

### Establish our system
isLinux = platform.system() == 'Linux'
isWindows = os.name == 'nt'
isMac = platform.system() == 'Darwin'

### Compiler
compiler = 'default'

debug = True # True by default (at least for now)

cpp_paths = []
cpp_defines = []
cpp_flags = []
link_flags = []
library_paths = []
libraries = []



def setup(ARGUMENTS):	
	### Error check our platform type
	if (not isLinux and not isWindows and not isMac):
		print("Sorry, but it appears your platform is not recognized")
		sys.exit(1)
	
	### Set our compiler
	compiler = ARGUMENTS.get('compiler')
	
	if (compiler is None or compiler == ''):
		compiler = 'default'
	if (compiler == 'gcc' and isWindows):
		compiler = 'mingw'
	if (compiler == 'msvc' and isWindows):
		compiler = 'default'
	
	### Error check compiler
	if (compiler == 'msvc' and isWindows):
		print( "Cannot use msvc in this environment!" )
		sys.exit(1)
	
	if (debug):
		cpp_defines.append('DEBUG')
	
	### Set our OS specific compiler variables
	if (not isWindows):
		if (compiler == 'gcc' or (compiler == 'default' and isLinux)):
			if (debug):
				cpp_flags.append('-g')
			cpp_flags.append('-O0') # optimization level 0
			#cpp_flags.append('-pg') # profiler
			cpp_flags.append('-std=c++11')
			cpp_flags.append('-pedantic-errors')
			#cpp_flags.append('-Wall')
			#cpp_flags.append('-Wextra')
			#cpp_flags.append('-Werror')
			
		# Dynamically link to boost log
		cpp_defines.append('BOOST_LOG_DYN_LINK')
		
		# For some reason, on windows we need to use boost::phoenix version 3 with boost::log
		cpp_defines.append('BOOST_SPIRIT_USE_PHOENIX_V3')
		
		# TODO: Properly include glr header files
		cpp_paths.append('../../src')
		cpp_paths.append('../src')
		
		# Need to install cef3 to '/usr/local/include/cef3' for this to work
		cpp_paths.append('/usr/local/include/cef3')
	else:
		if isWindows:
			if (compiler == 'default'):
				cpp_flags.append('/w') # disables warnings (Windows)
				cpp_flags.append('/wd4350') # disables the specific warning C4350
				cpp_flags.append('/EHsc') # Enable 'unwind semantics' for exception handling (Windows)
				cpp_flags.append('/MD')
				
				if (debug):
					link_flags.append('/DEBUG') # Enable debug during linking
			elif (compiler == 'mingw'):
				if (debug):
					cpp_flags.append('-g')
				cpp_flags.append('-O0') # optimization level 0
				#cpp_flags.append('-pg') # profiler
				cpp_flags.append('-std=c++11')
				cpp_flags.append('-pedantic-errors')
			
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

