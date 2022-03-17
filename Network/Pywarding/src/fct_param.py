#! /usr/bin/python
#---------------------------------------------
#Data and parameters for network connections
#---------------------------------------------

import pcapy


# Paths
path_ssd = "/media/ns/lidar_ssd"
path_capture = os.path.join(path_ssd, "capture")
path_dir_1 = os.path.join(path_capture, "lidar_1")
path_dir_2 = os.path.join(path_capture, "lidar_2")
path_lidar_1 = os.path.join(path_dir_1, "lidar_1.pcap")
path_lidar_2 = os.path.join(path_dir_2, "lidar_2.pcap")

#Parameters
IP_dst = "127.0.0.1";
is_two_lidar = False
lidar_1_port = 2368
lidar_2_port = 2368
lidar_1_dev = "enp34s0"
lidar_2_dev = "enxf8e43b6cdf6c"




#ask user to enter device name to sniff
def select_siffing_device(default):
    #-------------
    devices = pcapy.findalldevs()

    print("Available devices are :")
    print("----------------------")

    cpt = 0
    for d in devices :
        if(d == default):
            print(cpt, ' - \033[92m', d, '\033[0m')
        else:
            print(cpt, ' - ', d)
        cpt += 1

    print("----------------------")
    print("Default is " + default)
    in_dev = input("Enter device to sniff [Enter for default] : ")

    #Check for default
    if(in_dev == ""):
        print("-> Selected default \033[92m" + default + "\033[0m")
        print()
        return default;

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
        print('Error command')
        exit();

    print("-> Selected device \033[92m" + dev + "\033[0m")
    print()

    return dev
    #-------------

#Create new client socket
def make_scenario(conf):
    #-------------

    #Parameters - scenario 0 testing
    if(conf == 0):
        IP_dst = "127.0.0.1";
        output_path = '../capture_lidar_2.pcap';
        is_two_lidar = False

    #Parameters - scenario ..

    #-------------
