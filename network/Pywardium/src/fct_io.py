#! /usr/bin/python
#---------------------------------------------
#Python program for simulate lidar data
#---------------------------------------------

#Files
import os
import pcapy

from scapy.all import *


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

def get_size_Gb(path):
    #-------------
    if os.path.exists(path):
        size = os.path.getsize(path) / 1000000000
        return size
    else:
        return 0
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
