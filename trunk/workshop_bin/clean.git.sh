#!/bin/bash

echo $PATH
echo "===== find .git folder and delete it ====="
echo "===== START ====="
###find ./ -name \.git -exec ls -l {} \;
###find ./ -name \.git -exec rm -ir {} \;
find ./ -name \.git -exec rm -rf {} \;
echo "===== END ====="

