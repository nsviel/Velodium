#!/bin/sh
GREEN='\033[92m'
NC='\033[0m'


mkdir build
cd build


# OBSTACLE module
echo -n "With ${GREEN}OBSTACLE${NC} module [Y/n]? "
read answer
case ${answer} in y|Y|'' ) WITH_OBSTACLE=true ;;* ) WITH_OBSTACLE=false ;;esac

# SLAM module
echo -n "With ${GREEN}SLAM${NC} module [Y/n]? "
read answer
case ${answer} in y|Y|'' ) WITH_SLAM=true ;;* ) WITH_SLAM=false WITH_CERES=false ;;esac

# CERES library for SLAM module
if [ ${WITH_SLAM} = true ]
then
  echo -n "With ${GREEN}CERES${NC} library [y|N]? "
  read answer
  case ${answer} in y|Y ) WITH_CERES=true ;;* ) WITH_CERES=false ;;esac
fi

# Radiometry module
echo -n "With ${GREEN}Radiometry${NC} module [Y/n]? "
read answer
case ${answer} in y|Y|'' ) WITH_RADIO=true ;;* ) WITH_RADIO=false WITH_RADIO=false ;;esac

# Registration module
echo -n "With ${GREEN}Registration${NC} module [Y/n]? "
read answer
case ${answer} in y|Y|'' ) WITH_REGIS=true ;;* ) WITH_REGIS=false WITH_REGIS=false ;;esac

# ICP library
if [ ${WITH_REGIS} = true ]
then
  echo -n "With ${GREEN}PCL${NC} library [y/N]? "
  read answer
  case ${answer} in y|Y ) WITH_PCL=true ;;* ) WITH_PCL=false ;;esac
fi



#----------------------
if [ ${WITH_PCL} = true ]
then
  printf "${GREEN}--------------${NC}\n"
  printf "${GREEN} PCL v1.2 ${NC}\n"
  printf "${GREEN}--------------${NC}\n"
  sudo apt install libvtk7-dev=7.1.1+dfsg2-2ubuntu1
  #----freezing could occurs due to RAM failure -> make SWAP partition----#
  git clone https://github.com/PointCloudLibrary/pcl.git
  cd pcl && mkdir build && cd build && cmake .. && make -j4 && sudo make install
  cd ../.. && rm -r pcl
fi

if [ ${WITH_CERES} = true ]
then
    printf "${GREEN}--------------${NC}\n"
    printf "${GREEN} CERES 2.0 ${NC}\n"
    printf "${GREEN}--------------${NC}\n"
    sudo apt-get install -y libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev libgflags-dev libmetis-dev
    git clone https://ceres-solver.googlesource.com/ceres-solver
    tar zxf ceres-solver-2.0.0.tar.gz
    cd ceres-solver && mkdir build && cd build && cmake .. && make -j4 && sudo make install
    cd ../.. && rm -r ceres-solver
fi

if [ ${WITH_OBSTACLE} = true ]
then


fi

if [ ${WITH_SLAM} = true ]
then
    printf "${GREEN}--------------${NC}\n"
    printf "${GREEN} Robin-map ${NC}\n"
    printf "${GREEN}--------------${NC}\n"
    sudo apt install -y robin-map-dev
    git clone https://github.com/Tessil/robin-map.git
    cd robin-map && mkdir build && cd build
    cmake .. && make -j5 && sudo make install
    cd ../.. && sudo rm -rf robin-map
fi
#----------------------

cd ..
rm -r build
