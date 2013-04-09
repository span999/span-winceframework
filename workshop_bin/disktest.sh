#!/bin/bash
#

echo "read ..."
sudo hdparm -Tt /dev/mapper/isw_bhhjcaejbb_RAID10


echo "write ..."
echo "4k ..."
dd if=/dev/zero of=test1.img bs=4k count=960k
sync
#dd if=test1.img of=/dev/null bs=4k
rm test1.img
sync

echo "8k ..."
dd if=/dev/zero of=test2.img bs=8k count=480k
sync
#dd if=test2.img of=/dev/null bs=8k
rm test2.img
sync

echo "64k ..."
dd if=/dev/zero of=test3.img bs=64k count=60k
sync
#dd if=test3.img of=/dev/null bs=64k
rm test3.img
sync

echo "512k ..."
dd if=/dev/zero of=test4.img bs=512k count=8k
sync
#dd if=test4.img of=/dev/null bs=512k
rm test4.img
sync

echo "2M ..."
dd if=/dev/zero of=test5.img bs=2048k count=2k
sync
#dd if=test5.img of=/dev/null bs=2048k
rm test5.img


