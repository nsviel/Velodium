#!/bin/sh

./build.sh

xhost +
docker run \
  --volume="/tmp/.X11-unix:/tmp/.X11-unix" \
  --device="/dev/dri:/dev/dri" \
  --env="DISPLAY=$DISPLAY" \
  --publish="1883" \
  velodium
