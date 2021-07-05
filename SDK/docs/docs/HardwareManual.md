## Overview of the OpenNCC Hardware  
OpenNCC is an open source programmable AI camera equipped with Intel Movidius Myriad X Visual Processing Unit (VPU). It is a low-power system-on-chip (SoC) used for deep learning and other artificial intelligence vision application acceleration for drones, smart cameras, VR/AR and other devices. Under the same power consumption conditions, the performance of the deep neural network (DNN) provided by Myriad X is ten times that of Myriad 2. Based on the theoretical 4+ TOPS computing power of Myriad X, the DNN peak throughput of more than one trillion times per second (TOPS) is reached.

    OpenNCC is composed of SENSOR board and CORE board, equipped with 2MP pixel sensor. OpenNCC CORE board uses the CSI_MIPI_RX 4lane interface to exchange data with the CORE board. The maximum sensor resolution is 20M@30fps, and it also supports 3D modules and infrared modules. CORE board output interface USB2.0/3.0, equipped with 4G/8G/16G LPDDR4, 16M SPI FLASH.。
![Figure-1](/openncc/docimg/zh/HardwareF1.png)  

## Hardware Spec.
* Lens  
![Figure-2](/openncc/docimg/zh/HardwareF2.jpg)  

* Camera module board CM2
sensor：SC2232H  
resolution：1920*1080（2MP）or 4K
Frame rate：30fps    
Sensor size：1/2.9  
![Figure-3](/openncc/docimg/zh/HardwareF3.jpg)  

* SoM
Size：38mm*38mm
![Figure-4](/openncc/docimg/zh/HardwareF4.jpg)  

* interface define
USB：TYPE C， Image orientation insertion supports USB3.0, and is recognized as USB2.0 when reverse inserting
![Figure-5](/openncc/docimg/zh/HardwareF5.jpg)  

J3 FPC interface definition:
| ID | PIN  | describe                | Electrical characteristics |
| ---- | ------------- | --------------------- | ----------- |
| 1    | VDD_5V        | 5V Power output        | Output current≤1A |
| 2    | VDD_5V        | 5V Power output         | 2           |
| 3    | VDD_5V        | 5V Power output         | 3           |
| 4    | GND           | Reference ground             | /           |
| 5    | GND           | Reference ground             | /           |
| 6    | GND           | Reference ground             | /           |
| 7    | CAM_A_AUX     | GPIO            | 1.8V        |
| 8    | CAM_A_RST     | GPIO/Reset     | 1.8V        |
| 9    | I2C1_SCL      | I2C CLK          | 1.8V        |
| 10   | COM_IO1       | GPIO            | 1.8V        |
| 11   | CAM_A_CLK     | GPIO/CLK     | 1.8V        |
| 12   | CAM_B_AUX     | GPIO/Power down     | 1.8V        |
| 13   | COM_IO2       | GPIO            | 1.8V        |
| 14   | CAM_A_PWM/RST | GPIO            | 1.8V        |
| 15   | I2C1_SDA      | I2C Data          | 1.8V        |
| 16   | GND           | Reference ground              | /           |
| 17   | CAM_A_D1_P    | MIPI Data-1 P | /           |
| 18   | CAM_A_D1_N    | MIPI Data-1 N | /           |
| 19   | GND           | Reference ground             | /           |
| 20   | CAM_A_D0_P    | MIPI Data-0 P | /           |
| 21   | CAM_A_D0_N    | MIPI Data-0 N | /           |
| 22   | GND           | Reference ground             | /           |
| 23   | CAM_A_L_C_P   | MIPI CLK P | /           |
| 24   | CAM_A_L_C_N   | MIPI CLK N | /           |
| 25   | GND           | Reference ground             | /           |
| 26   | CAM_A_D2_P    | MIPI Data-2 P | /           |
| 27   | CAM_A_D2_N    | MIPI Data-2 N | /           |
| 28   | GND           | Reference ground             | /           |
| 29   | CAM_A_D3_P    | MIPI Data-3 P | /           |
| 30   | CAM_A_D3_N    | MIPI Data-3 N | /           |



* Appendix I:  

| Item | openncc|
| -------------------------- | --------------------------------------------------------- |
| AI | AI |
|Computing power | maximum 4tflops|
|Support models | all openvino models|
|Support framework | onnx, tensorflow, Caffe, mxnet, Kaldi|
|Software | software|
|Image signal processing| √ |
|Open source kits | camera development kit openncc CDK, development technical documents, configuration tool openncc view|
|Supported development language | C / C + + / Python|
|SDK support function | 1. Get video stream |
| | 2. AI model download and replacement |
| | 3. Obtain the model operation results |
| | 4. Camera photographing, resetting, etc |
|OpenView function | configure camera parameters, configure camera local AI model|
|Openncc CDK supported operating systems | Linux and windows|
|OpenView supported operating system | Linux / windows |
|Hardware | hardware|
|Dimensions | 38 mm x 38 mm x 45 mm|
|Weight | camera net weight 31G|
| VPU | Intel Movidius Myriad X MV2085 |
|Memory | 4GB|
|Data interface | USB type-C 2.0 / 3.0|
|Power supply | 5V / 2A|
|Camera module | 2MP module|
|Resolution | 1920 x 1080|
|Frame rate | 30HZ |
|Horizontal field angle | 50°     |
