#! /usr/bin/python
#---------------------------------------------

from src import fct_param

import os


#Check existence, or create, directories
def create_directories():
    #-------------

    # Create directory capture
    sucess = 0
    try:
        os.mkdir(fct_param.path_capture)
        print("[SSD] Directory capture created")
    except OSError as error:
        sucess = sucess + 1

    # Create directory 1
    try:
        os.mkdir(fct_param.path_dir_1)
        print("[SSD] Directory 1 created")
    except OSError as error:
        sucess = sucess + 1

    # Create directory 2
    try:
        os.mkdir(fct_param.path_dir_2)
        print("[SSD] Directory 2 created")
    except OSError as error:
        sucess = sucess + 1

    #End message
    print("[SSD] Directories OK")

    #-------------
