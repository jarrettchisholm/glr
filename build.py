#! /bin/python

import subprocess, sys, os
import shlex
import shutil
import argparse

def beautifyCode():
	fileList = []
	
	for r,d,f in os.walk("."):
		for files in f:
			if files.endswith(".h") or files.endswith(".cpp"):
				fileList.append( os.path.join(r,files) )
	
	
	for f in fileList:
		subprocess.call( shlex.split("uncrustify --mtime --no-backup -q -c xsupplicant.cfg "+f) )


def compileGlr(compiler):
	os.chdir( '../glr' )
	return subprocess.Popen( 'scons compiler='+compiler, shell=True )

def clear():
	if (os.name == 'nt'):
		os.system('cls')
	else:
		os.system('clear')

def clean():
	os.chdir( '../glr' )
	shutil.rmtree( './build', True )

def exitOnError(returnCode):
	if ( returnCode != 0):
		print( returnCode )
		print( "Script halted due to error(s)!" )
		sys.exit(1)



clear()
if (os.name != 'nt'):
	os.system( 'echo' )
	os.system( 'echo' )
	os.system( 'echo' )


# Argument flags
doBeautification = False
doClean = False
compiler = ""


# Handle arguments
parser = argparse.ArgumentParser()
parser.add_argument("--beautify", action="store_true", help="will 'beautify' the source code using uncrustify")
parser.add_argument("--clean", action="store_true", help="will clean your build directory of all files")
parser.add_argument("--gcc", action="store_true", help="will use gcc if available")
parser.add_argument("--msvc", action="store_true", help="will use Microsofts compiler if available")
parser.add_argument("--clang", action="store_true", help="will use clang if available")
args = parser.parse_args()
if args.beautify:
	doBeautification = True
if args.clean:
	doClean = True

if args.gcc:
	if compiler == "":
		if os.name == 'nt':
			compiler = 'mingw'
		else:
			compiler = 'gcc'
	else:
		print( "Cannot use more than one compiler!" )
		sys.exit(1)

if args.msvc:
	if compiler == "":
		if os.name == 'nt':
			compiler = 'msvc'
		else:
			print( "Cannot use msvc in this environment!" )
			sys.exit(1)
	else:
		print( "Cannot use more than one compiler!" )
		sys.exit(1)
	
if args.clang:
	if compiler == "":
		compiler = 'clang'
	else:
		print( "Cannot use more than one compiler!" )
		sys.exit(1)
	


if (doBeautification):
	print("Beautifying Code")
	beautifyCode()
	print("Done")
	print("")


if (doClean):
	print("Cleaning Glr build directory")
	clean();
	print("Done")

print("Compiling Glr")
exitOnError( compileGlr(compiler).wait() )
print("Done")
