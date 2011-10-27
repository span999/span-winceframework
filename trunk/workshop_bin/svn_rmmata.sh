#!/bin/bash
# remove all .svn folder

find ./ -type d -name \.svn -exec rm -rf {} \;
