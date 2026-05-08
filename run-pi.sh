#!/bin/bash


# We are using OpenBox, a window manager, and X11
# to run a tiny new 'session' that doesnt have a desktop environment
# perfect for us as we are trying to make it very lightweight and fullscreened
# without true full screened


#to run the game
# first build using 'sh build-pi.sh'

# next run using 'startx ./run-linux.sh' 

xrandr --query > output.txt

xrandr --fb 3840x1080 \
--output HDMI-1 --primary --mode 1920x1080 --pos 0x0 \
--output HDMI-2 --mode 1920x1080 --pos 1920x0

xset -dpms
xset s off
xset s noblank
xsetroot -solid black

sleep 1

./build/game > game.log 2>&1