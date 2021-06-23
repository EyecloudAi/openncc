This directory include some library and bin file for RasBerry:
1: c/c++ library file libOpenNCC.a
2: python3 library file _openncc.so
3: moviUsbBoot

because  RasBerry used linux OS,so we can develop refer to how_to_use_sdk.

How to setup  envirment On RasBerry platform?
1: copy from  openncc/Public/Library/how_to_use_sdk to current 
RasBerry directory  openncc/Public/Library/Raspberry

2: replace library:
you need copy current RasBerry platform library file(in Raspberry/linux_c&c++) to how_to_use_sdk/lib

3:replace usbBoot exe:
you can find usb boot exe file in how_to_use_sdk\bin path,just move rasberry version moviUsbBoot to how_to_use_sdk/bin

4: compiled c++ example and run: 
just run "make" ,and got test program in bin path
1)cd bin
2)sudo ./OpenNCC

5:run python3 test example
1)copy  all files from Raspberry/python3  to bin path(how_to_use_sdk/bin)
2)run it with command :
sudo python3 hello_ncc.py
by the way,you can test hello_human.py too!
