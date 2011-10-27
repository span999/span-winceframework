#!/bin/bash
# enable DVI 

xrandr

##xrandr --newmode "1680x1050_60.00"  146.25  1680 1784 1960 2240  1050 1053 1059 1089 -hsync +vsync
##xrandr --addmode DVI1 "1680x1050_60.00"

xrandr --output VGA1 --off

xrandr --output DVI1 --mode "1680x1050_60.00"
##xrandr --output DVI1 --left-of VGA1
##xrandr --output DVI1 --auto

xrandr
