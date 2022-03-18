#! /usr/bin/python
#---------------------------------------------

from src import fct_param

import os


#Check existence, or create, directories
def check_directories():
    sucess = True
    #-------------

    if(os.path.exists(fct_param.path_ssd) == False):
        print("[\033[91mERR\033[0m] No SSD detected: " + fct_param.path_ssd)
        exit()

    # Create directory capture
    if(os.path.exists(fct_param.path_capture) == False):
        os.mkdir(fct_param.path_capture)
        print("[\033[92mSSD\033[0m] Directory capture created")

    # Create directory 1
    if(os.path.exists(fct_param.path_dir_1) == False):
        os.mkdir(fct_param.path_dir_1)
        print("[\033[92mSSD\033[0m] Directory 1 created")

    # Create directory 2
    if(os.path.exists(fct_param.path_dir_2) == False):
        os.mkdir(fct_param.path_dir_2)
        print("[\033[92mSSD\033[0m] Directory 2 created")

    #End message
    print("[\033[92mSSD\033[0m] Directories OK")

    #-------------
