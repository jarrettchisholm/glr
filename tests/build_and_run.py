import subprocess, sys, os
import shlex

args = ''
for arg in sys.argv:
	if (arg != 'tests/build_and_run.py' and arg != 'build_and_run.py'):
		args += ' ' + arg

subprocess.call( 'scons ' + args, shell=True)

print("Running Tests")

os.chdir( 'build/' )
subprocess.call( "./glr_tests" )

