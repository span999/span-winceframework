#!/usr/bin/python
# -*- coding: utf-8 -*-

""" import area """
import datetime
import os
import time
import string


""" declare """
REPLACETARGETFILE = 'platform.bib'
REPLACEBACKUPFILE = 'platform.abg'
REPLACETEMPFILE = 'platform.tmp'
DLLFILELIST = 'dlllist.txt'
DLLFILELISTB = 'dlllist2.txt'
DIRFILELIST = 'dir.tmp'
INSERTKEY = '\\dllbinary\\'
DLLBINARYFOLDER = 'dllbinary'

getList = [ '--1.start--' ]
getsList = [ '--2.start--' ]
dirList = [ '--3.start--' ]



""" check if it's a BSP folder """
""" ====================================================== """
def IsBSPfolder():
    bOk = False
    os.system('dir /AD /B > '+DIRFILELIST)
    with open(DIRFILELIST) as openedfile:
	for readline in openedfile:
#	    print readline + '=> ', len(readline)
            lastchar = readline[len(readline)-1]
            stripedline = readline.strip(lastchar)
            dirList.append(stripedline)
        dirList.append('--end--')

    for item in dirList:
        print item

    os.system('del '+DIRFILELIST)

    if \
        dirList.count('files') > 0 and \
        dirList.count('lib') > 0 and \
        dirList.count('target') > 0 \
        :
        print '===== Yes, it\'s a BSP !!! ====='
        bOk = True
    else:
        print '===== No, it\'s NOT a BSP !!! ====='
        bOk = False
    return bOk
""" ====================================================== """
	

""" copy dll binary to files folder """
""" ====================================================== """
def CopyDllBinary():
#	os.system('del '+DLLFILELISTB)
    os.system('dir /S /B target\\*.dll > '+DLLFILELISTB)
    os.system('dir /S /B target\\*.exe >> '+DLLFILELISTB)
    os.system('dir /S /B target\\*.rel >> '+DLLFILELISTB)

    with open(DLLFILELISTB) as openedfile:
        for readline in openedfile:
#            print readline + '=> ', len(readline)
            lastchar = readline[len(readline)-1]
            stripedline = readline.strip(lastchar)
            getList.append(stripedline)

        getList.append('--end--')

#	for item in getList:
#		print item

    os.system('del '+DLLFILELISTB)

    os.system('rmdir /S /Q files\\'+DLLBINARYFOLDER)
    os.system('mkdir files\\'+DLLBINARYFOLDER)

    for item in getList:
        print 'copy '+item+' to files\\'+DLLBINARYFOLDER
        os.system('copy '+item+' files\\'+DLLBINARYFOLDER+'\\')
        if \
            item.count('oal.exe') > 0 or \
            item.count('oal.rel') > 0  \        
            :
            os.system('copy '+item+' files\\')

""" ====================================================== """


""" create list with all dll used """
""" ====================================================== """
def GetDllList():
    os.system('del '+DLLFILELIST)
    os.system('dir /B target\\ARMV4I\\retail\\*.dll > '+DLLFILELIST)
    os.system('dir /B target\\ARMV4I\\retail\\*.exe >> '+DLLFILELIST)

    with open(DLLFILELIST) as openedfiles:
        for readline in openedfiles:
            print readline + '=> ', len(readline)
            lastchar = readline[len(readline)-1]
            stripedline = readline.strip(lastchar)
            getsList.append(stripedline)

    getsList.append('--end--')

    for item in getsList:
        print item

    os.system('del '+DLLFILELIST)
""" ====================================================== """


""" write line to tail of file """
""" ====================================================== """
def WritelineToFile( line ):
	writeto = open('files\\'+REPLACETEMPFILE,'a+')
	writeto.seek(0, os.SEEK_END)
	writeto.write(line)
	writeto.newlines
	writeto.flush()
	writeto.close()
""" ====================================================== """
	

""" handle line with replacement """
""" ====================================================== """
def DoLineparce( line ):
#	print line, len(line)
	for dllname in getsList:
#		print 'looking for ' + dllname + ' in (' + line + ')'
#		if dllname in line:
#		if -1 < string.find(line,dllname):
		if 0 < line.count(dllname):
			print 'hit "'+dllname+'" and replace it! '
			WritelineToFile(';abg; replace >> '+'"\\'+dllname+'" with "'+INSERTKEY+dllname+'"')
			WritelineToFile('\r')
			WritelineToFile(';abg; '+line)
			WritelineToFile(line.replace('\\'+dllname,INSERTKEY+dllname,2))
#		else:
#			print dllname
#			WritelineToFile(line)
#	WritelineToFile(line)
""" ====================================================== """


""" handle line without replacement """	
""" ====================================================== """
def nDoLineparce( line ):
#	print 'ignore >> ' + line, len(line)
#	WritelineToFile('ignore  >> '+line)
	WritelineToFile(line)
""" ====================================================== """

	


""" main start """
""" ====================================================== """

print \
"""
======================================================
=====   start auto binary BSP generation (abg)   =====
=====                                            =====
=====   This tool helps to generate the binary   =====
=====   BSP from normal BSP (WinCE BSP)          =====
=====                                            =====
=====   press 'Y' to start, or 'N' to exit       =====
======================================================

"""

select = raw_input('--> (Y/N) ')

if select != 'Y':
    exit()

""" check BSP """
if False == IsBSPfolder():
    exit()

""" copy dll binary """
CopyDllBinary()

""" retrive dll list in simple format """
GetDllList()

	
""" parse platform.bib """
os.system('del files\\'+REPLACETEMPFILE)

with open('files\\'+REPLACETARGETFILE) as openedfile:
    for readline in openedfile:
        if \
            readline[0] != '#' and \
            readline[0] != 'I' and \
            readline[0] != 'E' and \
            readline[0] != ';' \
            :
#           print readline, len(readline)
            DoLineparce( readline )
        else:
#           print 'ignore >> ' + readline, len(readline)
            nDoLineparce( readline )
			
os.system('rename files\\platform.bib platform.abg')
os.system('rename files\\'+REPLACETEMPFILE+' platform.bib')

os.system('rmdir /S /Q lib')
os.system('rmdir /S /Q target')
os.system('rmdir /S /Q src')


print \
"""
======================================================
=====   start auto binary BSP generation (abg)   =====
=====                                            =====
=====   Done!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   =====
======================================================

"""

"""		
os.system('del '+DIRTMPFILE)
"""
"""
for nn in getList:
#	time.sleep(0.5)
	print nn
"""
