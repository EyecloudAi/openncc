## Overview
&emsp;OpenNCC View is a software used to quickly experience the OpenNCC development kit . The default model under OpenNCC View does not depend on OpenVINO. OpenNCC View integrates all APIs of NCC SDK, realizes connection to OpenNCC, downloads of firmware and AI models, video stream display, and post-processing of algorithm results. Users can operate and control the camera through a friendly interface.
![Figure-1](/openncc/docimg/view_figure1.png)  

## Model analysis
&emsp;After the model is deduced by OpenNCC, the real-time deduction result is obtained through the OpenNCC CDK API. OpenNCC View implements a general parser for the deduction result for Object Detection Models.  
&emsp;OpenNCC View Post-inference processing supports output in the following formats:  

* The output data form:[1, 1, N, 7]
* N is the number of object frames detected in the current frame
* For each detection frame ,contains the following information format:
* [image_id, label, conf, x_min, y_min, x_max, y_max]:
* image_id - ID of the image in the batch
* label - predicted class ID
* conf - confidence for the predicted class
* (x_min, y_min) - coordinates of the top left bounding box corner
* (x_max, y_max) - coordinates of the bottom right bounding box corner.  
&emsp;Users can train their own models and define the output layer according to the above format, and can use the model import function of OpenNCC View to add object detection models without programming. If users need to add models in other output formats,You need to refer to the demo in ncc_cdk/Samples/How_to/load a model and write post-processing code in combination with your own application scenario.

## Functions detail
1) Get Device Info： 

&emsp;Obtain the USB connection information of the device and the computer, and the NCC device module information. If the connected device supports it, you can unlock more functional options. (For example, if NCC is connected to a computer via USB 3.0, you can unlock the yuv output format; if NCC is equipped with a 4K module, you can unlock the 4K resolution display)  

2）Stream Format：  

&emsp;Choose NCC camera USB video output format, currently supports YUV420P, H.264, MJPEG format. (You need to click Get Device Info before selecting YUV420P, which is only available when USB 3.0 is connected)  

3）Stream Resolution：  

&emsp;Change the video resolution of NCC camera, 1080P or 4K can be selected. 4K resolution support requires optional 4K camera module. (Get Device Info before selecting 4K, only available if the module supports 4K)  

4）1st network model：  

&emsp;Select the algorithm model, select None to not load the model, and only display the video stream. After selecting the algorithm model to load, you can select the ROI area by frame to identify only the scenes in the area.  

5）Model Score：  

&emsp;After loading the model, the recognition calculation result of the item is scored. When it exceeds a certain score threshold, it will be framed in the video, and the Model Score is the option to control the threshold. The minimum score of the recognition can be adjusted according to user needs. (The default value is 0.5)  

6）ROI：  

&emsp;Used in conjunction with the algorithm model. After loading the model, if you need to perform algorithm identification only for a certain area, you can manually select the area of interest and only identify the scenes in the area.  

7）Display Scaler：  

&emsp;When the video stream is displayed, due to the different resolutions of different computers, if the 1080P or 4K video is displayed at the original size, the video size may exceed the desktop size. The user can control the video display window size in real time through the Display Scaler (default value 0.5)  

8）Add model：  

&emsp;Click "add model" to import the generated .blob model file. Note here that you need to put the .blob file and the corresponding IR file (.xml&&.bin) in the same location and with the same file name. Refer to the “OpenNCC Software Manual.docx” for generating .blob files. After adding, you can select the corresponding model in 1st network.  

9）Del model：  

&emsp;Delete the model file imported by the user, and it will be removed in the 1st network model after deletion  

10）Start running model：  

&emsp;Click to start loading firmware and model, and display video window according to user's options  
11）Log：  

&emsp;Display the log of OoenNCC DK working, the user can view the running status of the device, and the prompt information about the abnormal state of the device  

12）Algo Results：

&emsp;The user chooses to load the algorithm model, and the algorithm recognizes an object in the scene, the algorithm result will be printed here (such as the coordinate information of the identified model relative to the current window, and the recognition score calculated by the algorithm, etc. )

## Algorithms speed up test results

| Version | AR0234 60Hz 1080P 3.0usb                          | YUV    | YUV    | H.264  | H.264  | MJPEG  | MJPEG  |
| ------- | ------------------------------------------------- | ------ | ------ | ------ | ------ | ------ | ------ |
|         | Number of Acceleration Engines                    | 2      | 1      | 2      | 1      | 2      | 1      |
|         | Model                                             | AI FPS | AI FPS | AI FPS | AI FPS | AI FPS | AI FPS |
|         | classification-fp16                               | 25     | 14     | 25     | 14     | 25     | 14     |
|         | face-detection-retail-0004-fp16                   | 80     | 43     | 80     | 55     | 80     | 52     |
|         | face-detection-adas-0001-fp16                     | 15     | 7      | 15     | 8      | 15     | 8      |
|         | person-detection-retail-0013-fp16                 | 12     | 5      | 12     | 6      | 12     | 6      |
|         | person-vehicle-bike-detection-crossroad-0078-fp16 | 6      | 2      | 6      | 3      | 6      | 3      |
|         | face-person-detection-retail-0002-fp16            | 10     | 4.5    | 10     | 5      | 10     | 5      |
|         | pedestrian-detection-adas-0002-fp16               | 15     | 7      | 15     | 9      | 15     | 9      |
|         | vehicle-detection-adas-0002-fp16                  | 15     | 8      | 15     | 9      | 15     | 9      |
|         | vehicle-license-plate-detection-barrier-0106-fp16 | 45     | 24     | 45     | 28     | 45     | 29     |
|         | license-plate-recognition-barrier-0001            | 40     | 24     | 40     | 23     | 40     | 23     |
|         | pedestrian-and-vehicle-detector-adas-0001-fp16    | 12     | 7      | 12     | 7      | 12     | 7      |

