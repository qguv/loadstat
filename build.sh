#!/bin/sh
rm -f loadstat
gcc -Wall --std=gnu99 -O3 -o loadstat loadstat.c
