#!/bin/sh
# motionutil build script
# (c) 2017 0xbmuessig

rm -f mutil*.elf
arm-none-linux-gnueabi-gcc -static mutil.c -o mutil.elf
arm-none-linux-gnueabi-strip --strip-unneeded -o mutil.min.elf mutil.elf
