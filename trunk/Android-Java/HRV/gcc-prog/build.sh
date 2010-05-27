#!/bin/bash
# build code

gcc -o hello c-main.c c-string.c c-file.c c-buffer.c -lpthread
