#!/usr/bin/env python
#
# please visit site below for installation
# http://wiki.wxpython.org/How%20to%20install%20wxPython
#


import os
import subprocess
import ftplib


print "hello!!"

# os.system("date")
# subprocess.call("date")

#f = os.popen('date')
#now = f.read()

p = subprocess.Popen("date", stdout=subprocess.PIPE, shell=True)
(now, err) = p.communicate()

print "today is", now
print now
print

ftp = ftplib.FTP('10.60.103.234', 'anonymous','')
pwd = ftp.pwd()
print "current path = ", pwd
ftp.cwd("upload")
print "ftp files list:"
filelist = ftp.dir()
print filelist
ftp.quit()
ftp.close()



