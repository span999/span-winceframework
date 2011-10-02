#!/usr/bin/python
# -*- coding: utf-8 -*-

""" import area """
#import datetime
import os
#import time
import string
import re
#import win32api
#import win32con
import spWinToolHelps

# make a short cut of spWinToolHelps
spWTH = spWinToolHelps
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
srcbinList = [ ESCCODE+'6' ]


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


""" Match sentance in line """
""" ====================================================== """
"""
def FindMatchPattern( lineString, matchPattern ):
#    print 'Try to find <'+matchPattern+'> in <'+lineString+'>'

    if \
        re.search( matchPattern, lineString, re.IGNORECASE ) == None and \
        re.match( matchPattern, lineString, re.IGNORECASE ) == None and \
        1 > lineString.count( matchPattern ) \
    :
        return False
    else:
        return True
"""
""" ====================================================== """


""" check if has escape code """
""" ====================================================== """
def HasESCcode( line ):

#    if FindMatchPattern( line, ESCCODE ):
    if spWTH.spWTH_FindPatternInStrNoCase( line, ESCCODE ):
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

    print '.'
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

    print '.'
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
#                    if True == FindMatchPattern( readline, item ):
                    if spWTH.spWTH_FindPatternInStrNoCase( readline, item ):
                        print 'hit! found '+item+' in '+filename
                        return True

    return False
""" ====================================================== """


""" filtering list of bib files """
""" ====================================================== """
def FilterBIBList():

    for item in bibList:
        if False == HasESCcode( item ):
#            if False == FindSentanceInFile( item, getsList ):
            if False == spWTH.spWTH_FindPatternsInFile( item, getsList ):
                bibList.remove( item )
            else:
                bibsList.append( item )

    bibsList.append( ESCCODE )
    print '.'
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
"""
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
"""
""" ====================================================== """


""" compare char with case ignored """
""" ====================================================== """
def CompareCharInCase( s1, s2 ):
    if s1 == s2:
        return True
    if s1.upper() == s2.upper():
        return True
    if s1.lower() == s2.lower():
        return True

    return False
""" ====================================================== """

""" index of match pattern """
""" ====================================================== """
def GetIndexMatchPattern( lineString, MatchPattern ):
    rIndex = 9
    patternLen = len(MatchPattern)
    lineLen = len(lineString)
    matchKey = False

    for idx in range(0,1+lineLen-1-patternLen+1):
        if rIndex > 0:
            rIndex = 0
            for idx2 in range(0,1+patternLen-1):
                if CompareCharInCase(MatchPattern[idx2],lineString[idx+idx2]):
                    rIndex = rIndex
                else:
                    rIndex = rIndex + 1
        else:
#            print 'GetIndexMatchPattern=',idx
            return idx-1

    print 'GetIndexMatchPattern found NO matched '+MatchPattern+' !!!'
    print 'In '+lineString+' !!!'
    return -1
""" ====================================================== """


""" replace subkey in string with case ignored """
""" ====================================================== """
def ReplaceInSentance( linestring, oldKey, newKey ):
#    print 'ReplaceInSentance:'+linestring
#    print 'oldKey='+oldKey+', newKey='+newKey
    oldKeylen = len(oldKey)

    tmp = []
#   locate the position of oldKey, get the index
    oldKeyIndex = GetIndexMatchPattern(linestring, oldKey)

#   check if out of range
    if oldKeyIndex == -1:
        print 'ReplaceInSentance fail !!!'
        return linestring

#   copy start
    for idx in range(0,1+oldKeyIndex-1):
        tmp.append(linestring[idx])
#    print tmp

#   replace from the oldKey position with newKey
#    for idx2 in range(0,1+len(newKey)):
#        tmp.append(newKey[idx2])
    tmp.append(newKey)
#    print tmp

#   append tail of linestring
    for idx3 in range(oldKeyIndex+len(oldKey),1+len(linestring)-1):
        tmp.append(linestring[idx3])
#    print tmp

#   add newline
#    tmp.append('\r')
#    return tmp
    return ''.join(tmp)

""" ====================================================== """

