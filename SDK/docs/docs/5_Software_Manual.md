## Overview
This chapter introduces the SDK which contains the library of resources necessary for the application and the related basic application routines.

## SDK structure

| Directory     | Content                                                      |
| ------------- | ------------------------------------------------------------ |
| ./Platform    | Contains  scripts for generating runtime environments for different platforms. |
| ./SDK/docs    | Contains  SDK-related introduction and documentation.        |
| ./SDK/Drivers | Contains  the drivers that must be installed for different platforms. |
| ./SDK/Example | Contains  SDK related routines.                              |
| ./SDK/Source  | Includes  firmware, model and SDK library files.             |
| ./SDK/Tools   | Includes  relevant model conversion and compilation tools.   |
| ./Viewer      | Includes  compiled Viewer and QT source code.                |



## Supported platform

SDK支持的产品如下：

*  OpenNCC DK

*  OpenNCC Lite

*  OpenNCC USB

 SDK支持的平台如下：

* Ubuntu 16.04, Ubuntu 18.04
* Windows 10
* Raspberry Pi
* Arm Linux

 SDK支持的语言如下：

- C/C++
- Python3.5、Python3.7

### 2.1 SDK development package directory structure

| Directory                                                    | Contents                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| Example/How_to/How_to_use_sdk                                | Sample  application, how to use the SDK library in your project. |
| Example/How_to/Capture_video                                 | Sample  application, how to get a video stream using the SDK library. |
| Example/How_to/Load_a_model                                  | Sample  application, how to download a deep learning model in Blob format using the  SDK library. |
| Example/How_to/work_with_multiple_models                     | Sample  application, how to use a second-level model.        |
| Example/How_to/Python_demo                                   | Python  related examples.                                    |
| Example/Linkage_demo/  <br />work with AlwaysAI /  <br />pedestrian_tracking_demo | Face  model, using AlwaysAI to parse the result display and count the number of  people passing the recognition area. |
| Example/Linkage_demo/  <br />work_with_OpenVINO/  <br />human_pose_estimation_demo | Human  skeleton model, using OpenVINO to parse the result display. |
| Example/Linkage_demo/  <br />work_with_OpenVINO/  <br />interactive_face_detection_demo | Face,  age, gender, and mood models, using OpenVINO parsing results. |
| Tools/myriad_compiler                                        | IR file conversion Blob file tool                            |
| Tools/deployment                                             | Kit deployment script                                        |



## OpenVINO installation and getting start  
 &ensp;&ensp; To deploy a deep learning model on end-point target devices, you need to optimize and convert a trained model to the VPU characteristics to achieve higher operating performance. OpenNCC is compatible with OpenVINO's tool set and model format, and needs to rely on Intel OpenVINO's model optimizer to complete model optimization and conversion into Blob format. When using OpenNCC SDK, you need to install OpenVINO as follows:
 If you need to convert the trained model yourself, you need to install OpenVINO to run the model optimizer.
 When OpenVINO runs in a mixed mode with the OpenVINO inference engine, it also needs OpenVINO support.

### Download and install OpenVINO
 &ensp; OpenNCC currently supports OpenVINO version: 2020.3.194, OpenVINO installation reference [here](/openvino_install.md)  

  ### Intel Free model download  
  &ensp; OpenNCC supports OpenVINO models, Intel has a large number of free trained models for learning reference and testing. After we have installed OpenVINO, we can use the Intel download tool to download the model collection. Model download tool path: `openvino/deployment_tools/tools/model_downloader/downloader.py`, common commands are as follows:  
  * View all downloadable models：./downloader.py --print
  * Download the specified model：./downloader.py --name *  

  For example, download a face detection model ：`./downloader.py --name face-detection-adas-0001-fp16`  
  ![Figure-1](/openncc/docimg/sw_figure1.png)  

