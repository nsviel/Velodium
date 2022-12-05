#!/bin/sh

xhost + local:root# >/dev/null
sudo docker run \
    --network host \
    -it \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    --device="/dev/dri:/dev/dri" \
    --env="DISPLAY=$DISPLAY" \
    -p 2370:2370 \
    -p 8888:8888 \
    -v data:/app/hubium \
    velodium
xhost - >/dev/null
