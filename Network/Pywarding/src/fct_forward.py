#! /usr/bin/python
#---------------------------------------------

#Record and send lidar data
def port_forwarding(dev_lidar, dst_IP, dst_PORT):
    #-------------

    #Input and output socket stuff
    cap = pcapy.open_live(dev_lidar , 1300 , 1 , 0)
    client_sock = fct_socket.create_client_socket()

    # Start sniffing packets
    nb_packet = 0
    while(1):
        (header, packet) = cap.next()

        print('Packet ', nb_packet, ' of size ', len(packet))
        nb_packet += 1

        # Send packet to velodium server
        client_sock.sendto(packet, (dst_IP, dst_PORT))
    #-------------
