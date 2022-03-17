#! /usr/bin/python
#---------------------------------------------
#Python program for simulate lidar data
#---------------------------------------------

#Files
from src import fct_socket

import os
import pcapy

from scapy.all import *



#Record and save lidar data
def record_and_capture(device, path):
    #-------------

    #Input and output socket stuff
    cap = pcapy.open_live(device , 1300 , 1 , 0)
    client_sock = fct_socket.create_client_socket()

    # Start sniffing packets
    nb_packet = 0
    print("---- Sniffing device " + device + '----')

    while(1):
        (header, packet) = cap.next()
        wrpcap(path, packet, append=True)
        
    #-------------

#Open pcap file
def open_pcap(path):
    #-------------
    pcap = scapy.utils.rdpcap(path)
    return pcap
    #-------------

#count number of packet inside a pcap
def get_nb_paqyet(pcap):
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
