#!/bin/bash
# enforce eth0 ip setting

sudo ifconfig eth0 10.60.100.108
sudo route add default gw 10.60.64.254

ifconfig eth0
