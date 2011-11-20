#!\bin\bash
# flush swap disk back to RAM


err="not enough RAM to write swap back, nothing done"
mem=`free|grep Mem:|awk '{print $4}'`
swap=`free|grep Swap:|awk '{print $3}'`
test $mem -lt $swap && echo -e $err && exit 1
swapoff -a && swapon -a &&
exit 0


