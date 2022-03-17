#! /usr/bin/python
#---------------------------------------------

import pcapy


#ask user to enter device name to sniff
def select_device(name, default):
    #-------------
    devices = pcapy.findalldevs()

    print("[\033[92mDEV\033[0m] Available devices for \033[96m" + name + "\033[0m are:")
    print("----------------------")

    cpt = 0
    for d in devices :
        if(d == default):
            print(cpt, ' - \033[92m', d, '\033[0m')
        else:
            print(cpt, ' - ', d)
        cpt += 1

    print("----------------------")
    in_dev = input("Enter device to sniff [\033[92m" + default + "\033[0m] : ")

    #Check for default
    if(in_dev == ""):
        print("[\033[92mDEV\033[0m] Selected default \033[92m" + default + "\033[0m")
        return default;

    try:
        val = int(in_dev)
    except ValueError:
        print('[\033[91mERR\033[0m] An integer is required')
        exit()


    #Check for good selected command
    good_choice = False
    cpt = 0
    dev = 0
    for d in devices:
        if(int(in_dev) == cpt):
            dev = d
            good_choice = True
        cpt += 1;

    if(good_choice == False):
        print('[\033[91mERR\033[0m] Not in list')
        exit();

    print("[\033[92mDEV\033[0m] Selected device \033[92m" + dev + "\033[0m")

    return dev
    #-------------
