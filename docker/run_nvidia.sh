#!/bin/sh

xhost + >/dev/null
sudo docker run \
    --gpus all \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix" \
    --env="DISPLAY=$DISPLAY" \
    --publish=2370:2370 \
    --publish=8888:8888 \
    -v data:/app/hubium \
    velodium
xhost - >/dev/null
