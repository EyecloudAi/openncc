## 准备工作 

### 下载Software Development Kit(SDK)

* 进入[官网](https://www.openncc.com.cn/)
* 点击<font color="blue">**→**</font>访问SDK仓库

![img](/openncc/docimg/ch/download.png)

* 下载压缩包或复制地址使用Git克隆即可。

![image-20210702134237102](/openncc/docimg/ch/git.png)



 下载完成后进入`openncc/Platform`，目录如下图：

![image-20210702141842821](/openncc/docimg/ch/mulu.png)

选择需求的文件目录进入，对应文件夹内包含环境搭建的脚本。

------

<font color="red">警告：环境搭建脚本会自动生成和覆盖相关文件，运行前请确认是首次运行或已经完成备份。</font>

------

## 快速入门之linux

### 环境搭建

* 进入目录`opennccc/Platform/Linux`。
* 右键打开终端。
* 输入命令 `./linux.sh`。

![image-20210702142033823](/openncc/docimg/ch/linuxsh.png)

生成目录如下：

![image-20210702142115637](/openncc/docimg/ch/linuxsh_dir.png)

### OpenNCC_Linux操作演示

* 进入openncc/Platform/Linux/Viewer/OpenNcc_Linux 目录。

* 右键打开终端，执行`sudo ./AppRun`，启动软件。

* 连接OpenNCC相机到电脑USB 3.0接口，点击 Get device info 按钮获取设备信息，此时log区域会有两种提示：

  \- USB3.0：USB interface is 3.0, yuv outflow is currently available。

  \- USB2.0：USB interface is not 3.0, yuv outflow will be disabled。

------

<font color="red">提示：OpenNCC TYPE-C接口有正反接入两种模式，分别对应USB3.0 和 USB2.0。受传输速率影响，USB2.0模式下会暂时禁用YUV420P格式的视频流输出 。</font>

------

![image-20210702142441399](/openncc/docimg/ch/linux_view1.png)

![image-20210702142529686](/openncc/docimg/ch/linux_view2.png)

​		如果需要切换USB模式，旋转TPYE-C接口再次接入，重新点击Get device info按钮即可。如果仍然没有改变，请检查电脑USB接口类型或[联系我们](https://www.openncc.com.cn/jszc)。

* 任意选择一种视频流格式 yuv420p/H.264/H.265/mjpeg。 

* `Stream Resolution`：两种分辨率可供选择，1080p和4K。（具体由OpenNCC产品类型决定）

![image-20210702142649007](/openncc/docimg/ch/linux_view3.png)

* `1st network model`：选择算法模型。

![img](/openncc/docimg/ch/linux_view4.png)

目前支持10多种模型可选，选择None即不加载模型，仅显示原始视频流。

* 可以通过框选ROI区域限制算法区域，算法只对区域内的场景进行识别。

（具体见[运行结果](#运行结果演示)展示）

* `2nd network model`：选择二级算法模型。

示例模型：vehicle-license-plate-detection-barrier-0106-fp16

![img](/openncc/docimg/ch/linux_view5.png)

*  `Model Score` ：设置算法识别的最低分数，达到阈值后才会在画面中框选出识别结果。

* `Display Scaler` ：设置视频显示窗体大小，可以调节显示窗口分辨率。
* `Exposure control`：设置曝光，可以选择Auto和Manual，当选择Manual时，可以自己设置Exposure times和iso。

![img](/openncc/docimg/ch/linux_view6.png)

![image-20210702143055575](/openncc/docimg/ch/linux_view7.png)

* 勾选`show state`，选择是否在画面上显示当前状态信息，包括视频流帧率、算法帧率、分辨率、设备id。
* 勾选`inference accelerate`，选择是否启用算法加速.（必须在加载算法模型前选择）。

![img](/openncc/docimg/ch/linux_view8.png)

* 勾选`save_avi`，视频将在打开视频流后，以时间命名的avi文件保存到目录openncc/Platform/Linux/Viewer/OpenNcc_Linux /avi文件夹下。当关闭视频流后，视频会停止保存。（yuv420p没有此项）

![img](/openncc/docimg/ch/linux_view9.png)

## 快速入门之Windows

### 环境搭建

* 进入openncc/Platform/Windows

![img](/openncc/docimg/ch/windows_view1.png)

* 双击运行`windows.bat`。

* 结果如下：

![img](/openncc/docimg/ch/windows_view2.png)

* 脚本运行成功后，进入openncc/Platform/Windows/Drivers目录, 安装Windows USB驱动。详细安装步骤见 <font color="blue">OpenNCC_USB_Driver_install_guide_win.pdf</font>。

### OpenNCC_Windows操作演示

* 进入openncc/Platform/Windows/Viewer/OpenNcc_Windows 目录。
* 双击运行 `OpenNCC.exe`。
* 后续操作同[OpenNCC_Linux](#OpenNCC_Linux操作演示)

## 快速入门之Raspberry Pi

### 环境搭建

* 打开终端
* 进入openncc/Platform/Raspberry

![img](/openncc/docimg/ch/pi_view1.png)

* 输入命令 `./pi.sh`。

![img](/openncc/docimg/ch/pi_view2.png)

运行成功后，会生成目录如下：

```
└── Raspberry
	├── Example
	├── Source
	├── Viewer
	├── pi.sh
	├── ReadMe.md
```

### OpenNCC_Raspberry 操作演示

------

<font color="red">提示：运行OpenNCC_Raspberry之前，请先查看设备号并[联系我们](https://www.openncc.com.cn/jszc)获取密钥，设备号获取方法详见 openncc/Platform/Raspberry/ReadMe.md</font>

------

* 将密钥文件（eyecloud.key）复制到目录openncc/Platform/Raspberry/Viewer/OpenNcc_Raspberry/Configuration/fw
* 回到目录openncc/Platform/Viewer/OpenNcc_Raspberry
* 打开终端，执行 `sudo ./AppRun`，启动软件。
*  后续操作同[OpenNCC_Linux](#OpenNCC_Linux操作演示)。

## Custom（自定义）

### 环境搭建

* 进入openncc/Platform/Custom。
* 右键打开终端。

* 输入命令 `./custom.sh`

------

<font color="red">提示：如需在自定义平台运行，请[联系我们](https://www.openncc.com.cn/jszc)获取定制化服务。</font>

------

## 运行结果演示

以物体分类算法模型为例：

 香蕉在算法区域内结果：

![image-20210702145523142](/openncc/docimg/ch/bannna.png)

苹果在算法区域内结果：

![image-20210702145621696](/openncc/docimg/ch/apple.png)
