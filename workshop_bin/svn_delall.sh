#!/bin/bash
# add all new to svn

svn st | grep '^\!' | gawk '{ print $2 }' | svn rm --targets -

