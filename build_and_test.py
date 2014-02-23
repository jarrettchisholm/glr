#! /bin/python

import subprocess, sys, os
import shlex

args = ''
for arg in sys.argv:
	if (arg != 'build_and_test.py' and arg != 'build_and_run.py'):
		args += ' ' + arg

exitCode = subprocess.call( 'scons ' + args, shell=True)

if (exitCode is not 0):
	sys.exit(1)

os.chdir( 'tests/' )
exitCode = subprocess.call( 'scons ' + args, shell=True)

if (exitCode is not 0):
	sys.exit(1)

# Don't run the tests yet (Can't on Travis CI)
#os.chdir( 'build/' )
#exitCode = subprocess.call( "./glr_tests" )
#
#if (exitCode is not 0):
#	sys.exit(1)
