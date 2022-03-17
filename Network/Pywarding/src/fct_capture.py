#! /usr/bin/python
#---------------------------------------------

import fct_io
import fct_socket
import pcapy



#Record and save lidar data
def start_record_and_capture(output_path):
    #-------------
    # Select device and start 
    dev_lidar = fct_io.select_siffing_device()

    #Input and output socket stuff
    cap = pcapy.open_live(dev_lidar , 1300 , 1 , 0)
    client_sock = fct_socket.create_client_socket()

    # Start sniffing packets
    nb_packet = 0
    print("---- Sniffing device " + dev_lidar + '----')

    while(1):
        (header, packet) = cap.next()
        wrpcap(output_path, packet, append=True) 
    #-------------
