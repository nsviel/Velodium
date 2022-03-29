#!/bin/sh

#export DISPLAY=:0

# Allow X server connection
xhost +local:root

docker run -it --rm \
    --env="DISPLAY" \
    --env="QT_X11_NO_MITSHM=1" \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    velodium

# Disallow X server connection
xhost -local:root
