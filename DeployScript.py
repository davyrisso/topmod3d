
#This file is used for deployment of the topmod
#make sure the following variables exist and update them if necessay
#This script was tested for g++ (MinGW.org GCC-8.2.0-5) 8.2.0
qtDeployFolder="C:/Qt/5.12.0/mingw73_64/bin"
deploy="windeployqt.exe"
topModBuildFolder="D:/TM/build-TopMod-Desktop_Qt_5_12_0_MinGW_64_bit-Release/topmod/release"

import os, shutil
from pathlib import Path

#remove build files. Maybe some other files are needed to store the preferences? 
for f in os.listdir(topModBuildFolder):
	file_path = os.path.join(topModBuildFolder, f)
	try:
		if os.path.isfile(file_path) or os.path.islink(file_path):
			filename, file_extension = os.path.splitext(f)
			if file_extension != '.exe':
				print("deleting file {}".format(file_path))
				os.unlink(file_path)
		#elif os.path.isdir(file_path):
			#shutil.rmtree(file_path)
	except Exception as e:
		print('Failed to delete %s. Reason: %s' % (file_path, e))

#run qt deploy tool
cmd = "{}/{} {}".format(qtDeployFolder,deploy,topModBuildFolder)
os.system(cmd);

#copy the GCC DLLs from the qtDeploy folder to the topModBuildFolder if you are missing something
for f in os.listdir(qtDeployFolder):
	filename, file_extension = os.path.splitext(f)
	if file_extension == ".dll" and "lib" in filename:
		try:
			file_path = os.path.join(qtDeployFolder, f)
			shutil.copy(file_path, topModBuildFolder)
		except Exception as e:
			print('Failed to delete %s. Reason: %s' % (file_path, e))
		
