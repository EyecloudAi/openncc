Supported Platforms:

    Linux,windows

Supported Architecture:

    i386 / i586 / i686 / x86_64
    armv6l / armv7l / aarch64 / armv8

 &ensp; OpenNCC is an open source programmable AI camera developed by eyecloud.ai equipped with Intel Movidius Myriad X Visual Processing Unit (VPU). It is a low-power system-on-chip (SoC) used for deep learning and other artificial intelligence vision application acceleration for drones, smart cameras, VR/AR and other devices. Under the same power consumption conditions, the performance of the deep neural network (DNN) provided by Myriad X is ten times that of Myriad 2. Based on the theoretical 4+ TOPS computing power of Myriad X, the DNN peak throughput of more than one trillion times per second (TOPS) is reached.

  &ensp; OpenNCC is composed of SENSOR board and CORE board, equipped with 2MP pixel sensor. OpenNCC DK's CORE board uses the CSI_MIPI_RX 4lane interface to exchange data with the CORE board. The maximum sensor resolution is 20M@30fps, and it also supports 3D modules and infrared modules. CORE board output interface USB2.0/3.0, equipped with 4G/8G LPDDR4. 
  OpenNCC  function:
* Plug and play

  OpenNCC  uses the USB-C data interface, and as long as it is connected to the computer, it can be developed, and it is easy to create its own variety of AI cameras.
* Openvino standard model easily switch to use

Thanks to the Intel AI chip included in OpenNCC, it is compatible with all OpenVINO models
As long as it is a model in OpenVINO standard format, after format conversion and parser processing, it can be used through OpenNCC View software
* Support multiple deep learning frameworks

   Support Caffe, ONNX, TensorFlow, MXNet and other deep learning frameworks to facilitate user development 
* High-quality image output

   Support 1920x1080 resolution image quality, complete preprocessing of images Support YUV420, H.264, MJPEG multiple video format output
* Support secondary development

 &ensp; OpenNCC provides a dedicated OpenNCC CDK development kit and related technical documents. It supports C/C++/Python. Users can easily call related API interfaces to implement camera parameter settings, model download, output video parameter settings, and quickly implement smart camera algorithm deployment. The OpenNCC model supports the official model provided by openvino, and also supports the deployment of custom algorithm models to quickly realize productization.

* Specifications：

|  Model   | OpenNCC   |
|  ----  | ----  |
| Computing power  | Up to 4TFlops |
| Support model  | All the OpenVINO models |
| Support Framework|ONNX，TensorFlow，Caffe，MXNet，Kaldi|
| ISP | √ |
| Open source | OpenNCC CDK，Development Technical Document，Configuration Tool OpenNCC View |
| Supported languages | C/C++/Python |
| CDK support function | 1. Get video stream
| | 2. AI model download and replacement
||3. Get model operation results
||4. Camera photo, reset, etc. |
| OpenView function| Configure camera parameters, configure camera local AI model |
| OpenNCC CDK Supported OS | Linux / windows |
| OpenNCC View | Linux |
|  Hardware  |
| VPU | Intel Movidius Myriad X MV2085 |
| RAM | 4Gb or 8Gb |
| Data interface | USB Type-C 2.0/3.0|
| Power| 5V / 2A|
| Camera module| 2MP Visible light module|
| Resolution | 1920 x 1080 or 4K|
| Frame rate| 30Hz|
| FOV (H)| 50°|

![](https://openncc.eyecloud.tech/ncc-cn/20200628/2eUogoyg.png)
