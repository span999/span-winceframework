#!/bin/bash
# add all new to svn

###svn st | grep '^\?' | gawk '{ print $2 }' | svn add --targets -

svn add `svn st .|grep "^?"|awk '{print $2}'`