""" handle line with replacement """
""" ====================================================== """
def DoLineparse( line ):
#    print 'DoLineparse:'+line, len(line)

    bHit = False

    for dllname in getsList:
        if False == HasESCcode( dllname ):
#            print 'looking for ' + dllname + ' in (' + line + ')'
#            if FindMatchPattern( line, '\\'+dllname ):
            if spWTH.spWTH_FindPatternInStrNoCase( line, '\\'+dllname ):
                print 'hit "'+dllname+'" and replace it! '
                WritelineToFile(';abg; replace >> '+'"\\'+dllname+'" with "'+INSERTKEY+dllname+'"')
                WritelineToFile('\r')
                WritelineToFile(';abg; '+line)
                replaced = ReplaceInSentance(line,'\\'+dllname,INSERTKEY+dllname)
#                WritelineToFile(line.replace('\\'+dllname,INSERTKEY+dllname,2))
                WritelineToFile(replaced)
                bHit = True
                return
#            else:
#                print dllname+' not found'
#                WritelineToFile(line)
#        else:
#            print 'found escape code in list '+dllname+' !!!'
#            print 'try next in list'

    if bHit == False:
		WritelineToFile(line)

""" ====================================================== """


""" handle line without replacement """
""" ====================================================== """
def nDoLineparse( line ):
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
                nDoLineparse( readline )
            else:
                DoLineparse( readline )

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
    print '.'
    print '>>>>>>>> original '+filename+' -> '+newfilename
    os.system('copy files\\'+REPLACETEMPFILE+' '+filename)
""" ====================================================== """


""" clean src folder (delete obj folder) """
""" ====================================================== """
def CleanSrcFolder():
    os.system('dir /S /B /AD src\\ > '+DIRFILELIST)

    with open(DIRFILELIST) as openedfile:
        for readline in openedfile:
#	    print readline + '=> ', len(readline)
#            if FindMatchPattern(readline,'\\obj'):
            if spWTH.spWTH_FindPatternInStrNoCase(readline,'\\obj'):
#                if False == FindMatchPattern(readline,'ARMV4I'):
                if False == spWTH.spWTH_FindPatternInStrNoCase(readline,'ARMV4I'):
#                    print 'delete folder '+readline
                    os.system('rmdir /S /Q '+readline)

    os.system('del '+DIRFILELIST)
""" ====================================================== """


""" delete files in src folder (delete files in src folder except reserved) """
""" ====================================================== """
def DeleteSrcFiles():
    os.system('dir /S /B src\\ > '+DIRFILELIST)
    reversedFile = False

    with open(DIRFILELIST) as openedfile:
        for readline in openedfile:
#	    print readline + '=> ', len(readline)
            reversedFile = False
            for item in srcbinList:
#                if FindMatchPattern(readline,item):
                if spWTH.spWTH_FindPatternInStrNoCase(readline,item):
                    print 'reserved file: '+readline
                    reversedFile = True
#                else:
#                    print 'not reserved file'

            if False == reversedFile:
#                print os.path.getsize(readline)
                print 'check file/path: '+readline
                if os.path.exists(readline):
#                if 4 > os.path.getsize(readline):
                    print 'this is a folder: '+readline
                else:
                    print 'delete file: '+readline
                    os.system('del /Q /F '+readline)

    os.system('del '+DIRFILELIST)

def DeleteSrcFilesExt():
    fileList = []

    reversedFile = False

#   get file/folder list in src\
    os.system('dir /S /B src\\ > '+DIRFILELIST)

    with open(DIRFILELIST) as openedfile:
        for readline in openedfile:
            fileList.append( readline )

    os.system('del '+DIRFILELIST)

#   get folder list in src\ and remove it from file/folder list
    os.system('dir /S /B /AD src\\ > '+DIRFILELIST)

    with open(DIRFILELIST) as openedfile:
        for readline in openedfile:
            fileList.remove(readline)

    os.system('del '+DIRFILELIST)

#   now we have files only list
#    for item in fileList:
#        print item

    for filename in fileList:
        reversedFile = False
        for rsvitem in srcbinList:
            if spWTH.spWTH_FindPatternInStrNoCase(filename,rsvitem):
