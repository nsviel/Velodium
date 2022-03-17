#! /usr/bin/python
#---------------------------------------------

from src import fct_forward

import threading



class MonThread (threading.Thread):
    def __init__(self, jusqua):      # jusqua = donnée supplémentaire
        threading.Thread.__init__(self)  # ne pas oublier cette ligne
        # (appel au constructeur de la classe mère)
        self.jusqua = jusqua           # donnée supplémentaire ajoutée à la classe

    def run(self, dev_lidar):
        fct_forward.port_forwarding(fct_param.dev_lidar, fct_param.IP_dst, fct_param.velo_port)
)

#Main program loop
def loop(dev_1, dev_2):
    #-------------

    while(1):
        capture = pcapy.open_live(dev_1 , 1300 , 1 , 0)
        client_sock = fct_socket.create_client_socket()


    #-------------
