## Hardware specifications

* Fixed focus lenses 6mm  

  ![Figure-2](/openncc/docimg/ch/HardwareF2.jpg)

* Zoom lenses 2.8-12mm

<img src="/openncc/docimg/ch/2.8_12mm.png" alt="image-20210702134620748" style="zoom:100%;" /><img src="/openncc/docimg/ch/2.8_12mm_1.png" alt="img" style="zoom:100%;" />

![img](/openncc/docimg/ch/2.8_12mm_2.png)

* Module Board SC2232H（Discontinued）

  Basic parameters：

  ![image-20210702135300976](/openncc/docimg/ch/SC2232H.png)

  Structure size：

  ![image-20210702135353193](/openncc/docimg/ch/SC2232H_1.png)

* Module Board SC8238

  Basic parameters：

  ![image-20210702135838265](/openncc/docimg/ch/SC8238_1.png)

  Structure size：

  ![image-20210702135920924](/openncc/docimg/ch/SC8238_2.png)

* Module Board AR0234

  Basic parameters：

  ![image-20210702140012580](/openncc/docimg/ch/AR0234.png)

  Structure size：

  ![image-20210702140048596](/openncc/docimg/ch/AR0234_1.png)

* Module Board SC200AI

  Basic parameters：

  ![image-20210702140140582](/openncc/docimg/ch/SC200AI_1.png)

  Structure size：

  ![image-20210702140213519](/openncc/docimg/ch/SC200AI_2.png)

* Core Board SoM

  ![image-20210702135539661](/openncc/docimg/ch/SoM.png)

* Structure size：38mm*38mm（Standard 38 board, space spacing 34mm）  
  ![Figure-4](/openncc/docimg/ch/HardwareF4.jpg)  

* Interface Definition
  USB interface: TYPE C (the direction of the picture supports USB3.0, when the reverse is inserted to identify as USB2.0)

  ![image-20210702135640130](/openncc/docimg/ch/HardwareF5_1.jpg)  

FPC座J3接口定义:   

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

## Unboxing show


​		The sticker on the box provides the official website address of OpenNCC ([www.openncc.com](www.openncc.com)) for downloading relevant technical documentation and development kits.

![Figure-6](/openncc/docimg/OpenNCC_1.png)

When you open the box, you can see OpenNCC and the standard USB TPYE-C interface data cable.

![Figure-6](/openncc/docimg/OpenNCC_2.png)

## Start-up hardware

​		Use the official USB cable provided by OpenNCC to connect the computer to the OpenNCC's USB 3.0 port. As shown in the figure below.

![Figure-6](/openncc/docimg/ch/OpenNCC_3.png)

---

Tip: How to distinguish between computer USB ports.

![image-20210702133855768](/openncc/docimg/ch/USB2orUSB3.png)
