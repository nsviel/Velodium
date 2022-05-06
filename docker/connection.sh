#!/bin/bash

echo -e "[\e[92m--- Created LiDAR NetworkManager connections ---\e[0m]"
connection=$(nmcli connection show)

#--- LiDAR 1 connection setup ---
connection_name=lidar_1
device=enxf8e43b6cdf6c
ip=192.168.1.221/24

if [[ $connection == *"$substr"* ]];
then
    sudo nmcli connection modify $connection_name ipv4.addresses $ip ipv4.method manual ipv6.method disabled
    echo -e "[\e[92mok\e[0m] \e[36m$connection_name\e[0m [\e[36m$ip\e[0m] on device \e[36m$device\e[0m already exists"
else
    sudo nmcli connection add con-name $connection_name ifname $device type ethernet
    sudo nmcli connection modify $connection_name ipv4.addresses $ip ipv4.method manual ipv6.method disabled
    sudo nmcli connection up $connection_name
    echo -e "[\e[92mok\e[0m] \e[36m$connection_name\e[0m [\e[36m$ip\e[0m] on device \e[36m$device\e[0m created"
fi

#--- LiDAR 2 connection setup ---
connection_name=lidar_2
device=enxf8e43b6cecab
ip=192.168.1.222/22

if [[ $connection == *"$substr"* ]];
then
    sudo nmcli connection modify $connection_name ipv4.addresses $ip ipv4.method manual ipv6.method disabled
    echo -e "[\e[92mok\e[0m] \e[36m$connection_name\e[0m [\e[36m$ip\e[0m] on device \e[36m$device\e[0m already exists"
else
    sudo nmcli connection add con-name $connection_name ifname $device type ethernet
    sudo nmcli connection modify $connection_name ipv4.addresses $ip ipv4.method manual ipv6.method disabled
    sudo nmcli connection up $connection_name
    echo -e "[\e[92mok\e[0m] \e[36m$connection_name\e[0m [\e[36m$ip\e[0m] on device \e[36m$device\e[0m created"
fi


