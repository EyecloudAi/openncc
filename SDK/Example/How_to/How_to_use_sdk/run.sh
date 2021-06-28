#!/bin/bash
echo "Please input number[0..3]"
echo "example"
echo "#0 support one ai net example [default]"
echo "#1 two ai net"
echo "#2 one ai net and 2 input"
echo "#3 used call back to get data from ncc device"
echo -n "number:"
read Key
cd bin/
./OpenNCC $Key
