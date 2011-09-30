#!/usr/bin/python
# -*- coding: utf-8 -*-


import os
import spWinToolHelps

# make a short cut of spWinToolHelps
spWTH = spWinToolHelps






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



"""
EOF
"""
