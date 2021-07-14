## Preparatory work 

### Download Software Development Kit(SDK)

* Go to [official website](https://www.openncc.com/)
* Click **Documentation**

![img](/openncc/docimg/website_support.png)

![image-20210702134237102](/openncc/docimg/website_download.png)

* Download the zip file or copy the address and use Git to clone it.



After download completed, go to `openncc/Platform`，the directory is as follows：

![image-20210702141842821](/openncc/docimg/ch/mulu.png)

Select the required file directory to enter, the corresponding folder contains the scripts for environment building.

------

<font color="red">Warning: The environment builder script will automatically generate and overwrite related files, please make sure it is the first time you run it or have completed a backup before running it.</font>

------

## Quick Start to Linux

### Environment construction

* Go to the directory`opennccc/Platform/Linux`。
* Right click to open the terminal.
* Enter the command `./linux.sh`。

![image-20210702142033823](/openncc/docimg/ch/linuxsh.png)

The generated directory is as follows.：

![image-20210702142115637](/openncc/docimg/ch/linuxsh_dir.png)

### OpenNCC_Linux Operation Demo

* Go to the directory: openncc/Platform/Linux/Viewer/OpenNcc_Linux.

* Right-click to open the terminal and execute `sudo ./AppRun`l to start the software.

* Connect the OpenNCC camera to the USB 3.0 port of your computer and click on the Get device info button to get the device information, at this point there will be two prompts in the log area.：

  \- USB3.0：USB interface is 3.0, yuv outflow is currently available。

  \- USB2.0：USB interface is not 3.0, yuv outflow will be disabled。

------

<font color="red">Tip: The OpenNCC TYPE-C interface has two modes of positive and negative access, corresponding to USB3.0 and USB2.0 respectively. The USB2.0 mode temporarily disables the YUV420P format video stream output due to the transfer rate.</font>

------

![image-20210702142441399](/openncc/docimg/ch/linux_view1.png)

![image-20210702142529686](/openncc/docimg/ch/linux_view2.png)

​		If you need to switch USB mode, rotate the TPYE-C port to access it again and click the Get device info button again. If there is still no change, please check your computer USB port type or [contact us](https://www.openncc.com/contact).

* Choose any of the video streaming formats yuv420p/H.264/H.265/mjpeg.

* `Stream Resolution`：Two resolutions are available, 1080p and 4K.(Specifically determined by OpenNCC product type)

![image-20210702142649007](/openncc/docimg/ch/linux_view3.png)

* `1st network model`：Select the algorithm model. 

![img](/openncc/docimg/ch/linux_view4.png)

Currently supports more than 10 models to choose. If choose None that will not load models, only the original video stream could be displayed.

* The algorithm area can be restricted by boxing the ROI region, and the algorithm will only recognize scenes within the region.(See the [running result](#_Running result) demonstration for details)

* `2nd network model`：Select the secondary algorithm model.

Example model：vehicle-license-plate-detection-barrier-0106-fp16

![img](/openncc/docimg/ch/linux_view5.png)

*  `Model Score` ：Set the minimum score for the algorithm to recognize and reach the threshold before the recognition result will be framed in the screen.

*  `Display Scaler` ： Set the video display window size, and you can adjust the display window resolution.
*  `Exposure control`：Set the exposure, you can choose Auto and Manual, when you choose Manual, you can set your own Exposure times and iso.

![img](/openncc/docimg/ch/linux_view6.png)

![image-20210702143055575](/openncc/docimg/ch/linux_view7.png)

* Switch `show state`，l  Select whether to display current status information on the screen, including video stream frame rate, algorithm frame rate, resolution, and device id.
* Switch `inference accelerate`，Select whether to enable algorithm acceleration. (Must be selected before loading the algorithm model).

![img](/openncc/docimg/ch/linux_view8.png)

* Switch  `save_avi`，l  The video will be saved to the directory     openncc/Platform/Linux/Viewer/OpenNcc_Linux/avi folder with the time named avi file after opening the video stream. When the video stream is closed, the video will stop saving. (yuv420p does not have this item)

![img](/openncc/docimg/ch/linux_view9.png)

- Click  `Start running models`, Open the video stream. See [running result](# Running result) for running results

## Quick Start to Windows

### Environment construction

* Go to openncc/Platform/Windows

![img](/openncc/docimg/ch/windows_view1.png)

* Double-click to run `windows.bat`。

* The results are as follows：

![img](/openncc/docimg/ch/windows_view2.png)

* After the script runs successfully, go to the openncc/Platform/Windows/Drivers directory and install the Windows USB driver. Detailed installation steps can be found in <font color="blue">OpenNCC_USB_Driver_install_guide_win.pdf</font>。

### OpenNCC_Windows Operation Demo

* Go to the directory: openncc/Platform/Windows/Viewer/OpenNcc_Windows.
* Double-click to run `OpenNCC.exe`.
* Following steps are the same as [OpenNCC_Linux](#OpenNCC_Linux操作演示)

## Quick Start to Raspberry Pi

### Environment construction

* Open terminal
* Go to openncc/Platform/Raspberry

![img](/openncc/docimg/ch/pi_view1.png)

* Enter command `./pi.sh`。

![img](/openncc/docimg/ch/pi_view2.png)

After running successfully, the following directory will be generated:

```
└── Raspberry
	├── Example
	├── Source
	├── Viewer
	├── pi.sh
	├── ReadMe.md
```

### OpenNCC_Raspberry Operation Demo

------

<font color="red">Tip: Before running OpenNCC_Raspberry, please check the device number and [contact us](https://www.openncc.com/contact) for the key, see openncc/Platform/Raspberry/ReadMe.md for details.</font>

------

* Copy the key file (eyecloud.key) to the directory: openncc/Platform/Raspberry/Viewer/OpenNcc_Raspberry/Configuration/fw.
* Go back to the directory: openncc/Platform/Viewer/OpenNcc_Raspberry.
* Open a terminal and execute `sudo ./AppRun`to start the software.
* Following steps are the same as [OpenNCC_Linux](#OpenNCC_Linux操作演示)。

## Custom（Customization）

### Environment construction

* Go to openncc/Platform/Custom.
* Right click to open the terminal.

* Enter command `./custom.sh`

------

<font color="red">Tip: To run on a custom platform, please [contact us](https://www.openncc.com/contact) for customization services.</font>

------

## Running result

Take the object classification algorithm model as an example.：

Banana results in the algorithm area：

![image-20210702145523142](/openncc/docimg/ch/bannna.png)

Apple results in the algorithm area：

![image-20210702145621696](/openncc/docimg/ch/apple.png)
