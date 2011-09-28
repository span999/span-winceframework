#!/usr/bin/python
# -*- coding: utf-8 -*-

""" import area """
import datetime
import os
import time
import string
import re


""" declare """
REPLACETARGETFILE = 'platform.bib'
REPLACEBACKUPFILE = 'platform.abg'
REPLACETEMPFILE = 'platform.tmp'
DLLFILELIST = 'dlllist.txt'
DLLFILELISTB = 'dlllist2.txt'
DIRFILELIST = 'dir.tmp'
BIBFILELIST = 'biblist.txt'
INSERTKEY = '\\dllbinary\\'
DLLBINARYFOLDER = 'dllbinary'
ESCCODE = '----ESCCODE----'

getList = [ ESCCODE+'1' ]
getsList = [ ESCCODE+'2' ]
dirList = [ ESCCODE+'3' ]
bibList = [ ESCCODE+'4' ]
bibsList = [ ESCCODE+'5' ]


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

        dirList.append(ESCCODE)

    for item in dirList:
        print item

    os.system('del '+DIRFILELIST)

    if \
        dirList.count('files') > 0 and \
        dirList.count('lib') > 0 and \
        dirList.count('target') > 0 \
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


""" Match sentance in line """
""" ====================================================== """
def FindMatchPattern( lineString, matchPattern ):
#	print 'find <'+matchPattern+'> in <'+lineString+'>'

    if \
        re.search( matchPattern, lineString, re.IGNORECASE ) == None and \
        re.match( matchPattern, lineString, re.IGNORECASE ) == None and \
        1 > lineString.count( matchPattern ) \
    :
        return False
    else:
        return True
""" ====================================================== """


""" check if has escape code """
""" ====================================================== """
def HasESCcode( line ):

    if FindMatchPattern( line, ESCCODE ):
        return True
    else:
        return False
""" ====================================================== """

""" copy dll binary to files folder """
""" ====================================================== """
def CopyDllBinary():
#	os.system('del '+DLLFILELISTB)
    os.system('dir /S /B target\\*.dll > '+DLLFILELISTB)
    os.system('dir /S /B target\\*.exe >> '+DLLFILELISTB)
    os.system('dir /S /B target\\*.rel >> '+DLLFILELISTB)
    os.system('dir /S /B target\\*.map >> '+DLLFILELISTB)
    os.system('dir /S /B target\\*.pdb >> '+DLLFILELISTB)

    with open(DLLFILELISTB) as openedfile:
        for readline in openedfile:
#            print readline + '=> ', len(readline)
            lastchar = readline[len(readline)-1]
            stripedline = readline.strip(lastchar)
            getList.append(stripedline)

        getList.append(ESCCODE)

#	for item in getList:
#		print item

    os.system('del '+DLLFILELISTB)

    os.system('rmdir /S /Q files\\'+DLLBINARYFOLDER)
    os.system('mkdir files\\'+DLLBINARYFOLDER)

#   do the binary copy
    for item in getList:
        if False == HasESCcode( item ):
            print 'copy '+item+' to files\\'+DLLBINARYFOLDER
            os.system('copy /V '+item+' files\\'+DLLBINARYFOLDER+'\\')
            if \
                item.count('oal.exe') > 0 or \
                item.count('oal.map') > 0 or \
                item.count('oal.pdb') > 0 or \
                item.count('oal.rel') > 0 \
            :
                os.system('copy /V '+item+' files\\')

""" ====================================================== """


""" create list with all dll used """
""" ====================================================== """
def GetDllList():
    os.system('del '+DLLFILELIST)
    os.system('dir /B target\\ARMV4I\\retail\\*.dll > '+DLLFILELIST)
    os.system('dir /B target\\ARMV4I\\retail\\*.exe >> '+DLLFILELIST)

    with open(DLLFILELIST) as openedfiles:
        for readline in openedfiles:
#            print readline + '=> ', len(readline)
            lastchar = readline[len(readline)-1]
            stripedline = readline.strip(lastchar)
            getsList.append(stripedline)

    getsList.append(ESCCODE)

    print '>>>>>>>> Dll files list:'
    for item in getsList:
        print item

    os.system('del '+DLLFILELIST)
""" ====================================================== """


""" create list with all bib used """
""" ====================================================== """
def GetBIBList():
    os.system('del '+BIBFILELIST)
    os.system('dir /B /S *.bib > '+BIBFILELIST)

    with open(BIBFILELIST) as openedfiles:
        for readline in openedfiles:
#            print readline + '=> ', len(readline)
            lastchar = readline[len(readline)-1]
            stripedline = readline.strip(lastchar)
            bibList.append(stripedline)

    bibList.append(ESCCODE)

    print '>>>>>>>> bib files list:'
    for item in bibList:
        print item

    os.system('del '+BIBFILELIST)
""" ====================================================== """


