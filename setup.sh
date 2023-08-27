#!/bin/bash
# run make, move binary to /usr/local/bin so can use the command with bin name

make -f Makefile

mv build/pastimer /usr/local/bin