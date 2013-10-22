#!/bin/bash
#

echo "Un-mount RAID device ..."

if [ -e ~/workshop/bin ]; then
	echo "RAID device found!"
	echo "Un-mount RAID device ST1000DM003-0_0 ..."
	sudo umount ~/workshop
	echo "Un-mount RAID device ST1000DM003-0_0 done!"
else
	echo "No RAID device!"
fi


if [ -e ~/backup/bin ]; then
	echo "RAID device found!"
	echo "Un-mount RAID device ST1000DM003-1_0 ..."
	sudo umount ~/backup
	echo "Un-mount RAID device ST1000DM003-1_0 done!"
else
	echo "No RAID device!"
fi

