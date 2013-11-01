#!/bin/bash
#

#7z a -r -t7z -mx=9 -v2g target.7z dir/

# tar + 7z compress
#tar cf - directory | 7za a -si directory.tar.7z
# 7z + tar decompress
#7za x -so directory.tar.7z | tar xf -
#
# $1 : options
# $2 : compressed file
# $3 : source folder
# $4 : cpu cores
#
#echo "==> 7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g $1 $2"
#7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g $1 $2
#echo "==> tar cf - $2/ | 7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g -si $1"
#tar cf - $2/ | 7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g -si $1
#
# -t{Type}
# Type of archive (7z, zip, gzip, bzip2 or tar. 7z format is default) 


TOPTIONS=$1
TFILES=$2
TFOLDER=$3
TCPUS=$4
#TODAY=`date +%Y%m%d%02k%02M%02S`
TODAY=`date +%Y%m%d%02k%02M`


if [ -z $TFOLDER ]; then
	TFOLDER=$TFILES
fi

if [ "" == "$TCPUS" ]; then
	TCPUS=4
fi

echo "CPU core=$TCPUS"
if [ "$TOPTIONS" == "-c" ]; then
##	the default -m0 of -t7z is LZMA
#	tar cvf - $TFOLDER/ | 7za a -r -t7z -mx=9 -mmt=$TCPUS -ms=on -v2g -si $TFILES-$TODAY.tar.7z
#	tar cvf - $TFOLDER/ | 7za a -r -t7z -mx=9 -mmt=$TCPUS -m0=bzip2 -ms=on -v2g -si $TFILES-$TODAY.tar.7z
##	the default -m0 of -tbzip2 is BZip2
#	tar cvf - $TFOLDER/ | 7za a -r -tbzip2 -mx=9 -mmt=$TCPUS -ms=on -v2g -si $TFILES-$TODAY.tar.bz2
	tar cvf - $TFOLDER/ | 7za a -r -t7z -m0=LZMA2 -mx=9 -mmt=$TCPUS -ms=on -v2g -si $TFILES-$TODAY.tar.7z
##	the default -m0 of -tXZ is LZMA2 
#	tar cvf - $TFOLDER/ | 7za a -r -tXZ -mx=9 -mmt=$TCPUS -ms=on -v2g -si $TFILES-$TODAY.tar.xz
fi

if [ "$TOPTIONS" == "-x" ]; then
	7z x -so $TFILES | tar xf -
fi




