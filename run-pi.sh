#!/bin/bash


# We are using OpenBox, a window manager, and X11
# to run a tiny new 'session' that doesnt have a desktop environment
# perfect for us as we are trying to make it very lightweight and fullscreened
# without true full screened


#to run the game
# first build using 'sh build-pi.sh'

# next run using 'startx ./run-linux.sh' 

xset -dpms
xset s off
xset s noblank
xsetroot -solid black

./build/game