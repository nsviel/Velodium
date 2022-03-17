#Files
from src import fct_io
from src import fct_socket
from src import fct_param
from src import fct_file

#Packets
import pcapy
import sys
import socket

from scapy.all import *


print("---- Start program ----")
#-------------

# [SSD] Create directories
fct_file.create_directories()

# [LIDAR] Ask for ethernet network
fct_param.lidar_1_dev = fct_param.select_siffing_device(fct_param.lidar_1_dev)
#fct_param.lidar_2_dev = fct_param.select_siffing_device(fct_param.lidar_2_dev)

#fct_io.port_forwarding(dev_lidar, fct_scenario.IP_dst, fct_scenario.velo_port)

# [LIDAR] Save lidar data
fct_io.record_and_capture(fct_param.lidar_1_dev, fct_param.path_lidar_1)

#-------------
print("---- Stop program ----")
