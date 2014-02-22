import urllib2
import tarfile
import gzip
import sys


files = dict()
files['Boost 1.55'] = ['http://dl.bintray.com/jarrettchisholm/generic/boost_1.55_x64.tar.gz', 'boost_1.55_linux_x64.tar.gz']
files['FreeImage 3.15.1'] = ['http://dl.bintray.com/jarrettchisholm/generic/freeimage_3.15.1_linux_x64.tar.gz', 'freeimage_3.15.1_linux_x64.tar.gz']
files['AssImp 3.0.1270'] = ['http://dl.bintray.com/jarrettchisholm/generic/assimp_3.0.1270_linux_x64.tar.gz', 'assimp_3.0.1270_linux_x64.tar.gz']
files['CEF3 3.1650.1562'] = ['http://dl.bintray.com/jarrettchisholm/generic/cef3_3.1650.1562_linux_x64.tar.gz', 'cef3_3.1650.1562_linux_x64.tar.gz']

def installSystemDependencies():
	"""Install any system libraries we require"""
	pass

def download():
	"""Download external library files"""
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
		
		with open(files[k][1], 'w') as f:
			f.write( bytearray(data) )

def extract():
	"""Extract external library files"""
	
	for k in files.keys():
		print('Extracting ' + k)
		
		with tarfile.TarFile.open(files[k][1], 'r:gz') as f:
			f.extractall()

def install():
	"""Install the downloaded libraries locally"""
	pass


installSystemDependencies()
download()
extract()
install()
