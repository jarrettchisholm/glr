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
#downloadGlrDepsUrl = 'http://dl.bintray.com/jarrettchisholm/generic/'
downloadGlrDepsUrl = 'http://icebreakersentertainment.com/downloads/glr/deps/'

boostName = 'boost.tar.gz'
freeImageName = 'freeimage.tar.gz'
assImpName = 'assimp.tar.gz'
cef3Name = 'cef3.tar.gz'
glmName = 'glm.tar.gz'
glewName = 'glew.tar.gz'
sfmlName = 'sfml.tar.gz'

if isLinux:
	downloadGlrDepsUrl += 'linux/x64/'

if isWindows:
	sharedLibraryExt = 'dll'
	staticLibraryExt = 'lib'
	
	downloadGlrDepsUrl += 'windows/x86/'

if isMac:
	downloadGlrDepsUrl += 'macosx/x64/'

files = dict()
# Format
# Note: The only reason for the 'Regex for Dir' is because I can't delete from bintray at the moment, so I can't rename the directory in the zipped file and reupload.
#
# [Display Name]			= [URL, 												Save Filename,		Regex for Dir, 	New Name for Dir]
files['Boost 1.55'] 		= ['{0}{1}'.format(downloadGlrDepsUrl, boostName), 		'boost.tar.gz', 	'boost', 		'boost']
files['FreeImage 3.15.1'] 	= ['{0}{1}'.format(downloadGlrDepsUrl, freeImageName), 	'freeimage.tar.gz', 'freeimage', 	'freeimage']
files['AssImp 3.1.1'] 		= ['{0}{1}'.format(downloadGlrDepsUrl, assImpName), 	'assimp.tar.gz', 	'assimp', 		'assimp']
files['CEF3 3.1750.1738'] 	= ['{0}{1}'.format(downloadGlrDepsUrl, cef3Name), 		'cef3.tar.gz', 		'cef', 			'cef3']
files['GLM 0.9.5.2'] 		= ['{0}{1}'.format(downloadGlrDepsUrl, glmName), 		'glm.tar.gz', 		'glm', 			'glm']
files['GLEW 1.10.0'] 		= ['{0}{1}'.format(downloadGlrDepsUrl, glewName), 		'glew.tar.gz', 		'glew', 		'glew']
files['SFML 2.1'] 			= ['{0}{1}'.format(downloadGlrDepsUrl, sfmlName), 		'sfml.tar.gz', 		'sfml', 		'sfml']

dependenciesDirectory = 'deps/'
librariesDirectory = 'lib/'

def copyFile(fromFile, toFile):
	try:
		shutil.copy(fromFile, toFile)
	except:
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
		savedSize = 0
		with open(dependenciesDirectory + files[k][1], 'wb+') as f:
			while 1:
				data = response.read(chunk)
				if not data:
					print("\n")
					break

				f.write( bytearray(data) )

				savedSize += len(data)
				sys.stdout.write( "\rRead {0} / {1} KB".format(((savedSize)/1000), int(contentLength)/1000) )
				sys.stdout.flush()
					

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
		print('There is no automated CEF3 wrapper dll build script for Windows yet.  There is a pre-built library included in the CEF3 library download which will be used.')

def install():
	"""Install the downloaded libraries locally"""
	
	if (not os.path.exists(librariesDirectory)):
		os.makedirs(librariesDirectory)

	cef3OutputDir = '{0}cef3/out/Release/obj.target/'.format(dependenciesDirectory)
	if isWindows:
		cef3OutputDir = '{0}cef3/lib/'.format(dependenciesDirectory)

	copyDirectory('{0}/libcef_dll_wrapper'.format(cef3OutputDir), './{0}/libcef_dll_wrapper'.format(librariesDirectory))	
	copyFile('{0}libcef_dll_wrapper.{1}'.format(cef3OutputDir, staticLibraryExt), './{0}/libcef_dll_wrapper.{1}'.format(librariesDirectory, staticLibraryExt))
	copyFile('{0}libcef.{1}'.format(cef3OutputDir, sharedLibraryExt), './{0}/libcef.{1}'.format(librariesDirectory, sharedLibraryExt))
	
	ffmpegsumoLibraryName = 'libffmpegsumo'
	if isWindows:
		ffmpegsumoLibraryName = 'ffmpegsumo'

	copyFile('{0}{1}.{2}'.format(cef3OutputDir, ffmpegsumoLibraryName, sharedLibraryExt), './{0}/libffmpegsumo.{1}'.format(librariesDirectory, sharedLibraryExt))
	
	if isWindows:
		copyFile('{0}libcef.{1}'.format(cef3OutputDir, staticLibraryExt), './{0}/libcef.{1}'.format(librariesDirectory, staticLibraryExt))



checkSystemDependencies()
installSystemDependencies()
download()
extract()
build()
install()
