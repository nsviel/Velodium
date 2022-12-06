#!/bin/sh
GREEN='\033[92m'
NC='\033[0m'


#----------------------
printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Installation ${NC}\n"
printf "${GREEN}--------------${NC}\n"
sudo apt update -y
mkdir build
cd build

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Dependencies ${NC}\n"
printf "${GREEN}--------------${NC}\n"
sudo apt update -y
sudo apt install -y git build-essential cmake libglfw3-dev \
     libglew-dev libeigen3-dev libflann-dev libboost-all-dev \
     libglm-dev gnuplot libtins-dev libjsoncpp-dev libssh-dev \
     libfreetype-dev doxygen libcurl4-openssl-dev libfreeimage-dev \
     libmicrohttpd12 libgnutls28-dev libarmadillo-dev socat

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Module ${NC}\n"
printf "${GREEN}--------------${NC}\n"
sudo sh ../src/Module/install.sh

printf "${GREEN}--------------${NC}\n"
printf "${GREEN} Libhttpserver ${NC}\n"
printf "${GREEN}--------------${NC}\n"
sudo apt install -y libmicrohttpd-dev=0.9.75-3ubuntu1 libmicrohttpd12=0.9.75-3ubuntu1
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
