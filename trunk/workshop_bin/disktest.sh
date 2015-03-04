#!/bin/bash
#
#

TOFILE=testfile.img
#TARDEV=/dev/mapper/isw_bhhjcaejbb_RAID10
#TARDEV=/dev/mapper/isw_cgadeadfgc_RAID10_1p1
#TARDEV=/dev/mapper/isw_cgadeadfgc_RAID10_2p1
#TARDEV=/dev/md126
#TARDEV=/dev/sda1
TARDEV=/dev/sde1



echo "Disk read ..."
sudo hdparm -Tt ${TARDEV}


###time sh -c "dd if=/dev/zero of=testfile bs=4k count=256k  && sync"

echo "1k ..."
dd if=/dev/zero of=${TOFILE} bs=1k count=2048k conv=fdatasync
rm ${TOFILE}
sync

echo "Disk write ..."
echo "4k ..."
dd if=/dev/zero of=${TOFILE} bs=4k count=512k conv=fdatasync
rm ${TOFILE}
sync

echo "8k ..."
dd if=/dev/zero of=${TOFILE} bs=8k count=256k conv=fdatasync
rm ${TOFILE}
sync

echo "64k ..."
dd if=/dev/zero of=${TOFILE} bs=64k count=32k conv=fdatasync
rm ${TOFILE}
sync

echo "128k ..."
dd if=/dev/zero of=${TOFILE} bs=128k count=16k conv=fdatasync
rm ${TOFILE}
sync

echo "512k ..."
dd if=/dev/zero of=${TOFILE} bs=512k count=4k conv=fdatasync
rm ${TOFILE}
sync

echo "2M ..."
dd if=/dev/zero of=${TOFILE} bs=2048k count=1k conv=fdatasync
rm ${TOFILE}
sync

