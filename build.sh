#!/bin/sh
# motionutil build script
# (c) 2017 0xbmuessig

echo "Cleaning up target..."
rm -f mutil*.elf
echo "Building dynamic ARM executable..."
arm-none-linux-gnueabi-gcc mutil.c -o mutil.elf
echo "Building static ARM executable..."
arm-none-linux-gnueabi-gcc -static mutil.c -o mutil.stat.elf
echo "Stripping static ARM executable..."
arm-none-linux-gnueabi-strip --strip-unneeded -o mutil.smin.elf mutil.stat.elf
echo "Building native executable..."
gcc mutil.c -o mutil.i686.elf
echo "Done."