#                print 'reserved file: '+filename+' for '+rsvitem
                reversedFile = True

        if reversedFile:
            print 'reserved file: '+filename
        else:
#            print 'delete file: '+filename
            os.system('del /Q /F '+filename)

""" ====================================================== """


"""  """
""" ====================================================== """
def IsReservedSrcFile( readline ):
    reversedFile = False

    for item in srcbinList:
        if False == HasESCcode(item):
#           if FindMatchPattern(readline,item):
            if spWTH.spWTH_FindPatternInStrNoCase(readline,item):
#               print 'reserved file: '+readline
                reversedFile = True
#           else:
#               print 'not reserved file'

    return reversedFile
""" ====================================================== """


""" delete files in src folder """
""" ====================================================== """
"""
def DeleteSrcFolders():

    # pick a folder you have ...
    folder = 'src\\'
    folder_size = 0

    for (path, dirs, files) in os.walk(folder):
        for item in files:
            filename = os.path.join(path, item)
#            print filename
            folder_size += os.path.getsize(filename)
            #check if it's reserved file
            if IsReservedSrcFile(filename):
                print 'reserved file: '+filename
            else:
#                print 'delete file: '+filename
                os.system('del /Q /F '+filename)

    print "Folder files total = %0.1f MB" % (folder_size/(1024*1024.0))
    print "os.path.getsize = %0.1f MB" % (os.path.getsize(folder)/(1024*1024.0))
"""
""" ====================================================== """



""" find all bib files item that reserved in src\ folder """
""" ====================================================== """
def FilterSrcForBIB():

    for filename in bibsList:
        if False == HasESCcode(filename):
            print 'open bib file: '+filename
            with open(filename) as openedfiles:
                for readline in openedfiles:
                    if False == IsBIBescapeCode(readline):
#                        if FindMatchPattern(readline,'\\src'):
                        if spWTH.spWTH_FindPatternInStrNoCase(readline,'\\src'):
                            splited = readline.rsplit('\\',1)
                            splited = splited[1].rsplit()
                            srcbinList.append(splited[0])


    srcbinList.append( '.bib' )
    srcbinList.append( '.reg' )
#    srcbinList.append( '.BIB' )
#    srcbinList.append( '.REG' )

    srcbinList.append( ESCCODE )
    print '.'
    print '>>>>>>>> after filter src\\ in bib files list:'
    for item in srcbinList:
        print item
""" ====================================================== """


""" delete empty folder in src """
""" ====================================================== """
def DeleteEmptyFolders():
    # pick a folder you have ...
    folder = 'src\\'
#    folder_size = 0

    for (path, dirs, files) in os.walk(folder, topdown=False):
        for name in dirs:
            dirname = os.path.join(path, name)
            print 'folder: '+dirname
            if spWTH.spWTH_IsFolderEmpty(dirname):
                os.rmdir( dirname )
                print 'delete folder: '+dirname
""" ====================================================== """






""" main start """
""" ====================================================== """

print \
"""
======================================================
=====      auto binary BSP generation (abg)      =====
=====                                 v 0.1      =====
=====                                            =====
=====   This tool helps to generate the binary   =====
=====   BSP from normal BSP (WinCE BSP)          =====
=====                                            =====
=====   press 'Y' to start, or 'N' to exit       =====
======================================================

"""
spWTH.spWTH_Version()


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

""" filter out bib files that won't change from list """
FilterBIBList()

""" parse each bib file. make necessary change """
for item in bibsList:
    if False == HasESCcode( item ):
        ParseBIBfile( item )

""" parse src\ in BIB files """
FilterSrcForBIB()


print \
"""
======================================================
=====   killing source & binary files  !!!!      =====
======================================================
"""

os.system('rmdir /S /Q lib')
os.system('rmdir /S /Q target')
#os.system('rmdir /S /Q src')
# clean src folder. remove all obj folder
CleanSrcFolder()
# delete files in src\ except reserved
#DeleteSrcFolder()
DeleteSrcFilesExt()
# delete empty folder under src\
DeleteEmptyFolders()

print \
"""
======================================================
=====      auto binary BSP generation (abg)      =====
=====                                            =====
=====   Done!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!   =====
======================================================

"""

