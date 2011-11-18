#!/bin/bash
# mount ramdisk

#sudo mount -t tmpfs -o size=4096M,nr_inodes=10k,mode=0777 tmpfs /home/span/workshop/mnt/ramdisk
sudo mount -t tmpfs -o size=4096M,mode=0777 tmpfs /home/span/workshop/mnt/ramdisk


