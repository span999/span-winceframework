#!/usr/bin/python
# -*- coding: utf-8 -*-

import datetime
import os
import time


d = datetime.datetime(2010, 7, 4, 12, 15, 58)
'{:%Y-%m-%d %H:%M:%S}'.format(d)

if 1<0:
	print 'Hello world !!'
	print 'Hello world !!'
print 'Hello world !!'

string1 = 'string #1'
# string2 = u'¬Y¬q¤å¦r'
number1 = 199

print '{:%Y-%m-%d %H:%M:%S}'.format(d)
print """ string out """
print string1
print number1
number1 = number1 + 12
print string1, number1
# print string2;
os.system('dir')

StringList = [ 'sl1','sl2','sl3','sl4']

for n in [ 0,1,2,3]:
	print StringList[n]
	
for nn in StringList:
	print nn
	
DIRTMPFILE = 'dir.list'
os.system('dir /O /B > '+DIRTMPFILE)

getList = [ '<<start>>' ]

with open(DIRTMPFILE) as dirfile:
	for readline in dirfile:
		print len(readline)
		if len(readline) > 1:
			getList.append(readline)
		print readline

getList.append('<<end>>')
		
os.system('del '+DIRTMPFILE)
	
for nn in getList:
	time.sleep(0.5)
	print nn
	print nn
print nn