#!/bin/bash
cd bin/
echo -n "Please input video number (/dev/video*):"
read num
./Load_a_face_model /dev/video$num
