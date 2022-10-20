FROM ubuntu:22.04

# Install dependancy packages
RUN apt update \
    && apt install -y --no-install-recommends git libtool sudo xterm build-essential mesa-utils cmake gnuplot ca-certificates xvfb \
    libglfw3-dev libglew-dev libeigen3-dev libglm-dev libflann-dev libcurl4-openssl-dev libtins-dev libjsoncpp-dev \
    libssh-dev libfreetype-dev  libfreeimage-dev libboost-all-dev libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev libgflags-dev libglvnd0 libgl1 libglx0 libegl1 libxext6 libx11-6 libgl1-mesa-glx libgl1-mesa-dri libgnutls28-dev libmicrohttpd12 robin-map-dev libmicrohttpd-dev libmicrohttpd12 \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Env vars for the nvidia-container-runtime.
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES graphics,utility,compute

# Install dependancy libraries
COPY install.sh /app/velodium/install.sh
RUN /app/velodium/install.sh

# Copy & build application
COPY . /app/velodium
VOLUME /app/hubium/data
WORKDIR /app/velodium/build
RUN cmake .. && make -j4

# Open port
EXPOSE 2370
EXPOSE 8888

# Start app with xvfb
COPY media/run_headless.sh /app/velodium/build/run.sh
RUN chmod a+x /app/velodium/build/run.sh

# Run application
CMD ["./run.sh"]
