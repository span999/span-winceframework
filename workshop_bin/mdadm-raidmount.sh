#!/bin/bash
#

echo "find RAID device ..."
sudo mdadm --assemble -scan

sleep 3

#if [ -e /dev/md/RAID10 ]; then
if [ -e /dev/md/ST1000DM003-0_0 ]; then
	echo "RAID device found!"
	echo "mount RAID device ST1000DM003-0_0 ..."
	# dmraid
	#sudo mount /dev/mapper/isw_bcdjjjjhih_RAID10 ~/workshop
	# mdadm
	#sudo mount /dev/md/RAID10 ~/workshop
	sudo mount /dev/md/ST1000DM003-0_0 ~/workshop
	echo "mount RAID device ST1000DM003-0_0 done!"
else
	echo "No RAID device!"
fi


if [ -e /dev/md/ST1000DM003-1_0 ]; then
	echo "RAID device found!"
	echo "mount RAID device ST1000DM003-1_0 ..."
	sudo mount /dev/md/ST1000DM003-1_0 ~/backup
	echo "mount RAID device ST1000DM003-1_0 done!"
else
	echo "No RAID device!"
fi

