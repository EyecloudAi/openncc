## OpenNCC 概述
&emsp; OpenNCC 是一款eyecloud.ai公司自主研发的开源的可编程AI 相机,搭载了Intel Movidius Myriad X 视觉处理单元（VPU），它是一种低功耗片上系统（SoC），用于无人机、智能相机、VR/AR 头盔等设备的深度学习及其他人工智能视觉应用加速。Myriad X 将会在同样功耗条件下提供 Myriad 2 十倍的深度神经网络（DNN）性能。Myriad X 理论计算能力 4+ TOPS 的基础上达到超过每秒万亿次（TOPS）的 DNN 峰值吞吐量。  
&emsp; OpenNCC 是由SENSOR板和CORE板组成，搭载了200万或800万像素的传感器。 同时OpenNCC 核心板采用CSI_MIPI_RX 4lane 接口，与CORE板进行数据交互传输，最大支持sensor分辨率为20M@30fps，同时也支持3D模组、红外模组。CORE 板输出接口USB2.0/3.0，可搭载4G/8G/16G LPDDR4, 16M SPI FLASH。 

  <img src="/openncc/docimg/ch/HardwareF_1.png" alt="Figure-1" style="zoom:80%;" />

## OpenNCC 功能介绍  
* 即插即用  
最短30s轻松打造属于自己的百变AI相机，OpenNCC DK使用USB-C数据接口，只要将其接入电脑就可开始开发 。
* Openvino标准模型即换即用  
得益于OpenNCC 内含的Intel VPU芯片，兼容OpenVINO模型。
* 支持多种深度学习框架  
支持Caffe,ONNX,TensorFlow,MXNet等多种深度学习框架 ，方便用户开发。
* 优质图像输出  
支持1920x1080或4K分辨率画质，图像完成出厂调优，支持YUV420，H.264,MJPEG多种视频格式输出  。
* 支持二次开发  
OpenNCC DK提供专用OpenNCC CDK开发包，和相关技术文档，支持C/C++/Python，用户可方便调用相关API接口，实现相机参数设置，模型下载，输出视频参数设置，快速实现智能相机的算法部署。OpenNCC模型支持openvino提供的官方模型，同时也支持用于自定义算法模型的部署，快速实现产品化。   

## OpenNCC系列产品

​		&emsp;根据不同的应用场景，OpenNCC推出了不同的产品系列，其中主要有针对开发者的OpenNCC Light系列，针对工业应用场景的OpenNCC USB和OpenNCC IPC系列。

### OpenNCC Lite系列

 



* 简介：

  &emsp;OpenNCC Lite主要面向开发者，采用USB接口，即插即用，方便开发者进行快速部署算法模型。

  &emsp;OpenNCC Lite内置了英特尔Movidius Myriad X VPU，拥有强大算力，满足嵌入式AI的运算需求，适合人工智能算法工程师、系统集成工程师、产品经理等进行方案预研，也适合人工智能方向的学生、老师、研究员进行研究学习。

  ![Figure-1](/openncc/docimg/ch/OpenNCC_Lite2.png)

* 技术规格

  ![image-20210702115956011](/openncc/docimg/ch/OpenLiteguige.png)

### OpenNCC USB

* 简介

![image-20210702115956011](/openncc/docimg/ch/OpenNccUsb.png)

&emsp;OpenNCC USB是针对工业应用场景的终端AI摄像头系列，采用USB接口即插即用，方便深度学习视觉系统开发人员进行商业化AI视觉解决方案的现场部署。

&emsp;OpenNCC USB系列摄像头基于Intel Movidius Myriad X VPU开发，支持的Intel® OpenVINO的工具箱。

* 技术规格

![image-20210702133049458](/openncc/docimg/ch/OpenUSBguige.png)
