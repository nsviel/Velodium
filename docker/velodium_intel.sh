#!/bin/sh

./build.sh

xhost +
docker run \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix" \
    --device="/dev/dri:/dev/dri" \
    --env="DISPLAY=$DISPLAY" \
    --publish="1883" \ #Localhost mqtt
    --publish="2368:2368/udp" \
    --publish="2369:2369/udp" \
    --publish="2370:2370/udp" \
    velodium

