FROM ubuntu:20.04

# Install dependancy packages
RUN apt-get update && DEBIAN_FRONTEND=noninteractive TZ=Europe/Paris \
    apt-get install -y --no-install-recommends \
    sudo xterm build-essential mesa-utils cmake libglfw3-dev libglew-dev libeigen3-dev libflann-dev libboost-all-dev libglm-dev \
    gnuplot libtins-dev libjsoncpp-dev libssh-dev libfreetype-dev libcurl4-openssl-dev libfreeimage-dev \
    libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev libgflags-dev \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Install dependancy libraries
COPY install.sh /Velodium/install.sh
RUN /Velodium/install.sh

# Copy application
COPY . /Velodium

# Build application
WORKDIR /Velodium/build
RUN cmake .. && make -j4

# Open port & Shared directory
EXPOSE 2370
VOLUME /Velodium/media/data

# Run application
CMD ["./executable"]

