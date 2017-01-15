#!/d/Programs/Python/python.exe

import subprocess
import sys
import argparse
import shutil
import os


srcPath = os.path.dirname(os.path.realpath(__file__))
buildPath = srcPath + '\\build'
installPath = srcPath + '\\gravizApp\\'

parser = argparse.ArgumentParser(description='Graviz build script')
parser.add_argument('-c', '--clean_build', action='store_const', const=True, default=False, help='Make clean build')
parser.add_argument('makeargs', nargs="?", default="install", help="make arguments")

args = parser.parse_args()

if args.clean_build and os.path.exists(buildPath):
    shutil.rmtree(buildPath, True)


cmakeArgs = [
    'cmake',
	'-G',
	'MinGW Makefiles',
	#'-DBoost_DEBUG=ON',
	'-DBoost_USE_STATIC_LIBS=ON',
    srcPath,
    '-DCMAKE_INSTALL_PREFIX=' + installPath,
]
makeArgs = [
     'mingw32-make.exe',
	 #'make.exe',
     args.makeargs
]

try:
    os.makedirs(buildPath)
    os.makedirs(installPath)
except:
    pass

if subprocess.call(cmakeArgs, cwd=buildPath) != 0:
    print("Failed to run cmake: " + repr(cmakeArgs))
    sys.exit(1)

if subprocess.call(makeArgs, cwd=buildPath) != 0:
    print("Failed to run make: " + repr(makeArgs))
    sys.exit(1)
