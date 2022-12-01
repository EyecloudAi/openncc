# 							**YoloCamera**





## 简介

-------------------------

YoloCamera旨在帮助用户方便地在OpenNCC上使用YOLO模型，包括YOLO的训练和转化，模型在OpenNCC设备上的部署，识别结果的打包和解包。





## 教程

--------------------------------------------------------------

#### **模型训练**

1. 安装环境依赖

   - **CMake >= 3.18**: https://cmake.org/download/

   - **Powershell** (already installed on windows): https://docs.microsoft.com/en-us/powershell/scripting/install/installing-powershell

   - **CUDA >= 10.2**（可选）: https://developer.nvidia.com/cuda-toolkit-archive 

   - **OpenCV >= 2.4**

   - **cuDNN >= 8.0.2**（可选） https://developer.nvidia.com/rdp/cudnn-archive (on **Linux** follow steps described here https://docs.nvidia.com/deeplearning/sdk/cudnn-install/index.html#installlinux-tar , on **Windows** follow steps described here https://docs.nvidia.com/deeplearning/sdk/cudnn-install/index.html#installwindows)

   - **GPU with CC >= 3.0**（可选）: https://en.wikipedia.org/wiki/CUDA#GPUs_supported

     

2. 训练工具编译
   
   ```
   $ git clone https://github.com/AlexeyAB/darknetcd darknet
   
   $ mkdir build_releasecd build_release
   
   $ cmake ..
   
   $ cmake --build . --target install --parallel 8
   ```
   
   
   
3. 准备数据集

   训练集图片放在train文件夹内，验证集放在val文件夹内

   

4. 数据标注

    ```
    $ git clone https://hub.fastgit.org/AlexeyAB/Yolo_mark.git
     
    $ cmake .
     
    $ make
     
    $ ./linux_mark.sh
    ```

     


   ​		使用方法详见Yolo_mark目录内的readme.md

5. 参数配置

   除两个数据集外，启动训练还需要配置几个参数文件。
   obj.data
   obj.name
   train.txt
   以上三个文件,会在数据标注时自动生成在Yolo_mark/x64/Release/data目录下，obj.name文件包含所有目标的类别名，train.txt包含所有训练图片路径，val.txt非必须，可以手动从train文件中分割出30%的图片用于验证。而obj.data文件申明了上述所有文件的路径和类别总数，如果使用自己的数据集，对应参数修改请在标注前完成修改。

   yolo.cfg(拓扑)
   Yolo.conv（预训练权重）
   cfg和conv存在一定对应关系，考虑此处训练的模型最终需部署在openncc上，推荐使用（yolov4-tiny.cfg+yolov4-tiny.conv.29）或（yolov3-tiny.cfg+yolov3-tiny.conv.11）的搭配，cfg文件可以直接在darknet/cfg目录下找到。
   Cfg文件修改！！
   如果目标类别数量不等于80，则必须修改cfg文件。
   搜索cfg文件中所有yolo层的位置，若总共有3 类目标，则将[yolo]层classes参数定义为3，再将[yolo]上一层[convolutional]层的filters定义为24.计算方式为filters=（classes+5）*3。
   对yolov4-tiny.cfg来说有两个yolo层，所以一共需要修改4个参数。

   ![image-20221109113704997](/home/xulei/.config/Typora/typora-user-images/image-20221109113704997.png)

