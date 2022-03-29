#!/bin/sh

#export DISPLAY=:0

docker run -it \
    --gpus all \
    --env="DISPLAY" \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    velodium

