#!/bin/bash
# enable both DVI and VGA display port

xrandr

xrandr --output DVI1 --same-as VGA1
xrandr --output DVI1 --left-of VGA1

xrandr
