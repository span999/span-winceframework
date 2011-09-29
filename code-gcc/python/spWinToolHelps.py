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
    if s1 == s2:
        return True
    if s1.upper() == s2.upper():
        return True
    if s1.lower() == s2.lower():
        return True

    return False
""" ====================================================== """


"""
find pattern in string with case ignored
"""
def spWTH_FindPatternInStrNoCase( StringIn, PatternIn ):
    StringInLen = len(StringIn)
    PatternInLen = len(PatternIn)
    PatternMismatchedCnt = 0

    print PatternInLen,':'+PatternIn+' & ',StringInLen,':'+StringIn

    if StringInLen < PatternInLen:
        print 'Parameter Error!!'
        print '    '+PatternIn+' > '+StringIn+' !!'
        return False

    for sidx in range( 0, StringInLen-1-PatternInLen+1 ):
        PatternMismatchedCnt = 0
        for pidx in range( 0, PatternInLen-1 ):
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
