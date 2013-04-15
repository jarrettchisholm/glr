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


def compileGlr():
	os.chdir( '../' )
	return subprocess.call( "scons" )

def compileGlrTests():
	os.chdir( 'tests' )
	return subprocess.call( "scons" )

def clean():
	shutil.rmtree( './build', True )

def exitOnError(returnCode):
	if ( returnCode != 0):
		print( "Script halted due to error(s)!" )
		sys.exit(1)



os.system( 'clear' )
os.system( 'echo' )
os.system( 'echo' )
os.system( 'echo' )


# Argument flags
doBeautification = False
doClean = False


# Handle arguments
parser = argparse.ArgumentParser()
parser.add_argument("--beautify", action="store_true", help="will 'beautify' the source code using uncrustify")
parser.add_argument("--clean", action="store_true", help="will clean your build directory of all files")
args = parser.parse_args()
if args.beautify:
	doBeautification = True
if args.clean:
	doClean = True
    


if (doBeautification):
	print("Beautifying Code")
	beautifyCode()
	print("Done")
	print("")


if (doClean):
	print("Cleaning Glr Tests build directory")
	clean();
	print("Done")

print("Compiling Glr")
exitOnError( compileGlr() )
print("Done")

print("Compiling Glr Tests")
exitOnError( compileGlrTests() )
print("Done")
