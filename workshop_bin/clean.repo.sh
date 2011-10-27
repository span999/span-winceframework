#!/bin/bash

echo $PATH
echo "===== find .repo folder and delete it ====="
echo "===== START ====="
###find ./ -name \.git -exec ls -l {} \;
###find ./ -name \.git -exec rm -ir {} \;
find ./ -name \.repo -exec rm -rf {} \;
echo "===== END ====="