6. 启动训练

   第二步编译成功后，会在darknet目录下生成./darknet工具。

   

   输入命令：

   ```
    $ ./darknet detector train ./obj.data ./yolov4-tiny.cfg ./yolov4-tiny.conv.2
   ```

   ![image-20221109114856997](/home/xulei/.config/Typora/typora-user-images/image-20221109114856997.png)

   如果你使用显卡训练，显存小于8G的显卡，需要在cfg的第一层[net]中将batch参数改到8以下（8，4，2，1）。
   若训练进行顺利，可看到如下图的训练日志图表。
   训练结束后，可看到一系列.weights文件。这里还是建议制作数据集时设置一个验证集，这样可以直接锁定验证集中map最高的权重yolov4-tiny_best.weights作为后续使用。

   

   6. 启动训练

      - Darknet to tensorflow

        ```
        $ git clone https://github.com/RenLuXi/tensorflow-yolov4-tiny.git
        
        $ cd tensorflow-yolov4-tiny
        
        $ python convert_weights_pb.py --class_names obj.names --weights_file yolov4-tiny_best.weights --tiny
        ```
        
        

      这里需要用到第五步中obj.names和yolov4-tiny_best.weights.

      

      - Tensorflow to IR(openvino推理格式)
        修改json配置文件
        打开tensorflow-yolov4-tiny目录下的yolo_v4_tiny.json，将其中的classes值修改为你自己的类别数，openvino进行tensorflow转换需要用到这个文件。
        然后替换json配置文件

        ```
        $ cp ./yolo_v4_tiny.json  /opt/intel/openvino/deployment_tools/model_optimizer/extensions/front/tf
        ```

        进入openvino模型转换工具目录

        ```
        $ cd /opt/intel/openvino/deployment_tools/model_optimizer
        ```

        转换命令

        ```
        $ python mo.py --input_model yolov4-tiny.pb --transformations_config ./extensions/front/tf/yolo_v4_tiny.json --batch 1 --data_type FP32 --reverse_input_channels
        ```

        

      - IR to blob
        老办法，先初始化openvino环境，然后把上一步生成的xml和bin文件丢过去转换

        ```
        source /opt/intel/openvino_2020.3.194/bin/setupvars.sh
        
        cd /opt/intel/openvino_2020.3.194/deployment_tools/inference_engine/lib/intel64
        
        cp /opt/intel/openvino/deployment_tools/model_optimizer/yolov4-tiny.xml ./
        cp /opt/intel/openvino/deployment_tools/model_optimizer/yolov4-tiny.bin ./
        
        /opt/intel/openvino_2020.3.194/deployment_tools/inference_engine/lib/intel64/myriad_compile -m yolov4-tiny.xml -o yolov4-tiny.blob -VPU_MYRIAD_PLATFORM VPU_MYRIAD_2480 -VPU_NUMBER_OF_SHAVES 6 -VPU_NUMBER_OF_CMX_SLICES 6
        ```

   7. 使用模型
   
      转化完成后，至此，你已经获得了在OpenNCC上部署所需的全部模型文件，xml，bin和blob。
   
      

### **模型部署**

进入Yolo目录

![2022-11-09 14-31-03 的屏幕截图](/home/xulei/图片/2022-11-09 14-31-03 的屏幕截图.png)

将xml，bin和blob文件拷贝到 bin/blob/2020.3/xxx  目录下，然后修改main.cpp中模型文件的具体存放路径。

Line 115

    // 5.2 Blob file save path of the algorithm
    const char *blob = "./blob/2020.3/xxx/yolov4-tiny-fp16.blob";



修改模型输入尺寸,以yolov4-tiny为例。Line 105,106 

    //5.2 Image preprocessing parameter initialization
    cam_info.inputDimWidth  = 416;
    cam_info.inputDimHeight = 416;



编译、运行。

```
$ make all
$ make run
```



### **识别结果打包与解包**

##### 打包会在YOLO运行阶段实时进行，结果以json格式存放在bin目录下。

![2022-11-09 15-16-10 的屏幕截图](/home/xulei/图片/2022-11-09 15-16-10 的屏幕截图.png)

json包中包含两项内容：

​	YOLO：检测目标信息。

​	jpeg：该帧图像。

检测目标信息中，Class表示目标名称，Conf表示目标置信度，x1、y1、x2、y2表示目标锚框左上角和右下角坐标。



##### 获得result.json后，解包方法可参考ParseJson工具。

包括如何提取对应的检测目标信息，以及如何将json中保存的图像字串解码并重新显示的方法。

![2022-11-09 15-28-57 的屏幕截图](/home/xulei/图片/2022-11-09 15-28-57 的屏幕截图.png)