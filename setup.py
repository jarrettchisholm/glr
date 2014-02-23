import urllib2
import tarfile
import sys, os
import glob
import subprocess
import shutil


files = dict()
files['Boost 1.55'] 		= ['http://dl.bintray.com/jarrettchisholm/generic/boost_1.55_x64.tar.gz', 				'boost.tar.gz', 	'boost', 		'boost']
files['FreeImage 3.15.1'] 	= ['http://dl.bintray.com/jarrettchisholm/generic/freeimage_3.15.1_linux_x64.tar.gz', 	'freeimage.tar.gz', 'freeimage', 	'freeimage']
files['AssImp 3.0.1270'] 	= ['http://dl.bintray.com/jarrettchisholm/generic/assimp_3.0.1270_linux_x64.tar.gz', 	'assimp.tar.gz', 	'assimp', 		'assimp']
files['CEF3 3.1650.1562'] 	= ['http://dl.bintray.com/jarrettchisholm/generic/cef3_3.1650.1562_linux_x64.tar.gz', 	'cef3.tar.gz', 		'cef', 			'cef3']
dependenciesDirectory = 'deps/'
librariesDirectory = 'lib/'

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
	print("\n")

def build():
	"""Build any dependencies"""
	print('Building the CEF3 wrapper dll')
	command = 'bash ./build_cef.sh'
	subprocess.call( command, shell=True)

def install():
	"""Install the downloaded libraries locally"""
	
	if (not os.path.exists(librariesDirectory)):
		os.makedirs(librariesDirectory)
	
	try:
		shutil.copytree(dependenciesDirectory + 'cef3/out/Release/obj.target/libcef_dll_wrapper', './{0}/libcef_dll_wrapper'.format(librariesDirectory))
	except:
		#print('Failed to copy cef wrapper!')
		pass
	
	try:
		shutil.copy(dependenciesDirectory + 'cef3/out/Release/obj.target/libcef_dll_wrapper.a', './{0}/libcef_dll_wrapper.a'.format(librariesDirectory))
	except:
		#print('Failed to copy cef wrapper library!')
		pass
	
	try:
		shutil.copy(dependenciesDirectory + 'cef3/Release/libcef.so', './{0}/libcef.so'.format(librariesDirectory))
	except:
		#print('Failed to copy cef library!')
		pass
	
	try:
		shutil.copy(dependenciesDirectory + 'cef3/Release/libffmpegsumo.so', './{0}/libffmpegsumo.so'.format(librariesDirectory))
	except:
		#print('Failed to copy ffmpegsumo library!')
		pass


installSystemDependencies()
download()
extract()
build()
install()
