#!/bin/sh

./build.sh

xhost +
docker run -it \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix" \
  --device="/dev/dri:/dev/dri" \
  --env="DISPLAY=$DISPLAY" \
  velodium bash
