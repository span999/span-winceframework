#!/usr/bin/python
# diff and patch


import os


PATCHEXT = '.patch'
TARGETROOT = '/home/span/workshop/svn/checkout/ti/ANDROIDDEVKIT-GINGERBREAD/rowboat-android'
SOURCEROOT = '/home/span/workshop/git/source-git/ti/rowboat-android'


""" ================================================== """
def CreatePatch( oldFolder, newFolder, patchName ):

	os.system( 'rm '+patchName+PATCHEXT )
	print '>>> diff -ruN --exclude-from=ignoreFILE '+oldFolder+' '+newFolder+' > '+patchName+PATCHEXT
	os.system( 'diff -ruN --exclude-from=ignoreFILE '+oldFolder+' '+newFolder+' > '+patchName+PATCHEXT )

""" ================================================== """


""" remove last char in string """
""" ====================================================== """
def spWTH_RemoveStringTailChar( stringIn ):
    lastchar = stringIn[len(stringIn)-1]
    return stringIn.strip(lastchar)
""" ====================================================== """


dlist = []

"""
main()
"""

print \
"""
This script try to helps you from android code upgrade.
please use compare tool after patching by this script, 
since the diff dosen't handle binary well.
------------------------- powered by span.liu 10/7/2011
"""


os.system( 'ln -s '+TARGETROOT+' targetroot' )
os.system( 'ln -s '+SOURCEROOT+' sourceroot' )


os.system( 'ls sourceroot/ > dirLIST' )

with open("dirLIST") as dirlist:
	for readline in dirlist:
#		print readline
		dlist.append( spWTH_RemoveStringTailChar(readline) )


for item in dlist:
	CURR = item
	print '>>> '+CURR+' <<<'
	CreatePatch( 'targetroot/'+CURR, 'sourceroot/'+CURR, CURR )
	print ' '


#CreatePatch( o, n, CURR )




os.system( 'rm dirLIST' )

os.system( 'rm targetroot' )
os.system( 'rm sourceroot' )


