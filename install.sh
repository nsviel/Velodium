#-------------
#INSTALLATION
#-------------

#!/bin/sh
echo Installation...

#Install dependancies -> Essential
sudo apt install build-essential cmake libglfw3-dev libglew-dev libeigen3-dev libflann-dev libboost-all-dev libglm-dev gnuplot 

#Install dependancies -> ?
#sudo apt install libvtk7-dev=7.1.1+dfsg2-2ubuntu1 libarmadillo-dev=1:9.800.4+dfsg-1build1 libqhull-dev=2015.2-4 git libjsoncpp-dev libtins-dev sqlite3 sqlite3-dev
#sudo apt install build-essential cmake=3.16.3-1ubuntu1 libglfw3-dev=3.3.2-1 libglew-dev=2.1.0-4 libeigen3-dev=3.3.7-2 libflann-dev=1.9.1+dfsg-9build1 libboost-all-dev=1.71.0.0ubuntu2 libglm-dev=0.9.9.7+ds-1 gnuplot=5.2.8+dfsg1-2 libvtk7-dev=7.1.1+dfsg2-2ubuntu1 libarmadillo-dev=1:9.800.4+dfsg-1build1 libqhull-dev=2015.2-4 git libjsoncpp-dev libtins-dev sqlite3 sqlite3-dev

#Install pcl library - V1.2 is requiered
#----freezing could occurs due to RAM failure -> make SWAP partition----#
#git clone https://github.com/PointCloudLibrary/pcl.git
#cd pcl && mkdir build && cd build && cmake .. && make -j4 && sudo make install 

#End
echo Installation finished