### Model optimization and format conversion  
&ensp;When we need to deploy a trained model to OpenNCC, we need to optimize and transform the model. After installing OpenVINO, you can use the model optimization tool: `/opt/intel/openvino/deployment_tools/model_optimizer/mo.py` to optimize the model. For specific documents, see the official Intel documents: [Model Optimizer Developer Guide](https://docs.openvinotoolkit.org/2020.3/_docs_MO_DG_prepare_model_Config_Model_Optimizer.html).  

&ensp;After the model optimization is completed, the model needs to be converted to the Blob format before it can be deployed on OpenNCC. In the OpenVINO installation directory: `/opt/intel/openvino/deployment_tools/inference_engine/lib/intel64myriad_compile` tool, the method of use is as follows:
Enter from the command line terminal：`./myriad_compile -m input_xxx-fp16.xml  -o output_xxx.blob  -VPU_PLATFORM VPU_2480 -VPU_NUMBER_OF_SHAVES  8  -VPU_NUMBER_OF_CMX_SLICES 8`  

&ensp; After the format conversion is completed, the model can be deployed on OpenNCC, refer to: `ncc_SDK/Samples/How_to/load a model`, or use the OpenNCC View interface program to add the model to deploy and test it.  

## OpenNCC operating mechanism
&ensp; From a model training environment to embedded deployment, it is a very important task, which requires mastering the framework of deep learning, such as commonly used: Caffe*, TensorFlow*, MXNet*, Kaldi*, etc.In addition, it is very important to master the deployed embedded platform. You need to understand the platform performance, system architecture characteristics, and then combine the platform characteristics to optimize the training model framework, and finally tune, transplant, and deploy to the embedded platform.  

&ensp; OpenNCC focuses on the rapid deployment of deep learning models, is compatible with Intel OpenVINO tools, and for embedded graphics and image application scenarios, it has completed the integration of different resolution sensors from 2MP to 20MP on end-point target devices, and the end-point target devices has realized the deployment of professional-level ISP.  OpenVINO optimized converted model files can be dynamically downloaded to the end-point OpenNCC camera to achieve rapid deployment of deep learning models. OpenNCC has designed independent working mode, mixed development mode and co-processing compute stick mode to adapt to different work application scenarios.

### 4.1 OpenNCC standalone mode
&ensp;In the independent mode, OpenNCC independently runs a deep learning model, and feeds back the inference results to the user through the OpenNCC SDK API.
The application deployment process is as follows:  

![Figure-2](/openncc/docimg/sw_figure2.png)  

&ensp;According to the OpenVINO documentation, for a specific training framework [Configure Model Optimizer](https://docs.openvinotoolkit.org/2020.3/_docs_MO_DG_prepare_model_Config_Model_Optimizer.html)   

&ensp;Run [Model Optimizer](https://docs.openvinotoolkit.org/2020.3/_docs_IE_DG_Introduction.html#MO) to produce an optimized Intermediate Representation (IR) of the model based on the trained network topology, weights and biases values, and other optional parameters.
The IR is a pair of files that describe the whole model:

* .xml: The topology file - an XML file that describes the network topology
* .bin: The trained data file - a .bin file that contains the weights and biases binary data  
* Then run myriad_compile to generate a BLOB file from the IR file.  
* To integrate the BLOB model file generated after optimization using OpenNCC SDK, see the demo program of `Examples/How_to/Load a model` under SDK.  
* &emsp; OpenNCC View is an application demonstration program with an operating interface integrated with OpenNCC SDK. You can also use OpenView to deploy models and obtain test results. Refer to OpenNCC View Guide Because different depth models have differentiated inference output results, if users cannot find a suitable post-processing analytical model under the SDK, they need to refer to `Examples/How_to/load a model` and write post-processing code in combination with their own application scenarios.

#### 4.1.1 Secondary model operation support
Considering the end-to-side computing capability, at present, SDK multi-level models support cascading of two-level models, as shown in the following figure:  

![F](/openncc/docimg/ch/SoftManualF10.jpg)  

The first level model must be a target detection or classification model, and the output is defined as follows:  

![F](/openncc/docimg/ch/SoftManualF11.jpg)   

process：  

1）After pre CV [1], the original image scale is converted to the input size of the first level model, and the corresponding format conversion is performed. Then the first level model reasoning calculation is performed, and the first level reasoning result is output to pre CV [2].  

2) The pre CV [2] module analyzes the reasoning results of the first level model, and takes the qualified label and conf detection target according to the coordinate starting point (x)_ min, y_ Min), the end point (x)_ max,y_ Max) from the original graph's Cross and scale are converted to the input size of the secondary model, and the corresponding format conversion is performed to enter the second level model reasoning.  

