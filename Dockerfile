FROM ubuntu

ENV DEBIAN_FRONTEND noninteractive
ENV TZ Europe/Paris

# Install dependancy packages
RUN mkdir app \
    && apt-get update \
    && apt-get install -y --no-install-recommends sudo xterm build-essential mesa-utils cmake libglfw3-dev libglew-dev libeigen3-dev libglm-dev gnuplot libflann-dev libcurl4-openssl-dev libtins-dev libjsoncpp-dev libssh-dev libfreetype-dev  libfreeimage-dev libboost-all-dev libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev libgflags-dev libmicrohttpd-dev libglvnd0 libgl1 libglx0 libegl1 libxext6 libx11-6 libgl1-mesa-glx libgl1-mesa-dri \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Env vars for the nvidia-container-runtime.
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES graphics,utility,compute

# Install dependancy libraries
COPY install.sh /velodium/install.sh
RUN /velodium/install.sh

# Copy & build application
COPY . /velodium
WORKDIR /velodium/build
RUN cmake .. && make -j4

# Open port & Shared directory
EXPOSE 2370
EXPOSE 1883

RUN mkdir \
    /data \
    /data/frame \
    /data/prediction \
    /data/image \
    /data/gps
VOLUME /data

# Run application
CMD ["./executable", "ai"]

