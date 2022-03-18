#! /usr/bin/python
#---------------------------------------------

from src import fct_param
from src import fct_io

import sys


def loop_nb_packet():
    #-------------

    sys.stdout.write("LiDAR 1 packet: [\033[94m%d\033[0m] - LiDAR 2 packet: [\033[94m%d\033[0m]\r" % (fct_param.lidar_1_nb_packet, fct_param.lidar_2_nb_packet))
    sys.stdout.flush()

    #-------------

def show_parameter():
    #-------------

    print("[\033[92mOPT\033[0m] Parameters:")
    print("----------------------")

    print(" path SSD - [\033[94m%s\033[0m]" % fct_param.path_ssd)
    print(" Velodium IP - [\033[94m%s\033[0m]" % fct_param.velo_IP)
    print(" Velodium port - [\033[94m%d\033[0m]" % fct_param.velo_port)
    print(" with_two_lidar - [\033[94m%s\033[0m]" % str(fct_param.with_two_lidar))
    print(" with_writing - [\033[94m%s\033[0m]" % str(fct_param.with_writing))
    print(" with_forwarding - [\033[94m%s\033[0m]" % str(fct_param.with_forwarding))

    print("----------------------")

    #-------------

def show_stat():
    #-------------

    print("----------------------")
    print("[\033[92mSTA\033[0m] Statistics:")

    print(" [\033[96mCapture\033[0m] Time - [\033[94m%d\033[0m]s" % fct_param.duration)

    print(" [\033[96mLiDAR 1\033[0m] Number of packet - [\033[94m%d\033[0m]" % fct_param.lidar_1_nb_packet)
    print(" [\033[96mLiDAR 1\033[0m] Size of capture file - [\033[94m%.2f\033[0m]Gb" % fct_io.get_size_Gb(fct_param.path_lidar_1))

    print(" [\033[96mLiDAR 2\033[0m] Number of packet - [\033[94m%d\033[0m]" % fct_param.lidar_2_nb_packet)
    print(" [\033[96mLiDAR 2\033[0m] Size of capture file - [\033[94m%.2f\033[0m]Gb" % fct_io.get_size_Gb(fct_param.path_lidar_2))

    #-------------
