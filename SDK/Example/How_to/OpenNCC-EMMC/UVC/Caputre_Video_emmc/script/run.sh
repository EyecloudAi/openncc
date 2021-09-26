#!/bin/bash
cd bin/
echo -n "Please input video number (/dev/video*):"
read num
./Capturevideo /dev/video$num
