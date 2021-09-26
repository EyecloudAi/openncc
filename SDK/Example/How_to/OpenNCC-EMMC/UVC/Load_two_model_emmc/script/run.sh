#!/bin/bash
cd bin/
echo -n "Please input video number (/dev/video*):"
read num
./Load_two_model /dev/video$num
