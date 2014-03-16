#! /bin/python

import urllib2
import tarfile
import sys, os
import platform
import glob
import subprocess
import shutil

### Establish our system
isLinux = platform.system() == 'Linux'
isWindows = os.name == 'nt'
isMac = platform.system() == 'Darwin'

# Handling libudev.so.0 missing (happens in newer linux distros)
# Easy way:
# 	64 bit:
# 		sudo ln -sf /lib/x86_64-linux-gnu/libudev.so.1 /lib/x86_64-linux-gnu/libudev.so.0
# 	32 bit:
# 		sudo ln -sf /lib/i386-linux-gnu/libudev.so.1 /lib/i386-linux-gnu/libudev.so.0
#
# Hard way:
# 	Unfortunately, I couldn't figure out where to put the libudev.so.0 sym link locally.

sharedLibraryExt = 'so'
staticLibraryExt = 'a'

boostName = 'boost_1.55_x64.tar.gz'
freeImageName = 'freeimage_3.15.1_linux_x64.tar.gz'
assImpName = 'assimp_3.0.1270_linux_x64.tar.gz'
cef3Name = 'cef3_3.1650.1562_linux_x64.tar.gz'
glmName = 'glm_0.9.5.2.tar.gz'
glewName = 'glew_1.10.0_linux_x64.tar.gz'
sfmlName = 'sfml_2.1_linux_x64.tar.gz'

if isWindows:
	sharedLibraryExt = 'dll'
	staticLibraryExt = 'lib'
	
	boostName = 'boost_1.55_win32.tar.gz'
	freeImageName = 'freeimage_3.15.1_win32.tar.gz'
	assImpName = 'assimp_3.0.1270_win32.tar.gz'
	cef3Name = 'cef3_3.1650.1562_win32.tar.gz'
	glewName = 'glew_1.10.0_win32.tar.gz'
	sfmlName = 'sfml_2.1_win32.tar.gz'

files = dict()
# Format
# Note: The only reason for the 'Regex for Dir' is because I can't delete from bintray at the moment, so I can't rename the directory in the zipped file and reupload.
#
# [Display Name]			= [URL, 																		Save Filename,		Regex for Dir, 	New Name for Dir]
files['Boost 1.55'] 		= ['http://dl.bintray.com/jarrettchisholm/generic/{0}'.format(boostName), 		'boost.tar.gz', 	'boost', 		'boost']
files['FreeImage 3.15.1'] 	= ['http://dl.bintray.com/jarrettchisholm/generic/{0}'.format(freeImageName), 	'freeimage.tar.gz', 'freeimage', 	'freeimage']
files['AssImp 3.0.1270'] 	= ['http://dl.bintray.com/jarrettchisholm/generic/{0}'.format(assImpName), 		'assimp.tar.gz', 	'assimp', 		'assimp']
files['CEF3 3.1650.1562'] 	= ['http://dl.bintray.com/jarrettchisholm/generic/{0}'.format(cef3Name), 		'cef3.tar.gz', 		'cef', 			'cef3']
files['GLM 0.9.5.2'] 		= ['http://dl.bintray.com/jarrettchisholm/generic/{0}'.format(glmName), 		'glm.tar.gz', 		'glm', 			'glm']
files['GLEW 1.10.0'] 		= ['http://dl.bintray.com/jarrettchisholm/generic/{0}'.format(glewName), 		'glew.tar.gz', 		'glew', 		'glew']
files['SFML 2.1'] 			= ['http://dl.bintray.com/jarrettchisholm/generic/{0}'.format(sfmlName), 		'sfml.tar.gz', 		'sfml', 		'sfml']

dependenciesDirectory = 'deps/'
librariesDirectory = 'lib/'

def copyFile(fromFile, toFile):
	try:
		shutil.copy(fromFile, toFile)
	except:
		#print('Failed to copy file!')
		pass

def copyDirectory(fromDirectory, toDirectory):
	try:
		shutil.copytree(fromDirectory, toDirectory)
	except:
		#print('Failed to copy directory!')
		pass

def checkSystemDependencies():
	"""Check for required system dependencies, and if they are not present, inform the user and quit"""
	
	if isLinux:
		packages = ['build-essential', 'libgtk2.0-dev', 'libgtkglext1-dev']
		
		for p in packages:
			devNull = open(os.devnull, "w")
			retVal = subprocess.call(["dpkg", "-s", p], stdout = devNull, stderr = subprocess.STDOUT)
			devNull.close()
			
			if (retVal != 0):
				print("Required package '{0}' is not installed.  You will need to install it before you can run setup.py.".format(p))
				sys.exit(1)

# TODO: Implement this?
def installSystemDependencies():
	"""Install any system libraries we require"""
	pass

def download():
	"""Download external library files"""
	
	if (not os.path.exists(dependenciesDirectory)):
		os.makedirs(dependenciesDirectory)
	
	for k in files.keys():
		print('Downloading ' + k)
		
		response = urllib2.urlopen(files[k][0])
		
		contentLength = response.headers["Content-Length"]
		
		data = ''
		chunk = 4096
		while 1:
			d = response.read(chunk)
			if not d:
				print("\n")
				break
			data += d
			
			sys.stdout.write( "\rRead {0} / {1} KB".format((len(data)/1000), int(contentLength)/1000) )
			sys.stdout.flush()
		
		with open(dependenciesDirectory + files[k][1], 'w') as f:
			f.write( bytearray(data) )

def extract():
	"""Extract external library files"""
	
	for k in files.keys():
		print('Extracting ' + k)
		
		with tarfile.TarFile.open(dependenciesDirectory + files[k][1], 'r:gz') as f:
			f.extractall(path=dependenciesDirectory)
			folders = [f for f in glob.glob(dependenciesDirectory + files[k][2] + '*') if os.path.isdir(f)]
			for f in folders:
				os.rename(f, dependenciesDirectory+files[k][3])

def build():
	"""Build any dependencies"""
	print('Building the CEF3 wrapper dll')
	
	if isLinux:
		command = 'bash ./build_cef.sh'
		subprocess.call( command, shell=True)
	elif isWindows:
		print('There is no automated CEF3 wrapper dll build script for Windows yet.  You will have to build it manually.')

def install():
	"""Install the downloaded libraries locally"""
	
	if (not os.path.exists(librariesDirectory)):
		os.makedirs(librariesDirectory)
	
	copyDirectory(dependenciesDirectory + 'cef3/out/Release/obj.target/libcef_dll_wrapper', './{0}/libcef_dll_wrapper'.format(librariesDirectory))	
	copyFile(dependenciesDirectory + 'cef3/out/Release/obj.target/libcef_dll_wrapper.a', './{0}/libcef_dll_wrapper.a'.format(librariesDirectory))
	copyFile(dependenciesDirectory + 'cef3/Release/libcef.so', './{0}/libcef.so'.format(librariesDirectory))
	copyFile(dependenciesDirectory + 'cef3/Release/libffmpegsumo.so', './{0}/libffmpegsumo.so'.format(librariesDirectory))



checkSystemDependencies()
installSystemDependencies()
download()
extract()
build()
install()
