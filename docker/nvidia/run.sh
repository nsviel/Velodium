#!/bin/sh

xhost +
docker run \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix" \
  --device="/dev/dri:/dev/dri" \
  --env="DISPLAY=$DISPLAY" \
  velodium_nvidia
