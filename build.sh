#!/bin/sh
rm -f mutil*.elf
arm-none-linux-gnueabi-gcc -static mutil.c -o mutil.elf
arm-none-linux-gnueabi-strip --strip-unneeded -o mutil.min.elf mutil.elf
