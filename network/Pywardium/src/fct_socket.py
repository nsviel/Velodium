#---------------------------------------------
#Python program fro port forwarding LiDAR data
#---------------------------------------------

import socket



#Create new client socket
def create_client_socket():
    #-------------

    client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    return client_sock
    #-------------

def truc():
    #-------------
    server_IP = "127.0.0.1"
    server_port = 2368
    buffer_size  = 1206
    velo_adress = ("127.0.0.1", 20001);

    #Create sockets
    server_sock = socket.socket (socket.AF_INET, socket.SOCK_DGRAM)
    client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    #connect server
    server_sock.bind((server_IP, server_port))
    print("UDP server up and listening")

    # Listen for incoming datagrams and resend to velodium
    # Buffer become the binary packet to be resend
    while(True):
        #Listen incoming packet
        udp_packet = server_sock.recvfrom(buffer_size)
        message = udp_packet[0]
        address = udp_packet[1]
        print(clientMsg)
        print(clientIP)

        # Send packet to velodium server
        client_sock.sendto(udp_packet, velo_adress)

        # Check for velodium message and resend it to lidar
        velo_message = client_sock.recvfrom(buffer_size)
        server_sock.sendto(velo_message, velo_adress)


    #Stop connection
    server_sock.close()
    client_sock.close()
    #-------------





