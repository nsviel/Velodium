#! /usr/bin/python
#---------------------------------------------
#Data and parameters for network connections
#---------------------------------------------

import os


# Paths
path_ssd = "/media/ns/lidar_ssd"
path_capture = os.path.join(path_ssd, "capture")
path_dir_1 = os.path.join(path_capture, "lidar_1")
path_dir_2 = os.path.join(path_capture, "lidar_2")
path_lidar_1 = os.path.join(path_dir_1, "lidar_1.pcap")
path_lidar_2 = os.path.join(path_dir_2, "lidar_2.pcap")

# Parameters
run = True;
with_two_lidar = False
with_writing = True
with_forwarding = True

velo_IP = "127.0.0.1";
velo_port = 2370
lidar_1_dev = "enp34s0"
lidar_2_dev = "enxf8e43b6cdf6c"

# Statistics
lidar_1_nb_packet = 0
lidar_2_nb_packet = 0
duration = 0
