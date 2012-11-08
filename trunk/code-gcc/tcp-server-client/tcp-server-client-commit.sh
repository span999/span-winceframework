#!/bin/bash
# do the diff & patch job for specified source folder


CURRPATH=`pwd`
FOLDER=tcp-server-client
SVNPATH=/home/span/workshop/svn/googlecode/trunk/code-gcc
BUILDFILES=./${FOLDER}
SVNFILES=${SVNPATH}/${FOLDER}

TMPPATCH=123.patch

DIFFOP1=-Naur
DIFFOP2=--exclude=".svn"



rm ${CURRPATH}/${TMPPATCH}

# A + C = B
# diff A B > C

ln -s ${BUILDFILES} B
ln -s ${SVNFILES} A

diff ${DIFFOP1} ${DIFFOP2} A B  > ${CURRPATH}/${TMPPATCH}


cd A
patch -p1 < ${CURRPATH}/${TMPPATCH}
cd -

rm A
rm B

