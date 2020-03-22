import os
import sys

pathToBuildDirectory = 'build'

try:
    os.mkdir(pathToBuildDirectory)
except FileExistsError:
    pass

os.chdir(pathToBuildDirectory)

if sys.platform == 'win32':
    print(os.popen('cmake -GNinja -DCMAKE_EXPORT_COMPILE_COMMANDS=on -DCMAKE_BUILD_TYPE=Debug .. && ninja').read())




