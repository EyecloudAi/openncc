##  Introduction

&emsp;OpenNCC is an open-source programmable AI camera. It is powered by the Intel Movidius Myriad X Vision Processing Unit (VPU). It is a low-power System-on-Chip (SoC). It can be used for drones, smart cameras, VR/AR headsets and other devices for deep learning and other AI vision application acceleration. Myriad X will deliver ten times the deep neural network (DNN) performance compared to Myriad 2 at the same power consumption. Myriad X reaches over trillion times per second (TOPS) peak DNN throughput based on a theoretical computational power of 4+ TOPS.

​		OpenNCC is composed of a SENSOR board and a CORE board, while it is equipped with a 2 or 8 megapixel sensor. The OpenNCC core board uses CSI_MIPI_RX 4lane interface. It supports sensor resolution up to 20M@30fps, and also supports 3D module and IR module. CORE board output interface is USB2.0/3.0, can be equipped with 4G/8G/16G LPDDR4, 16M SPI FLASH.

  <img src="/openncc/docimg/ch/HardwareF_1.png" alt="Figure-1" style="zoom:80%;" />

## Functions

* **Plug & Play**

  OpenNCC uses the USB-C data interface, just plug it into your computer and start developing, and it's as short as 30s to create your own versatile AI camera.
  
* **Openvino Standard Model Ready-to-Use**

  It has the advantage of the Intel VPU chip included in OpenNCC, which is compatible with the OpenVINO model.

* **Support for multiple deep learning frameworks**

   It supports Caffe, ONNX, TensorFlow, MX Net and other deep learning frameworks for user-friendly development and use.
* **High-quality image output**

   It has been factory video debugging, can support 1920x1080 or 4K resolution picture quality, as well as support YUV420, H.264, MJPEG and other video formats output.
* **Support secondary development**

   OpenNCC provides dedicated OpenNCC SDK development kit and related technical documentation, which supports C/C++/Python language, users can easily call the relevant API interface to realize camera parameter setting, model download, output video parameter setting, and quickly realize algorithm deployment of smart cameras. openNCC models support official models provided by openvino, and also support the deployment of custom algorithm models for rapid productization.

   

## OpenNCC Series

&emsp;According to different application scenarios, OpenNCC has launched different product series, among which there are mainly OpenNCC Lite series for developers, OpenNCC USB and OpenNCC IPC series for industrial application scenarios.

### OpenNCC Lite Series

- Introduction

  &emsp;OpenNCC Lite is mainly for developers, using USB interface, plug-and-play, convenient for developers to carry out rapid deployment of algorithm models.

  ![Figure-1](/openncc/docimg/ch/OpenNCC_Lite2.png)

  &emsp;OpenNCC Lite has built-in Intel Movidius Myriad X VPU with powerful arithmetic power to meet the computing needs of embedded AI. It is suitable for AI algorithm engineers, system integration engineers, and product managers for program pre-research, and for students, teachers, and researchers in the AI direction for research and study.

- Technical Specification

  ![image-20210702115956011](/openncc/docimg/OpenLiteguige.png)

### OpenNCC USB

- Introduction

- ![image-20210702115956011](/openncc/docimg/ch/OpenNccUsb.png)

  &emsp;OpenNCC USB is a series of endpoint AI cameras for industrial application scenarios with a plug-and-play USB interface to facilitate field deployment of commercial AI vision solutions by deep learning vision system developers.

   

  &emsp;OpenNCC USB series cameras are developed based on the Intel Movidius Myriad X VPU and support the Intel® OpenVINO toolbox.

- Technical Specification

  ![image-20210702133049458](/openncc/docimg/OpenUSBguige.png)

