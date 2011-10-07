#!/usr/bin/python
# remove .git / .svn / .repo folder

import os
#import shutil

ignoreFILE = [ '.git', '.svn', '.repo' ]



"""
main()
"""


for item in ignoreFILE:
	print item

PATH = '.'

for root, dirs, files in os.walk(PATH, topdown=False):
#	for name in files:
#		os.remove(os.path.join(root, name))
#		print os.path.join(root, name)
	for name in dirs:
#		os.rmdir(os.path.join(root, name))
		check = os.path.join(root, name)
#		if check.count( ignoreFILE[0] )>0: 
#			print check
		checksp = check.rsplit( '/', 1 )
#		print checksp
		if checksp[1] == ignoreFILE[0]:
#			print check.rsplit( '/', 1 )
#			print check
			os.system( 'rm -rf '+check )
		if checksp[1] == ignoreFILE[1]:
			os.system( 'rm -rf '+check )
		if checksp[1] == ignoreFILE[2]:
			os.system( 'rm -rf '+check )
