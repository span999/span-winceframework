#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
module spWinToolHelps.py
"""


"""
import area
"""
import os
import string



"""
show version !!!
"""
def spWTH_Version():
    print 'spWinToolHelp(spWTH) ver 0.0.1'
    return 'spWinToolHelp(spWTH) ver 0.0.1'
""" ====================================================== """


"""
compare single char with case ignored
"""
def spWTH_CompareCharNoCase( s1, s2 ):
    matched = False

    if \
        s1 == s2 or \
        s1.upper() == s2.upper() or \
        s1.lower() == s2.lower() \
    :
        matched = True
#    else:
#        matched =  False

#    if matched:
#        print s1+' '+s2+' is equal'
#    else:
#        print s1+' '+s2+' is not equal'

    return matched
""" ====================================================== """


""" remove last char in string """
""" ====================================================== """
def spWTH_RemoveStringTailChar( stringIn ):
    lastchar = stringIn[len(stringIn)-1]
    return stringIn.strip(lastchar)
""" ====================================================== """


"""
find pattern in string with case ignored
"""
def spWTH_FindPatternInStrNoCase( StringIn, PatternIn ):
    StringInLen = len(StringIn)
    PatternInLen = len(PatternIn)
    PatternMismatchedCnt = 0

#    print PatternInLen,':'+PatternIn+' & ',StringInLen,':'+StringIn

    if StringInLen < PatternInLen:
        print 'Invalid parameter!! ignored.'
        print '    '+PatternIn+' > '+StringIn+' !!'
        return False

    for sidx in range( 0, 1+StringInLen-1-PatternInLen+1 ):
        PatternMismatchedCnt = 0
        for pidx in range( 0, 1+PatternInLen-1 ):
            if spWTH_CompareCharNoCase( PatternIn[pidx], StringIn[sidx+pidx] ):
                PatternMismatchedCnt = PatternMismatchedCnt
            else:
                PatternMismatchedCnt = PatternMismatchedCnt + 1
        if PatternMismatchedCnt == 0:
            return True
#        else:
#            print 'try next char'

    return False
""" ====================================================== """


"""
find pattern in List with case ignored
"""
def spWTH_FindPatternInListNoCase( ListIn, PatternIn ):
#    print PatternInLen,':'+PatternIn+' & ',StringInLen,':'+ListIn
    PatternMatchedCnt = 0

    for item in ListIn:
        if spWTH_FindPatternInStrNoCase( item, PatternIn ):
            PatternMatchedCnt = PatternMatchedCnt + 1

    if PatternMatchedCnt == 0:
        return False
    else:
        return True
""" ====================================================== """


"""
check if folder pat is empty
"""
def spWTH_IsFolderEmpty( folder ):

    if folder == "":
        print "folder path please!!"
        return False
#    else:
#        print "check folder "+folder

#    print os.listdir( folder )
    if os.listdir( folder ) == []:
        return True
    else:
        return False
""" ====================================================== """


"""
find partten in file
"""
def spWTH_FindPatternInFile( filename, pattern ):

    with open(filename) as openedfiles:
        for readline in openedfiles:
            if spWTH_FindPatternInStrNoCase( readline, pattern ):
                print 'hit! found '+pattern+' in '+filename
                return True

    return False
""" ====================================================== """


"""
find parttens(in list) in file
"""
def spWTH_FindPatternsInFile( filename, patternList ):

    for item in patternList:
        if spWTH_FindPatternInFile( filename, item ):
            return True

    return False
""" ====================================================== """


""" get dir list in folder """
""" ====================================================== """
def spWTH_GetDirListinFolder( folder ):
    dirsList = []
    # pick a folder you have ...
#    folder = '.\\'

    for (path, dirs, files) in os.walk(folder):
        for name in dirs:
            dirname = os.path.join(path, name)
            print 'folder: '+dirname
            dirsList.append(dirname)

    for item in dirsList:
        print item


    return dirsList
""" ====================================================== """