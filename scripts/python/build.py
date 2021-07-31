import os
import sys
import subprocess


pathToBuildDirectory = 'build'

try:
    os.mkdir(pathToBuildDirectory)
except FileExistsError:
    pass

os.chdir(pathToBuildDirectory)

if sys.platform == 'win32':
    print(subprocess.run(['cmake', '-GNinja', '-DCMAKE_EXPORT_COMPILE_COMMANDS=on', '-DCMAKE_BUILD_TYPE=Debug', './..'], check = True))
    print(subprocess.run(['ninja', '-d', 'stats'], shell=True))

#process = subprocess.Popen( ['ninja', '-d', 'stats'], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

#print(process.wait())


