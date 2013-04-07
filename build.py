#! /bin/python

import subprocess, sys, os
import shlex
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
	os.chdir( '../glr' )
	return subprocess.call( "scons" )

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


# Handle arguments
parser = argparse.ArgumentParser()
parser.add_argument("--beautify", help="will 'beautify' the source code using uncrustify")
args = parser.parse_args()
if args.beautify:
	doBeautification = True
    


if (doBeautification):
	print("Beautifying Code")
	beautifyCode()
	print("Done")
	print("")


print("Compiling Glr")
exitOnError( compileGlr() )
print("Done")
