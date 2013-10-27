import subprocess, sys, os

subprocess.call( 'scons', shell=True)

print("Running Tests")

os.chdir( 'build/' )
subprocess.call( "./glr_tests" )
