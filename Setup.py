import os
import platform
import subprocess

if __name__ == '__main__':
    my_system = platform.system()

    print('Your detected System is: ' + my_system)
    print('Running premake...')

    if (my_system == 'Windows'):
        subprocess.call(["vendor/premake5/Windows/premake5.exe", "vs2022"])
    elif (my_system == 'Linux'):
        subprocess.call(["chmod", "+x", "vendor/premake5/Linux/premake5"])
        subprocess.call(["vendor/premake5/Linux/premake5", "gmake"])
    elif (my_system == 'Darwin'):
        subprocess.call(["chmod", "+x", "vendor/premake5/MacOS/premake5"])
        subprocess.call(["vendor/premake5/MacOS/premake5", "xcode4"])
        
