all include  sdk library and test example(how_to_use_sdk) .for windows we need a usb driver wich
are in directory usb_driver(wndows)

1.0.0
usb vsc  control/bulk transfer with movidus2480

1.0.1
1: fix can't enter boot loader status when sdk_init() failed
2: add read ai input width/height by xml
3: add get sdk version api

2.0.0
1:add python version
2:add get_meta_size() to get meta size for one ai mode

2.0.1
1: add android library which can been used in NDK env

2.0.2
1: add 3d support

3.0.0
1: add 2 ai mode interface
2: add 1 ai mode 1 input

3.0.1
1:update new firmware (add efuse)

SV02.103.03 2020.12.4
1:use the blob from openvino2020.3.194
2:test all examples  by  main()
3:add query firmware versin
4:add query device id
5:add sdk_init_ex for add param field inferenceACC

SV02.103.04  2020.12.14
1:modify usb PID from 0xF63B to 0xF63D