""" find sentence in file """
""" ====================================================== """
def FindSentanceInFile( filename, sentanceList ):

    with open(filename) as openedfiles:
        for readline in openedfiles:
            for item in sentanceList:
                if False == HasESCcode( item ):
                    if True == FindMatchPattern( readline, item ):
                        print 'hit! found '+item+' in '+filename
                        return True

    return False
""" ====================================================== """


""" filtering list of bib files """
""" ====================================================== """
def FilterBIBList():

    for item in bibList:
        if False == HasESCcode( item ):
            if False == FindSentanceInFile( item, getsList ):
                bibList.remove( item )
            else:
                bibsList.append( item )

    bibsList.append( ESCCODE )
    print '>>>>>>>> after filter bib files list:'
    for item in bibsList:
        print item
""" ====================================================== """


""" check if BIB file escape code """
""" ====================================================== """
def IsBIBescapeCode( line ):
#	print line

	if line[0] == '#':
		return True
	if line[0] == ';':
		return True
	if line[0] == 'I' and line[1] == 'F':
		return True
	if line[0] == 'E' and line[1] == 'N' and line[2] == 'D' and line[3] == 'I' and line[4] == 'F':
		return True
	if line[0] == 'E' and line[1] == 'L' and line[2] == 'S' and line[3] == 'E':
		return True
	if line[0] == 'M' and line[1] == 'O' and line[2] == 'D' and line[3] == 'U' and line[4] == 'L':
		return True
	if line[0] == 'F' and line[1] == 'I' and line[2] == 'L' and line[3] == 'E' and line[4] == 'S':
		return True
	if len(line) < 2:
		return True

	return False
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

""" find dll name match """
""" ====================================================== """
def FindMatchDllName( lineIn, name ):
#	print 'find <'+name+'> in <'+lineIn+'>'
#	if re.match( name, lineIn, re.IGNORECASE ) == None:
#	if re.search( name, lineIn, re.IGNORECASE ) == None:
	if \
        re.search( name, lineIn, re.IGNORECASE ) == None and \
        re.match( name, lineIn, re.IGNORECASE ) == None and \
        1 > lineIn.count(name) \
    :
		return False
	else:
		return True
""" ====================================================== """



""" handle line with replacement """
""" ====================================================== """
def DoLineparce( line ):
#	print line, len(line)
    bHit = False
    for dllname in getsList:
        if False == HasESCcode( dllname ):
#		     print 'looking for ' + dllname + ' in (' + line + ')'
#		     if dllname in line:
#		     if -1 < string.find(line,dllname):
#		     if 0 < line.count(dllname):
#		     if 0 < line.count('\\'+dllname):
            if FindMatchDllName(line,'\\'+dllname):
                print 'hit "'+dllname+'" and replace it! '
                WritelineToFile(';abg; replace >> '+'"\\'+dllname+'" with "'+INSERTKEY+dllname+'"')
                WritelineToFile('\r')
                WritelineToFile(';abg; '+line)
                WritelineToFile(line.replace('\\'+dllname,INSERTKEY+dllname,2))
                bHit = True
#           else:
#               print dllname
#               WritelineToFile(line)

	if bHit == False:
		WritelineToFile(line)

""" ====================================================== """


""" handle line without replacement """
""" ====================================================== """
def nDoLineparce( line ):
#	print 'ignore >> ' + line, len(line)
#	WritelineToFile('ignore  >> '+line)
	WritelineToFile(line)
""" ====================================================== """


""" parse .bib file and do the replacemenet """
""" ====================================================== """
def ParseBIBfile( filename ):
    print '>>>>>>>> parse bib file: '+filename
#    os.system('del files\\'+REPLACETEMPFILE)
    os.system('del files\\'+REPLACETEMPFILE)

    with open(filename) as openedfile:
        for readline in openedfile:
            if IsBIBescapeCode( readline ):
                nDoLineparce( readline )
            else:
                DoLineparce( readline )

    print \
    """
    ======================================================
    ===== save original ????.bib to ????.abg =====
    ======================================================
    """

#   os.system('rename files\\platform.bib platform.abg')
#   os.system('rename files\\'+REPLACETEMPFILE+' platform.bib')
    if filename.count('bib') > 0:
        newfilename = string.replace(filename, '.bib', '.abg' , 1)
    if filename.count('BIB') > 0:
        newfilename = string.replace(filename, '.BIB', '.abg' , 1)

    os.system('copy '+filename+' '+newfilename)
    print '>>>>>>>> original '+filename+' -> '+newfilename
    os.system('copy files\\'+REPLACETEMPFILE+' '+filename)
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

""" retrieve dll list in simple format """
GetDllList()

""" retrieve bib list in simple format """
GetBIBList()

""" filter out bib files that won't change in list """
FilterBIBList()

for item in bibsList:
    if False == HasESCcode( item ):
        ParseBIBfile( item )



print \
"""
======================================================
=====   killing source & binary files  !!!!      =====
======================================================
"""

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
