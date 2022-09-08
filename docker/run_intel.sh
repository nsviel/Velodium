#!/bin/sh

xhost +
sudo docker run \
    -it \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    --device="/dev/dri:/dev/dri" \
    --env="DISPLAY=$DISPLAY" \
    velodium
xhost -



