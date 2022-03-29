FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive
ENV TZ Europe/Paris

# Install dependancy packages
RUN mkdir app \
    && apt-get update \
    && apt-get install -y --no-install-recommends sudo xterm build-essential mesa-utils cmake libglfw3-dev libglew-dev libeigen3-dev libflann-dev libboost-all-dev libglm-dev gnuplot libtins-dev libjsoncpp-dev libssh-dev libfreetype-dev libcurl4-openssl-dev libfreeimage-dev libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev libgflags-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Install dependancy libraries
COPY install.sh /app/install.sh
RUN /app/install.sh

# Copy & build application
COPY . /app
WORKDIR /app/build
RUN cmake .. && make -j4

# Open port & Shared directory
EXPOSE 2370
VOLUME /app/media/data

# Env vars for the nvidia-container-runtime.
ENV NVIDIA_VISIBLE_DEVICES all
ENV NVIDIA_DRIVER_CAPABILITIES graphics,utility,compute
ENV MESA_GL_VERSION_OVERRIDE=3.3
ENV LIBGL_ALWAYS_SOFTWARE=1

# Run application
CMD ["./executable"]

