#!/bin/sh


sudo docker run \
    -it \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    --device="/dev/dri:/dev/dri" \
    --env="DISPLAY=99" \
    --publish=2370:2370 \
    --publish=8888:8888 \
    -v data:/app/hubium \
    --entrypoint="cd .. | pwd" \
    velodium
