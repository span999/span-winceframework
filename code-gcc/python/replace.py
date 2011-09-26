#!/usr/bin/python
# -*- coding: utf-8 -*-

import datetime
import os
import time
import string

REPLACETARGETFILE = 'platform.bib'
REPLACETEMPFILE = 'platform.tmp'
DLLFILELIST = 'dlllist.txt'
INSERTKEY = '\\insertfoldername\\'

getList = [ '<<start>>' ]


""" write line to tail of file """
def WritelineToFile( line ):
	writeto = open(REPLACETEMPFILE,'a+')
	writeto.seek(0, os.SEEK_END)
	writeto.write(line)
	writeto.newlines
	writeto.flush()
	writeto.close()

""" handle line with replacement """
def DoLineparce( line ):
#	print line, len(line)
	for dllname in getList:
#		print dllname
		print 'looking for ' + dllname + ' in (' + line + ')'
#		print string.find(line,dllname)
#		if dllname in line:
#		if -1 < string.find(line,dllname):
		if 0 < line.count(dllname):
			print 'hit '+dllname
			WritelineToFile('replace '+dllname+' in '+line)
#		else:
#			print dllname
#			WritelineToFile(line)
#	WritelineToFile(line)


""" handle line without replacement """	
def nDoLineparce( line ):
#	print 'ignore >> ' + line, len(line)
	WritelineToFile('ignore >> '+line)


	
	
""" 
get dll list from file 
getList = [ '<<start>>' ]
"""

with open(DLLFILELIST) as fileread:
	for readline in fileread:
#		print readline + '=> ', len(readline)
		getList.append(readline)

getList.append('<<end>>')

#for nn in getList:
#	time.sleep(0.5)
#	print nn



	
"""	
DIRTMPFILE = 'dir.list'
os.system('dir /O /B > '+DIRTMPFILE)
"""

os.system('del '+REPLACETEMPFILE)

with open(REPLACETARGETFILE) as openedfile:
	for readline in openedfile:
		if \
			readline[0] != '#' and \
			readline[0] != 'I' and \
			readline[0] != 'E' and \
			readline[0] != ';' \
			:
#			print readline, len(readline)
			DoLineparce( readline )
		else:
#			print 'ignore >> ' + readline, len(readline)
			nDoLineparce( readline )
			




"""
getList = [ '<<start>>' ]

with open(DIRTMPFILE) as dirfile:
	for readline in dirfile:
		print len(readline)
		if len(readline) > 1:
			getList.append(readline)
		print readline

getList.append('<<end>>')
"""

"""		
os.system('del '+DIRTMPFILE)
"""
"""
for nn in getList:
#	time.sleep(0.5)
	print nn
"""