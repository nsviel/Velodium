#! /usr/bin/python
#---------------------------------------------
#Python program for simulate lidar data
#---------------------------------------------

#Files
import fct_socket

import os
import pcapy

from scapy.all import *



#ask user to enter device name to sniff
def select_siffing_device():
    #-------------

    #show the different available devices and ask a choice
    devices = pcapy.findalldevs()
    print("Available devices are :")
    print("----------------------")

    cpt = 0
    for d in devices :
        print(cpt, ' - ', d)
        cpt += 1

    print("----------------------")
    in_dev = input("Enter device to sniff : ")

    #Check if the selected choice correspond to a device
    good_choice = False
    cpt = 0
    dev = 0
    for d in devices:
        if(int(in_dev) == cpt):
            dev = d
            good_choice = True
        cpt += 1;
    
    #if not, exit
    if(good_choice == False):
        print('Error command')
        exit();

    #if true, return device
    return dev
    #-------------


#Open pcap file
def open_pcap(path):
    #-------------
    pcap = scapy.utils.rdpcap(path)
    return pcap
    #-------------

#count number of packet inside a pcap
def get_nb_paquet(pcap):
    #-------------
    nb = 0
    for pkt in pcap:
        if pkt.haslayer(UDP):
            nb += 1
    return nb
    #-------------


#Write pcap function
def write_pcap(pcap, path, is_append):
    #-------------
    #get file size and convert it into Gb
    if os.path.exists(path):
        size = os.path.getsize(path) / 1000000000 
    else:
        size = 0
    
    #If the file is under 50 Gb save new pcap in file
    if size < 50:
        for pkt in pcap:
            if pkt.haslayer(UDP):
                wrpcap(path, pkt, append=is_append)  #appends packet to output file
            else:
                pass
    #-------------
