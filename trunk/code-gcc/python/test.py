#!/usr/bin/python
# -*- coding: utf-8 -*-


import os
import spWinToolHelps

# make a short cut of spWinToolHelps
spWTH = spWinToolHelps


DIRFILELIST = 'dir.tmp'








""" check if it's a BSP folder """
""" ====================================================== """
def IsBSPfolder():
    dirsList = []

    os.system('dir /AD /B > '+DIRFILELIST)

    with open(DIRFILELIST) as openedfile:
        for readline in openedfile:
#	    print readline + '=> ', len(readline)
            dirsList.append(spWTH.spWTH_RemoveStringTailChar(readline))

    os.system('del '+DIRFILELIST)

    for item in dirsList:
        print item


    bOk = False
    if \
        spWTH.spWTH_FindPatternInListNoCase(dirsList,'files') and \
        spWTH.spWTH_FindPatternInListNoCase(dirsList,'lib') and \
        spWTH.spWTH_FindPatternInListNoCase(dirsList,'target') \
        :
        print '================================'
        print '===== Yes, it\'s a BSP !!! ====='
        print '================================'
        print '.'
        bOk = True
    else:
        print '==================================='
        print '===== No, it\'s NOT a BSP !!! ====='
        print '==================================='
        print '.'
        bOk = False

    return bOk
""" ====================================================== """




"""
main()
"""

checkfolder = "D:\workshop\python\empty\\folder"
checkfile = "D:\workshop\python\empty\\file"

if spWTH.spWTH_IsFolderEmpty( checkfolder ):
    print checkfolder+" is empty."
else:
    print checkfolder+" is not empty."

print os.path.dirname( checkfolder )
print os.path.dirname( checkfile )

print os.path.isdir( checkfolder )
print os.path.isdir( checkfile )

print os.path.isfile( checkfolder )
print os.path.isfile( checkfile )

print os.path.basename( checkfolder )
print os.path.basename( checkfile )

print os.path.exists( checkfolder )
print os.path.exists( checkfile )

IsBSPfolder()

"""
EOF
"""
