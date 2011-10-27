#!/bin/bash
# add Java 1.5 r22 to PATH


JAPATH=~/workshop/bin/jdk1.5.0_22/bin
echo $PATH

PATH=$JAPATH:$PATH
export PATH

echo $PATH
