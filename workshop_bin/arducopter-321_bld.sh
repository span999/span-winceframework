#!/bin/sh
#

USER_OPT=$1
ARDUPILOT_VER=3.2.1
NOW_DIR=`pwd`
NOW_DATE=`date +"%Y%m%d-%H%M%S"`
RELEASE_DIR=${NOW_DIR}/../release
SCRIPT_DIR=${NOW_DIR}
ARDUPILOT_DIR=${NOW_DIR}/../APM/ardupilot
PX4FW_DIR=${NOW_DIR}/../APM/PX4Firmware
PX4NU_DIR=${NOW_DIR}/../APM/PX4NuttX
ARDUCOPTER_DIR=${ARDUPILOT_DIR}/ArduCopter
CROSS_COMPILE_DIR=/opt/gcc-arm-none-eabi-4_7-2014q2/bin
BUILDTMP_DIR=/tmp/ArduCopter.build

GIT_CMD_CLEAN="git clean -d -x -f"
CPU=4

if [ -z "${USER_OPT}" ]; then
	USER_OPT=
fi

if [ "apm" = "${USER_OPT}" ]; then
	USER_OPT=
fi
if [ "px4" = "${USER_OPT}" ]; then
	USER_OPT=px4-v1
#	USER_OPT=px4
	CPU=1
fi

if [ "pixhawk" = "${USER_OPT}" ]; then
	USER_OPT=px4-v2
	CPU=1
fi

if [ "clean" = "${USER_OPT}" ]; then
	echo "clean code ...."
fi

# create log file
touch ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "##################################" >> ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "##################################"

echo "time tag = ${NOW_DATE}" >> ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "time tag = ${NOW_DATE}"
echo "ardupilot source version = ${ARDUPILOT_VER}" >> ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "ardupilot source version = ${ARDUPILOT_VER}"
echo "release path = ${RELEASE_DIR}" >> ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "release path = ${RELEASE_DIR}"
echo "ardupilot path = ${ARDUPILOT_DIR}" >> ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "ardupilot path = ${ARDUPILOT_DIR}"
echo "arducopter path = ${ARDUCOPTER_DIR}" >> ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "arducopter path = ${ARDUCOPTER_DIR}"
echo "user build option = ${USER_OPT}" >> ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "user build option = ${USER_OPT}"

echo "##################################" >> ${RELEASE_DIR}/${NOW_DATE}_bld.log
echo "##################################"

#exit 0

cd ${PX4FW_DIR}
echo `pwd`
${GIT_CMD_CLEAN}
git checkout ArduCopter-${ARDUPILOT_VER} 

cd ${PX4NU_DIR}
echo `pwd`
${GIT_CMD_CLEAN}
git checkout ArduCopter-${ARDUPILOT_VER} 

cd ${ARDUPILOT_DIR}
echo `pwd`
rm -rf *.elf
rm -rf *.hex
${GIT_CMD_CLEAN}
#git clean -d -x -f
git checkout ArduCopter-${ARDUPILOT_VER} 

if [ "clean" = "${USER_OPT}" ]; then
	cd ${SCRIPT_DIR}
	exit 0
fi

cd ${ARDUCOPTER_DIR}
echo `pwd`
echo $PATH
export PATH=${ARDUPILOT_DIR}/Tools/autotest:${CROSS_COMPILE_DIR}:$PATH
echo $PATH
if [ -d ${BUILDTMP_DIR} ]; then
	rm -rf ${BUILDTMP_DIR}
fi 
make configure
make ${USER_OPT} -j${CPU} 2>&1 | tee -a ${RELEASE_DIR}/${NOW_DATE}_bld.log

if [ -f ${ARDUCOPTER_DIR}/ArduCopter.elf ]; then
	cp ${ARDUCOPTER_DIR}/ArduCopter.elf ${RELEASE_DIR}/${NOW_DATE}_ArduCopter.elf
	cp ${BUILDTMP_DIR}/ArduCopter.hex ${RELEASE_DIR}/${NOW_DATE}_ArduCopter.hex
	cp ${BUILDTMP_DIR}/ArduCopter.eep ${RELEASE_DIR}/${NOW_DATE}_ArduCopter.eep
fi

if [ -d ${PX4FW_DIR}/Build ]; then
	echo "PX4 build ....."
#	cp -r ${PX4FW_DIR}/Build ${RELEASE_DIR}/${NOW_DATE}_Build
fi 

cd ${SCRIPT_DIR}

exit 0

# _EOF_

