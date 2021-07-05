It is a demo on how to used the sdk in linux os(ubunto):
1: run envirment
in bin directory,you will find sub directory blob which included test ai files and fw
 which included firmvare for ncc device.

2: compile method:
 run make
and you will get a test program in bin directory

3: run method:
enter bin directory,and run
sudo ./OpenNCC [0..3]
example 
#0 support one ai net example [default]
#1 two ai net
#2 one ai net and 2 input
#3 used call back to get data from ncc device

notes:
we used opecv4.4 which are from openvino2020.3 version
if you used opencv3.x,just edit the makefile to select it
OPENCV = `pkg-config opencv --cflags --libs`
remove -I/usr/local/include/opencv4
