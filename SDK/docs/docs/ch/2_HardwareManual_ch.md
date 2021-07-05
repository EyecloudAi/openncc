## 硬件规格

* 定焦镜头 6mm  
![Figure-2](/openncc/docimg/ch/HardwareF2.jpg)
* 变焦镜头 2.8-12mm

![image-20210702134620748](/openncc/docimg/ch/2.8_12mm.png)

![img](/openncc/docimg/ch/2.8_12mm_1.png)

![img](/openncc/docimg/ch/2.8_12mm_2.png)

* 模组板 SC2232H（已停产）

  基本参数：

  ![image-20210702135300976](/openncc/docimg/ch/SC2232H.png)

  结构尺寸：

  ![image-20210702135353193](/openncc/docimg/ch/SC2232H_1.png)

* 模组版 SC8238

  基本参数：

  ![image-20210702135838265](/openncc/docimg/ch/SC8238_1.png)

  结构尺寸

  ![image-20210702135920924](/openncc/docimg/ch/SC8238_2.png)

* 模组板 AR0234

  基本参数：

  ![image-20210702140012580](/openncc/docimg/ch/AR0234.png)

  结构尺寸

  ![image-20210702140048596](/openncc/docimg/ch/AR0234_1.png)

* 模组板 SC200AI

  基本参数：

  ![image-20210702140140582](/openncc/docimg/ch/SC200AI_1.png)

  结构尺寸：

  ![image-20210702140213519](/openncc/docimg/ch/SC200AI_2.png)

* 核心板SoM

  ![image-20210702135539661](/openncc/docimg/ch/SoM.png)

* 结构尺寸：38mm*38mm（标准38板，空间间距34mm）  
  ![Figure-4](/openncc/docimg/ch/HardwareF4.jpg)  

* 接口定义
  USB接口：TYPE C， 图片方向支持USB3.0，当反向插入时识别为USB2.0 

  ![image-20210702135640130](/openncc/docimg/ch/HardwareF5_1.jpg)  

FPC座J3接口定义:   
| 序号 | 管脚定义  | 描述                | 电气特性 |
| ---- | ------------- | --------------------- | ----------- |
| 1    | VDD_5V        | 5V电源输出        | 输出电流≤1A |
| 2    | VDD_5V        | 5V电源输出        | 2           |
| 3    | VDD_5V        | 5V电源输出        | 3           |
| 4    | GND           | 参考地             | /           |
| 5    | GND           | 参考地             | /           |
| 6    | GND           | 参考地             | /           |
| 7    | CAM_A_AUX     | 通用GPIO            | 1.8V        |
| 8    | CAM_A_RST     | 通用GPIO/复位     | 1.8V        |
| 9    | I2C1_SCL      | I2C时钟线          | 1.8V        |
| 10   | COM_IO1       | 通用GPIO            | 1.8V        |
| 11   | CAM_A_CLK     | 通用GPIO/时钟     | 1.8V        |
| 12   | CAM_B_AUX     | 通用GPIO/断电     | 1.8V        |
| 13   | COM_IO2       | 通用GPIO            | 1.8V        |
| 14   | CAM_A_PWM/RST | 通用GPIO            | 1.8V        |
| 15   | I2C1_SDA      | I2C数据线          | 1.8V        |
| 16   | GND           | 参考地             | /           |
| 17   | CAM_A_D1_P    | MIPI数据对1 差分信号+ | /           |
| 18   | CAM_A_D1_N    | MIPI数据对1 差分信号- | /           |
| 19   | GND           | 参考地             | /           |
| 20   | CAM_A_D0_P    | MIPI数据对0 差分信号+ | /           |
| 21   | CAM_A_D0_N    | MIPI数据对0 差分信号- | /           |
| 22   | GND           | 参考地             | /           |
| 23   | CAM_A_L_C_P   | MIPI时钟线 差分信号+ | /           |
| 24   | CAM_A_L_C_N   | MIPI时钟线 差分信号- | /           |
| 25   | GND           | 参考地             | /           |
| 26   | CAM_A_D2_P    | MIPI数据对2 差分信号+ | /           |
| 27   | CAM_A_D2_N    | MIPI数据对2 差分信号- | /           |
| 28   | GND           | 参考地             | /           |
| 29   | CAM_A_D3_P    | MIPI数据对3 差分信号+ | /           |
| 30   | CAM_A_D3_N    | MIPI数据对3 差分信号- | /           |

## 开箱OpenNCC

&emsp;包装盒的贴纸提供了OpenNCC的官网地址（[www.openncc.com.cn](http://www.openncc.com.cn)），可供下载相关的技术文档和开发包。

![Figure-6](/openncc/docimg/ch/OpenNCC_1.png)

&emsp;打开包装盒，可以看到OpenNCC和标配的USB TPYE-C接口数据线。

![Figure-6](/openncc/docimg/ch/OpenNCC_2.png)

## 启动硬件

&emsp;使用OpenNCC官方配备的USB数据线分别连接OpenNCC与电脑<font color="red">USB 3.0</font>接口。如下图所示：

![Figure-6](/openncc/docimg/ch/OpenNCC_3.png)

------

提示：如何区分电脑USB接口。

![image-20210702133855768](/openncc/docimg/ch/USB2orUSB3.png)
