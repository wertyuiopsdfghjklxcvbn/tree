import shutil
import os

pathToBuildDirectory = 'build'

if os.path.isdir(pathToBuildDirectory):
    ch = 'y'
    while ch != 'y' and ch != 'n':
        ch = input('delete \'' + pathToBuildDirectory + '\' folder ? y/n: ')

    if ch == 'y':
        shutil.rmtree(pathToBuildDirectory)
