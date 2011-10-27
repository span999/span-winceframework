#!/bin/bash
# shell script of my own


MYSTARTPATH=/home/span/workshop/bin
JAVA5TOOLPATH=/home/span/workshop/bin/jdk1.5.0_22/bin

###export PATH=${JAVA5TOOLPATH}:${PATH}
export PATH=${MYSTARTPATH}:${PATH}
echo "system path now set as below:"
echo "==> $PATH"

echo "=========================="
echo "=========================="
echo "===   span's workshop  ==="
echo "=== jump to workshop !! =="
echo "=========================="
echo "=========================="
cd /home/span/workshop

echo "iBus enabled!!"
export GTK_IM_MODULE=ibus
export XMODIFIERS=@im=ibus
export QT_IM_MODULE=ibus 




