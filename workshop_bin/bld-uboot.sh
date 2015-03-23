#!/bin/sh
#

_PWD=`pwd`
_DEF=mx6qsabresd_defconfig
_AC="arm"
#_CC="CROSS_COMPILE=/home/span/workshop/build/deviationTx/toolchains/gcc-arm-none-eabi-4_9-2014q4/bin/arm-none-eabi-"
_CC=/home/span/workshop/build/deviationTx/toolchains/gcc-arm-none-eabi-4_9-2014q4/bin/arm-none-eabi-
_CCC="ccache ${_CC}"
#_CC="CROSS_COMPILE=/home/span/workshop/bin/gcc-4.4.4-glibc-2.11.1-multilib-1.0/arm-fsl-linux-gnueabi/bin/arm-fsl-linux-gnueabi-"
#_CC=
_REL_DIR=${_PWD}
_SRC_DIR=${_PWD}/../u-boot-2015.01
_LOG=u-boot_bld.log


cd ${_SRC_DIR}
echo `pwd`
make distclean
make mrproper
make ${_DEF} ARCH=${_AC} CROSS_COMPILE=${_CC}

make -j4 ARCH=${_AC} CROSS_COMPILE=${_CC} 2>&1 | tee ${_REL_DIR}/${_LOG}

cd ${_REL_DIR}



# _EOF_
