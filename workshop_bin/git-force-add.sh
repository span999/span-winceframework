#!/bin/bash
#



#_Forcelist=`git ls-files --exclude-standard -oi`
_Forcelist=`git ls-files -o`
_Gitforcecmd='git add -f specified add files'

echo "${_Forcelist}"
echo "$_Gitforcecmd"

for _Item in ${_Forcelist}
do
	echo "now force adding <${_Item}> ... "
	git add -f ${_Item}
done


