#!/bin/bash
echo "Please input number[0..3]"
echo "example"
echo "#0 support one ai net example [default] face blob"
echo "#1 two ai net vehicle_license_plate_detection_barrier"
echo "#2 one ai net and 2 input"
echo "#3 used call back to get data from ncc device"
echo -n "number:"
read Key
cd bin/
./OpenNCC $Key
