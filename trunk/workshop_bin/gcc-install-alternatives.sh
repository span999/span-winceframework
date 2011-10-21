#!/bin/bash

# you should install gcc-4.4/gcc-4.3 g++-4.4/g++-4.4 gcov-4.4/gcov-4.3 in software center
# then you can use update-alternatives to do it

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.4 44 \
--slave /usr/bin/g++ g++ /usr/bin/g++-4.4 \
--slave /usr/bin/gcov gcov /usr/bin/gcov-4.4

sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.3 43 \
--slave /usr/bin/g++ g++ /usr/bin/g++-4.3 \
--slave /usr/bin/gcov gcov /usr/bin/gcov-4.3

# use below command for config
# sudo update-alternatives--config gcc
