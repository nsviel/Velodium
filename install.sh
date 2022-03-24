#-------------
#INSTALLATION
#-------------

#!/bin/sh
echo Installation...
sudo apt update

#Install dependancies -> Essential
sudo apt install -y git build-essential cmake libglfw3-dev libglew-dev libeigen3-dev libflann-dev libboost-all-dev libglm-dev gnuplot libtins-dev libjsoncpp-dev robin-map-dev libssh-dev libfreetype-dev doxygen libcurl4-openssl-dev libfreeimage-dev


#Install pcl library - V1.2 is requiered
#sudo apt install libvtk7-dev=7.1.1+dfsg2-2ubuntu1
#----freezing could occurs due to RAM failure -> make SWAP partition----#
#git clone https://github.com/PointCloudLibrary/pcl.git
#cd pcl && mkdir build && cd build && cmake .. && make -j4 && sudo make install 

#Install ceres 2.0
#sudo apt-get install -y libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev libgflags-dev
#git clone https://ceres-solver.googlesource.com/ceres-solver
#tar zxf ceres-solver-2.0.0.tar.gz
#cd ceres-solver && mkdir build && cd build && cmake .. && make -j4
#sudo make install
#cd ../.. && rm -r ceres-solver 

#Install Eclysp Paho for MQTT 
#version C
git clone https://github.com/eclipse/paho.mqtt.c
cd paho.mqtt.c && cmake . -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
sudo make install
sudo ldconfig
cd .. && rm -r paho.mqtt.c

#version C++
git clone https://github.com/eclipse/paho.mqtt.cpp
cd paho.mqtt.cpp && cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=TRUE -DPAHO_BUILD_SAMPLES=TRUE
sudo cmake --build build/ --target install
sudo ldconfig
cd .. && rm -r paho.mqtt.cpp

#Install robin-map
git clone https://github.com/Tessil/robin-map.git
cd robin-map && mkdir build && cd build
cmake .. && make -j5 && sudo make install
cd ../.. && rm -r robin-map

export MESA_GL_VERSION_OVERRIDE=3.3

#End
echo Installation finished

