## 概述
OpenNCC View是一款用于快速体验OpenNCC开发套件的软件，运行OpenNCC View下默认模型不依赖OpenVINO，OpenNCC View集成了OpenNCC SDK全部API,可以完成OpenNCC在独立模式下实现对相机的连接,固件和AI模型的下载，及完成视频流显示与算法结果的后处理。用户可以通过友好的界面，来操作和控制相机.  
![Figure-1](/openncc/docimg/ch/NccViewF1.png)   

## 运行环境与安装
* 操作系统：Ubuntu16.04(64 bit)、Ubuntu18.04 (64 bit)  
* 编译环境：QT 5.9.9及以上  
### QT安装
[QT下载地址](http://download.qt.io/archive/qt/)  
开发所用版本为QT5.12.7，尽量保持一致。下载linux安装包后，添加可执行权限，然后运行。  
安装按默认路径即可，断网安装不用登陆账号，否则需要注册验证后完成安装。  
QT安装完成后运行简单的例子，如果编译会出现`"cannot find -lGL"`错误：  
可以使用`locate libGL*`查找`libGL.so`文件，再用管理员权限添加软链接，链接到/usr/lib目录`locate libGL*`，
`sudo ln -s /usr/lib/xxx/libGL.so.1 /usr/lib/libGL.so`  
### 安装依赖包
* OpenCV  
`sudo apt-get update`  
`sudo apt-get install libcv-dev`  
`sudo apt-get install libopencv-dev`   
 * 安装libsub1.0  
`sudo apt-get install libusb-dev`  
`sudo apt-get install libusb-1.0-0-dev`  
* 安装ffmpeg  
`sudo apt-get install ffmpeg`  
### 编译View工程
1. 使用Qt Creator打开`View/Linux../..../..`的`OpenNCC.pro`工程
2. 编译工程
3. 将`View/Linux../../Configuration`目录拷贝到编译运行目录下，如`build-OpenNCC-Desktop_Qt_5_12_7_GCC_64bit-Debug`  
4. 开始调试、运行程序

### 模型解析
模型在OpenNCC完成推演后，通过OpenNCC SDK API获取实时的推演结果，OpenView针对物体检测这类模型[Object Detection Models](https://docs.openvinotoolkit.org/2019_R1.1/usergroup1.html)实现了推演结果的通用解析器。  
    OpenView的推演后处理支持如下格式输出：

    输出的数据形态：[1, 1, N, 7]
    N是当前帧下检测到的物体框数量
    对于每个检测框，包含以下信息格式：[image_id, label, conf, x_min, y_min, x_max, y_max], 其中:
        image_id - ID of the image in the batch
        label - predicted class ID
        conf - confidence for the predicted class
        (x_min, y_min) - coordinates of the top left bounding box corner
        (x_max, y_max) - coordinates of the bottom right bounding box corner.  

用户可以训练自己的模型，并将输出层按照以上格式定义，可以免编程使用OpenView的模型导入功能添加并测试物体检测模型。如果用户需要添加其他输出格式的模型，需要自己参考ncc_SDK/Samples/How_to/load a model并结合自己应用场景来编写解析代码。
### 功能点详细介绍
1）Get Device Info：获取设备与电脑usb连接信息及NCC设备模块信息，若连接的设备支持，能够解锁更多功能选项。（例如NCC与电脑通过usb 3.0连接，可解锁yuv出流显示视频；NCC装配了4K模组，可解锁4K分辨率显示）

2）Stream Format：选择NCC相机USB视频输出格式，目前支持YUV420P，H.264，MJPEG格式。（选择YUV420P前需先点击Get Device Info，仅在usb 3.0连接时可选）

3）Stream Resolution：更换NCC相机的视频分辨率，可选1080P或4K. 4K分辨率支持需要选装相应4K摄像模块。 （选择4K前需先Get Device Info，仅在模组支持4K时可选）

4）1st network model：选择算法模型，选择None即不加载模型，仅出流显示视频，而选择加载算法模型后，可以通过框选ROI区域，仅对区域内的场景进行识别。

5）Model Score：加载算法后，对物品的识别计算结果是有分数的，当超过了某一分数阈值，才会在视频中框选，而Model Score即为控制阈值的选项，根据用户需求，实时调整识别的最低分数（默认值为0.5）

6）ROI：配合算法模型使用，加载模型后，如果需要仅对某一区域进行算法识别，可以手动点击鼠标左键拖动，框选出自己感兴趣的区域，仅对区域内的场景进行识别

7）Display Scaler：视频显示时，由于不同电脑的分辨率不同，按原本尺寸显示1080P或4K的视频，可能会出现视频大小超出桌面大小的情况，用户可通过Display Scaler实时控制视频显示窗体大小（默认值为0.5）

8）Add model：点击"add model"  ，导入生成好的.blob模型文件，此处注意，需要把.blob文件和对应的IR文件(.xml&&.bin)放在同一位置，且文件名相同。.blob文件的生成过程参考开发手册。添加后即可在1st network model中选择对应模型。

9）Del model：删除用户导入的模型文件，删除后即会在1st network model中移除用户自己的模型。

10）Start running model：点击即开始加载固件，并根据用户的选项，显示视频窗口

11）Log：显示NCC工作时的log，用户可在此处查看设备的运行状况，以及关于设备异常状态的提示信息

12）Algo Results：显示算法结果，当用户选择加载算法模型，且在实际场景中有被识别到的算法模型，即会有算法结果在此处打印（如被识别模型相对当前窗口的坐标信息，及算法计算出的识别分数等）  


### 固件算法加速测试  
| 版本信息 | 60Hz                                              |  YUV |        |      |                | H.264 | H.264  |      |        | MJPEG |  MJPEG |      |        |
|------|---------------------------------------------------|------|--------|------|----------------|-------|--------|------|--------|-------|--------|------|--------|
|      | 1080P  3.0usb                                     | 双引擎  |        | 单引擎  |                | 双引擎   |        | 单引擎  |        | 双引擎   |        | 单引擎  |        |
|      | 模型                                                | 传输帧率 | AI FPS | 传输帧率 | AI FPS         | 传输帧率  | AI FPS | 传输帧率 | AI FPS | 传输帧率  | AI FPS | 传输帧率 | AI FPS |
|      | classification-fp16                               |      |        |      | 14             |       |        |      | 14     |       |        | 14   |        |
|      | face-detection-retail-0004-fp16                   |      |        |      | 55             |       |        |      | 55     |       |        | 52   |        |
|      | face-detection-adas-0001-fp16                     |      |        |      | 8              |       |        |      | 8      |       |        | 8    |        |
|      | person-detection-retail-0013-fp16                 |      |        |      | 6              |       |        |      | 6      |       |        | 6    |        |
|      | person-vehicle-bike-detection-crossroad-0078-fp16 |      |        |      | 2              |       |        |      | 3      |       |        | 3    |        |
|      | face-person-detection-retail-0002-fp16            |      |        |      | 5              |       |        |      | 5      |       |        | 5    |        |
|      | pedestrian-detection-adas-0002-fp16               |      |        |      | 8              |       |        |      | 9      |       |        | 9    |        |
|      | vehicle-detection-adas-0002-fp16                  |      |        |      | 9              |       |        |      | 9      |       |        | 9    |        |
|      | vehicle-license-plate-detection-barrier-0106-fp16 |      |        |      | 28             |       |        |      | 28     |       |        | 29   |        |
|      | --license-plate-recognition-barrier-0001          |      |        |      | 23             |       |        |      | 23     |       |        | 23   |        |
|      | pedestrian-and-vehicle-detector-adas-0001-fp16    |      |        |      | 7              |       |        |      | 7      |       |        | 7    |        |
