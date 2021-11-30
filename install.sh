#-------------
#INSTALLATION
#-------------

#!/bin/sh
echo Installation...

#Install dependancies -> Essential
sudo apt install git build-essential cmake libglfw3-dev libglew-dev libeigen3-dev libflann-dev libboost-all-dev libglm-dev gnuplot libtins-dev libjsoncpp-dev 


#Install pcl library - V1.2 is requiered
#sudo apt install libvtk7-dev=7.1.1+dfsg2-2ubuntu1
#----freezing could occurs due to RAM failure -> make SWAP partition----#
#git clone https://github.com/PointCloudLibrary/pcl.git
#cd pcl && mkdir build && cd build && cmake .. && make -j4 && sudo make install 

#Install ceres 2.0
sudo apt-get install libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev
git clone https://ceres-solver.googlesource.com/ceres-solver
tar zxf ceres-solver-2.0.0.tar.gz
cd ceres-solver && mkdir build && cmake .. && make -j4
make test && make install

#Install robin-map
git clone https://github.com/Tessil/robin-map.git
cd robin-map && mkdir build && cd build
cmake .. && make -j5 && sudo make install

export MESA_GL_VERSION_OVERRIDE=3.3

#End
echo Installation finished

