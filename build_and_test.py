#! /bin/python

import subprocess, sys, os
import shlex

args = ''
for arg in sys.argv:
	if (arg != 'build_and_test.py' and arg != 'build_and_run.py'):
		args += ' ' + arg

subprocess.call( 'scons ' + args, shell=True)

os.chdir( 'tests/' )
subprocess.call( 'scons ' + args, shell=True )

# Don't run the tests yet (Can't on Travis CI)
#os.chdir( 'build/' )
#subprocess.call( "./glr_tests" )
