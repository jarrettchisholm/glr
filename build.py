#! /bin/python

import subprocess, sys, os



os.system( 'clear' )
os.system( 'echo' )
os.system( 'echo' )
os.system( 'echo' )

returnCode = subprocess.call( "scons" )

if ( returnCode != 0):
	print( "Script halted due to error(s)!" )
	sys.exit(1)

