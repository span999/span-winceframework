#!/usr/bin/python
# do the svn work


import os
import sys
import StringIO


TMPFILE = '../_temp_.tmp'
RMLIST = []
ADDLIST = []
MDFLIST = []
NOLIST = []


""" ======================================================"""
def Getsvnstatus():

#	tmpFile = StringIO.StringIO()
#	tmpFile.write('frist line.\n')

#	sys.stdout = tmpFile

	if os.path.isfile(TMPFILE):
		os.system('rm '+TMPFILE)
	
	os.system('svn status >> '+TMPFILE)

#	tmpList = tmpFile.getvalue()

#	sys.stdout = sys.__stdout__

	with open(TMPFILE) as openedfile:
	    for line in openedfile:
			if '!' == line[0]:
				RMLIST.append(line)
			elif '?' == line[0]:
				ADDLIST.append(line)
			elif 'M' == line[0]:
				MDFLIST.append(line)
			else:
				NOLIST.append(line)
#			print line

	for item in RMLIST:	
		print item

	for item in ADDLIST:	
		print item

	for item in MDFLIST:
		print item

	for item in NOLIST:
		print item
	
	os.system('rm '+TMPFILE)
#	tmpFile.close()
""" ======================================================"""







"""
main()
"""

Getsvnstatus()






