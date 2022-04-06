#!/bin/sh

./build.sh

xhost +
docker run \
  --ipc host \
  --gpus all \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix" \
  --env="DISPLAY=$DISPLAY" \
  velodium
