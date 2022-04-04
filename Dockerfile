FROM ubuntu:20.04

ENV DEBIAN_FRONTEND noninteractive
ENV TZ Europe/Paris

# Install dependancy packages
RUN mkdir app \
    && apt-get update \
    && apt-get install -y --no-install-recommends sudo xterm build-essential mesa-utils cmake libglfw3-dev libglew-dev libeigen3-dev libflann-dev libboost-all-dev libglm-dev gnuplot libtins-dev libjsoncpp-dev libssh-dev libfreetype-dev libcurl4-openssl-dev libfreeimage-dev libgoogle-glog-dev libgflags-dev libatlas-base-dev libsuitesparse-dev libgflags-dev libgl1-mesa-glx libgl1-mesa-dri \
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


# Run application
CMD ["./executable"]

