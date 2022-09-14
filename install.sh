#!/bin/sh
GREEN='\033[92m'
NC='\033[0m'

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Installation ${NC}\n"
printf "${GREEN}--------------${NC}\n"
sudo apt update -y
mkdir build
cd build
#----------------------

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Dependencies ${NC}\n"
printf "${GREEN}--------------${NC}\n"
sudo apt update -y
sudo apt install -y git build-essential cmake libglfw3-dev libglew-dev libeigen3-dev libflann-dev libboost-all-dev libglm-dev gnuplot libtins-dev libjsoncpp-dev libssh-dev libfreetype-dev doxygen libcurl4-openssl-dev libfreeimage-dev libmicrohttpd12 libgnutls28-dev

#Install pcl library - V1.2 is requiered
#sudo apt install libvtk7-dev=7.1.1+dfsg2-2ubuntu1
#----freezing could occurs due to RAM failure -> make SWAP partition----#
#git clone https://github.com/PointCloudLibrary/pcl.git
#cd pcl && mkdir build && cd build && cmake .. && make -j4 && sudo make install 

#Install ceres 2.0
#sudo apt-get install -y libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev libgflags-dev libmetis-dev
#git clone https://ceres-solver.googlesource.com/ceres-solver
#tar zxf ceres-solver-2.0.0.tar.gz
#
#cd ceres-solver && mkdir build && cd build && cmake .. && make -j4 && sudo make install
#cd ../.. && rm -r ceres-solver 

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Eclyspe Paho v. C ${NC}\n"
printf "${GREEN}--------------${NC}\n"
git clone https://github.com/eclipse/paho.mqtt.c
cd paho.mqtt.c && cmake . -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
sudo make install
sudo ldconfig
cd .. && sudo rm -rf paho.mqtt.c

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Eclyspe Paho v. C++ ${NC}\n"
printf "${GREEN}--------------${NC}\n"
git clone https://github.com/eclipse/paho.mqtt.cpp
cd paho.mqtt.cpp && cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
sudo cmake --build build/ --target install
sudo ldconfig
cd .. && sudo rm -rf paho.mqtt.cpp

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Robin-map ${NC}\n"
printf "${GREEN}--------------${NC}\n"
sudo apt install -y robin-map-dev 
git clone https://github.com/Tessil/robin-map.git
cd robin-map && mkdir build && cd build
cmake .. && make -j5 && sudo make install
cd ../.. && sudo rm -rf robin-map

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Libhttpserver ${NC}\n"
printf "${GREEN}--------------${NC}\n"
sudo apt install -y libmicrohttpd-dev libmicrohttpd12
git clone https://github.com/etr/libhttpserver
cd libhttpserver && ./bootstrap && make -f Makefile.cvs
mkdir build && cd build && ../configure
make -j4 && sudo make install
cd ../.. && sudo rm -rf libhttpserver

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Parametrization ${NC}\n"
printf "${GREEN}--------------${NC}\n"
echo MESA_GL_VERSION_OVERRIDE=3.3
export MESA_GL_VERSION_OVERRIDE=3.3

#----------------------
cd ..
printf "${GREEN}--------------${NC}\n"
printf "${GREEN} End   \o/ ${NC}\n"
printf "${GREEN}--------------${NC}\n"