3）Finally, the reasoning results of the first level model and all the second level models are packaged and output together.  

Model output analysis (parameter configuration in the figure is: valid label: 2,3, conf = 0.8)  

![F](/openncc/docimg/ch/SoftManualF12.png)  

Example：`Examples/How_to/work_with_multiple_models`,the first level model is vehicle and license plate detection, the second level model is license plate detection, and the effective label is set to 2  

Based on the detection results of the first stage, the detection coordinates of the first stage are adjusted appropriately, which is conducive to the identification of:  

*Fine tuning the starting point to the left and up（startXAdj，startYAdj ）  

*Bottom right down fine adjustment（endXAdj，endYAdj）  

  cnn2PrmSet.startXAdj  = -5;  

  cnn2PrmSet.startYAdj  = -5;  

  cnn2PrmSet.endXAdj   = 5;  

  cnn2PrmSet.endYAdj   = 5;   

### OpenNCC mixed mode
&emsp;When it is necessary to solve some complex application scenarios, multiple network model combination processing is required, OpenNCC end-point computing performance cannot be met, or the end-side processing needs to be concentrated on the edge side for post-processing, system expansion is often required. Run the models with high real-time requirements on the OpenNCC end-point, and the other models on the post-processing edge machine or cloud.  

&emsp;As shown in the figure, Model-1 runs on the OpenNCC end-point  to complete the pre-processing of the video stream. OpenNNC returns the results of the first-level processing model to the user application. Model-1 and Model-2 fully run under the OpenVINO inference engine to implement subsequent processing.  

![Figure-3](/openncc/docimg/sw_figure3.png)  

&emsp;In `Examples/Linkage_demo/work_with_OpenVINO` demonstrated how to combine OpenNCC and OpenVINO on Host PC to implement a distributed AI system.  

### 4.3 Co-processing compute stick mode
&emsp;OpenNCC's co-processing mode is similar to Intel NCS2. In this mode of operation, OpenNCC's vision sensor does not work, and users can use OpenNCC alone to achieve full compatibility with the OpenVINO environment. The typical deep learning model deployment process of OpenVINO is as follows:  

![Figure-4](/openncc/docimg/sw_figure4.png)    

&emsp;[Configure Model Optimizer](https://docs.openvinotoolkit.org/2020.3/_docs_MO_DG_prepare_model_Config_Model_Optimizer.html) for specific training framework according to OpenVINO documentation. 

&emsp;Run Model Optimizer to produce an optimized Intermediate Representation (IR) of the model based on the trained network topology, weights and biases values, and other optional parameters. 

&emsp;Download the optimized IR file to OpenNCC to run the Inference Engine. For details, refer to OpenVINO documents: [Inference Engine validation application](https://docs.openvinotoolkit.org/2020.3/_docs_IE_DG_Introduction.html#IE) and [sample applications.](https://docs.openvinotoolkit.org/2019_R1.1/_docs_IE_DG_Samples_Overview.html)  

&emsp;Copy `Source/Firmware/fw /usb-ma2x8x.mvcmd` and replace openvino/inference_engine/lib/intel64/usb-ma2x8x.mvcmd in the openvino installation directory.(Remarks: usb-ma2x8x.mvcmd in the openvino installation directory must be backed up before replacement. This file needs to be restored when using NCS2 inference)  

### 4.4 Difference between independent mode and co-processing mode
&emsp;The right side of the figure below is the independent mode of OpenNCC, and the left side is the co-processing mode of OpenNCC (similar to Intel NCS2).  

![Figure-5](/openncc/docimg/sw_figure5.png'))  

 &emsp;When we need to deploy a vision-based deep learning model, first we need to obtain a high-quality video stream, then run the inference engine to calculate the input image data, and finally output the result. For the co-processing mode on the left, we need an OpenNCC or Intel NCS2 implements end-to-side reasoning. At the same time, we need to obtain a video stream from a camera and send the video frame to OpenNCC  via USB. In the independent mode on the right, no additional camera is needed to obtain the video stream. We only need to download the model to OpenNCC to obtain the deduction results.  

 Refer to OpenVINO official website:https://docs.openvinotoolkit.org/
