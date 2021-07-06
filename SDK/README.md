# OpenNCC SDK Release Note

---



## R21.07.01

Rebuild SDK：

1. Add script to help user set up environment.

2. Add new document.

OpenNCC Firmware MV02.101.21

1. Add SC200AI module



OpenNCC Viewer NV02.101.80

1. Fix video stream block in mode of calling second level model 



## R21.04.01

1. Fixed h.265 decode issue of AR0234 module on PC.

2. Added video-saving function to OpenNCC Viewer on PC.( MJGEG and h.264/h.265 format）

   

## R21.02.02

1. add Manual exposure in opennccview

2. add vehicle License plate recognition of python3 in Public\Library\For_Python

3. add face recognize of python3 in Public\Library\For_Python

**OpenNCC SDK SV02.103.04**

no change

**OpenNCC Viewer NV02.101.15**

1. add Manual exposure

**OpenNCC Firmware MV02.101.14**

no change

Known issues:

1: View for windows will collapsed when switch ai mode many times.

2:AR0234 sensor exposed issue.



## R21.02.01

1.  and show in nccview

2. add fps show in nccview

**OpenNCC SDK SV02.103.04**

no change

**OpenNCC Viewer NV02.101.14**

1. add fps show

2. add h265 show

**OpenNCC Firmware MV02.101.14**

1. fix some AP0234 bug

Known issues:

1. View for windows will collapsed when switch ai mode many times.

2. AR0234 sensor exposed issue.



## R20.12.02

1. Modified usb pid from F63B to F63D

2. Resolve losted frame bug in firmware

3. Support [Alwaysai](https://www.alwaysai.co/),and add a howto demo

**OpenNCC SDK SV02.103.04**

1. modify usb PID from 0xF63B to 0xF63D

**OpenNCC Viewer NV02.101.31:**

update sdk.

**OpenNCC Firmware MV02.101.05:**

1. modify firmware usb pid from F63B to F63D

2. resolve losted frame bug in firmware

Known issues:

1.  View for windows will collapsed when switch ai mode many times.

2. AR0234 sensor exposed issue.

3. It will been crashed just used ai mode “face-detection-retail-0004-fp16” when used AR0234 sensor.



## R20.12.01

1. Updated to OpenVINO R20.03
2. Support Raspberry Pi 3/4

**OpenNCC SDK V02.103.3:**

1. use the blob from openvino2020.3.194

2. test all examples by main()

3. add query firmware version

4. add query device id

5. add sdk_init_ex for add param field inferenceACC

**OpenNCC Viewer V1.3.0:**

1. Add show FPS and add Accelerating inference

**OpenNCC Firmware MV02.101.03:**

1. Support AR0234 sensor @60fps
2. Support OpenVINO R20.03

Known issues:

1. View for windows will collapsed when switch AI mode many times.
2. AR0234 exposure issue



## R20.11.1


**OpenNCC SDK V3.0.2:**

- updated documents

**OpenNCC Viewer V1.2.4:**

- Add OpenNCC Viewer for windows
- Fixed switch to secondary model coursed crash
- Add log system
- Download link:
  https://pan.baidu.com/s/1S8viRWDMpIccuaJ_YNxiYQ
  Code：e11e

**OpenNCC Firmware:**

- Fixed some bugs
- Add version control
