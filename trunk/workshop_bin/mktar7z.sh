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
#
#echo "==> 7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g $1 $2"
#7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g $1 $2
#echo "==> tar cf - $2/ | 7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g -si $1"
#tar cf - $2/ | 7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g -si $1

TOPTIONS=$1
TFILES=$2
TFOLDER=$3
#TODAY=`date +%Y%m%d%02k%02M%02S`
TODAY=`date +%Y%m%d%02k%02M`


if [ -z $TFOLDER ]; then
	TFOLDER=$TFILES
fi

if [ "$TOPTIONS" == "-c" ]; then
	tar cf - $TFOLDER/ | 7z a -r -t7z -mx=9 -mmt=on -ms=on -v2g -si $TFILES-$TODAY.tar.7z
fi

if [ "$TOPTIONS" == "-x" ]; then
	7z x -so $TFILES | tar xf -
fi




