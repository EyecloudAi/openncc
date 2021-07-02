# Install QT

OS：Windows10

QT version：5.12.9

VS:VS 2017

**<font color="red">Please confirm that vs2017 is installed before installing QT</font>** 

download url:https://download.qt.io/official_releases/qt/5.12/5.12.9/

![image-20210701094543097](./image/download.png)

**step 1**：open `qt-opensource-windows-x86-5.12.9.exe`

![image-20210701104936380](./image/qtinstallstep1.png)

**step 2**： Enter your QT account  

![image-20210701105159854](./image/qtinstallstep2.png)

**step 3**： Select `I have read and approve the obligations of using Open Source Qt`

![image-20210701105320847](./image/qtinstallstep3.png)

**step 4**：Next

![image-20210701105408644](./image/qtinstallstep4.png)

**step 5**： Select installation directory 

![image-20210701105511008](./image/qtinstallstep5.png)

**step 6**： Select component 

![image-20210701105616479](./image/qtinstallstep6.png)

![image-20210701105631358](./image/qtinstallstep61.png)

**step 7**：Select `I have read and agree to the terms contained in the license agreements.`

![image-20210701105839930](./image/qtinstallstep7.png)

**step 8**：Next

**step 9**：Install

![image-20210701105925691](./image/qtinstallstep9.png)

# Download ffmpeg and opencv

ffmpeg url：https://pan.mebk.org/s/0WmEsM passwd：OpenNcc

opencv url：https://pan.mebk.org/s/aqpZUg passwd：OpenNcc

**step 1**： Download ffmpeg and opencv 

**step 2**： Extract it to where you want to put it 

![image-20210701112000759](./image/download_ffmpeg.png)

![image-20210701112011686](./image/download_opencv.png)

# Run View

 Run the windows script first, you can see the following folder.

![image-20210701113217960](./image/RunView1.png)

**step 1**：Open qtcreator

![image-20210701140440828](./image/RunViewstep1.png)

![image-20210701140501303](./image/RunViewstep1_1.png)

![image-20210701113542952](./image/RunViewstep1_2.png)

**step 2**：Open project

![image-20210701113742952](./image/RunViewstep2.png)

press `Ctrl+O`

![image-20210701114037953](./image/RunViewstep2_1.png)

**step 3**：OK

![image-20210701114204473](./image/RunViewstep3.png)

**step 4**：Configure Project

![image-20210701114240452](./image/RunViewstep4.png)

**step 5**：modify ffmpeg path and opencv path in `OpenNCC.pro`

![image-20210701114402439](./image/RunViewstep5.png)

![image-20210701114445898](./image/RunViewstep5_1.png)

* line 83：

![image-20210701114524680](./image/RunViewstep5_2.png)

* line 84：

![image-20210701114618350](./image/RunViewstep5_3.png)

* line 87 and line 91：

![image-20210701114723371](./image/RunViewstep5_4.png)

* line 97

![image-20210701114816532](./image/RunViewstep5_5.png)

 After modification (Note: In the red box is what I modified )

![image-20210701115031508](./image/RunViewstep5_6.png)

**step 6**：select Release

![image-20210701115233357](./image/RunViewstep6.png)

**step 7**：build

![image-20210701115301651](./image/RunViewstep7.png)

**step 8**：Run

![image-20210701115502165](./image/RunViewstep8.png)

 You'll find nothing,don't worry.

![image-20210701115610171](./image/RunViewstep8_1.png)

**step 9**：copy ffmpeg dll and opencv dll to release

![image-20210701115724396](./image/RunViewstep9.png)

![image-20210701115803468](./image/RunViewstep9_1.png)

![image-20210701115831402](./image/RunViewstep9_2.png)

opencv_world340.dll for release

![image-20210701115858209](./image/RunViewstep9_3.png)

opencv_world340d.dll for debug

![image-20210701115914280](./image/RunViewstep9_4.png)

Now,we are in release.

![image-20210701120003718](./image/RunViewstep9_5.png)

then Run.

![image-20210701115502165](./image/RunViewstep9_6.png)

![image-20210701133423011](./image/RunViewstep9_7.png)

copy `Configuration` to `release`

![image-20210701133522685](./image/RunViewstep9_8.png)

![image-20210701133600145](./image/RunViewstep9_9.png)

Run again.

![image-20210701133655514](./image/RunViewstep9_10.png)

![image-20210701133711271](./image/RunViewstep9_11.png)

**step 10**：`Get Device info`

After `Get Device info`

![image-20210701133819108](./image/RunViewstep10.png)

**step 11**：Select `Stream Format`

![image-20210701133919698](./image/RunViewstep11.png)

**step 12**：Select `Stream Resolution`

![image-20210701134118747](./image/RunViewstep12.png)

**step 13**： You can choose the model or not 

![image-20210701134129554](./image/RunViewstep13.png)

**step 14**：You can modify `Model Score` or not

![image-20210701134229926](./image/RunViewstep14.png)

**step 15**：You can modify `Display Scaler` or not

![image-20210701134253454](./image/RunViewstep15.png)

**step 16**：choose `Exposure control` Auto or Manual

![image-20210701134421596](./image/RunViewstep16.png)

**step 17**：`Start running model`

![image-20210701134535679](./image/RunViewstep17.png)



# Pack QT program

After Run View,find `OpenNCC.exe` in `release`

![image-20210701134825921](C:\Users\beams\AppData\Roaming\Typora\typora-user-images\image-20210701134825921.png)

**step 1**：copy `OpenNCC.exe` to other folder.

![image-20210701134951591](./image/Packstep1.png)

![image-20210701135928845](./image/Packstep1_1.png)

**step 2**：copy ffmpeg dll and opencv dll to this floder.(dll in Run View step 9)

![image-20210701140231476](./image/Packstep2.png)

**step 3**：Run `Qt 5.12.9(MSVC 2017 64-bit)` 

![image-20210701140440828](./image/Packstep3.png)

![image-20210701140501303](./image/Packstep3_1.png)

![image-20210701140356902](./image/Packstep3_2.png)

enter this floder.

![image-20210701140741888](./image/Packstep3_3.png)

use `windeployqt.exe OpenNCC.exe` pack program

![image-20210701140845467](./image/Packstep3_4.png)

**step 4**：copy `Configuration` to this floder.

![image-20210701141004846](./image/Packstep4.png)

![image-20210701141031186](./image/Packstep4_1.png)

**step 5**：Run `OpenNCC.exe`

![image-20210701141114940](./image/Packstep5.png)

