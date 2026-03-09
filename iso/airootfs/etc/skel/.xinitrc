#!/bin/bash

# Start the performance daemon in the background
/usr/bin/raindrop-daemon &

# Start the shell — restarts automatically if it crashes
while true; do
    /usr/bin/raindrop-shell
    sleep 1
done