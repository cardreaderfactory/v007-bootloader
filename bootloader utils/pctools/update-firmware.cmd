@echo off
gcreate -c aesconfig.txt -f ..\..\v007\smth.hex -n -o smth.aes
update smth.aes -COM3 -250000
