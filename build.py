#!/d/Programs/Python/python.exe

import subprocess
import sys
import argparse
import shutil
import os


srcPath = os.path.dirname(os.path.realpath(__file__))
buildPath = srcPath + '\\build'
installPath = srcPath + '\\gravizApp\\'

targetOptions = [
	"linux",
	"win32"
]

parser = argparse.ArgumentParser(description='Graviz build script')
parser.add_argument('-c', '--clean_build', action='store_const', const=True, default=False, help='Make clean build')
parser.add_argument('-t', '--target', default="linux", choices=targetOptions, help="operation system target = { linux, win }")
parser.add_argument('makeargs', nargs="?", default="install", help="make arguments")

args = parser.parse_args()

if args.clean_build and os.path.exists(buildPath):
    shutil.rmtree(buildPath, True)


cmakeArgs = [
    'cmake',
	'-DBoost_USE_STATIC_LIBS=ON',
    srcPath,
    '-DCMAKE_INSTALL_PREFIX=' + installPath,
	'-DGRAVIZ_TARGET_NAME=' + args.target,
]

if args.target == "win32":
	cmakeArgs.append('-G')
	cmakeArgs.append('MinGW Makefiles')

makeArgs = []
if args.target == 'linux':
	makeArgs.append('make')
else:
	makeArgs.append('mingw32-make.exe')
makeArgs.append(args.makeargs)


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
