#!/bin/bash
# get linux kernel and make patch

_FTPPATH=ftp://ftp.kernel.org/pub/linux/kernel
_MVER=2.6
_NVER=32
_VERARRAY="1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17"



fn_get_kernel_base() 
{
if [ -f linux-$_MVER.$_NVER.tar.bz2 ]; then
    echo "=== Target file already here ==="
else
    echo "=== get code base ==="
    wget $_FTPPATH/v$_MVER/linux-$_MVER.$_NVER.tar.bz2
fi
}

fn_get_kernel_patch()
{
for _THISVER in $_VERARRAY
do
    if [ -f patch-$_MVER.$_NVER.$_THISVER.bz2 ]; then
        echo "=== Target file already here ==="
    else
        echo "=== get patch files [patch-$_MVER.$_NVER.$_THISVER.bz2] ==="
        wget $_FTPPATH/v$_MVER/patch-$_MVER.$_NVER.$_THISVER.bz2
    fi
done
}


fn_diff_kernel()
{
OLDVER=`expr $1 - 1`
echo linux-$_MVER.$_NVER.$OLDVER
echo linux-$_MVER.$_NVER.$1

tar -xvf linux-$_MVER.$_NVER.tar.bz2 > /dev/null
mv linux-$_MVER.$_NVER linux-$_MVER.$_NVER.$1
bzip2 -d patch-$_MVER.$_NVER.$1.bz2
cd linux-$_MVER.$_NVER.$1
patch -p1 < ../patch-$_MVER.$_NVER.$1 > /dev/null
cd ..
diff -Naur linux-$_MVER.$_NVER.$OLDVER linux-$_MVER.$_NVER.$1 > $_MVER.$_NVER.$OLDVER-$_MVER.$_NVER.$1.patch
rm -rf linux-$_MVER.$_NVER.$OLDVER
}


fn_diff_kernels()
{
tar -xvf linux-$_MVER.$_NVER.tar.bz2 > /dev/null
mv linux-$_MVER.$_NVER linux-$_MVER.$_NVER.0

for _THISVER in $_VERARRAY
do
    fn_diff_kernel $_THISVER
done
}


echo "=== Let's start the kernel source works !! ==="
echo "Are you ready??(y/n)"
read Ans

if [ $Ans = y -o $Ans = Y ]; then
    echo "Your answer is Yes !!"
    fn_get_kernel_base
    fn_get_kernel_patch
    fn_diff_kernels
    rm patch-*
else
    echo "Your answer is No !!"
fi
echo $Ans
echo "=== Works done !! ==="

