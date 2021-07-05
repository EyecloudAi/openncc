### Q: Are other versions of openvino supported?
* In addition to 2020.3.194 is officially supported by openncc, other new versions of openvino can also be supported by openncc, but there is no complete coverage evaluation, and some models run abnormally.

### Q：How to ensure that openncc camera has firmware algorithm acceleration enabled?  
* First of all, only openncc camera with ar0234 module can enable firmware dual engine algorithm acceleration function. With openncc view software, it can be activated directly by checking the "inference acceleration" box. If developers need to enable firmware algorithm acceleration in their own programs, they can set the last member of the structure network1par, inferenceacc, to 1 when the SDK is initialized. In addition to observing the frame rate change, you can also test the higher voltage confirmation at the USB interface when accelerating.  

### Q: What is the effect of firmware dual engine algorithm acceleration？  
* Firmware dual engine algorithm acceleration has a good effect on all models in openncc view, most of which can improve the performance by nearly 100%. 0004 face recognition model can be stable at about 80 frames in the test process, but it can only reach 43 frames before accelerating. In addition, the models that developers call based on openncc software development framework can be used correctly.


### Q: What deep learning frameworks and models does openvino support?
*   Caffe* (most public branches)  
    TensorFlow*  
    MXNet*  
    Kaldi*  
    ONNX*  
more：[Supported Frameworks and Formats ](https://docs.openvinotoolkit.org/2020.3/_docs_IE_DG_Introduction.html)


### Q: Does openncc have a hardware version that supports booting from Flash?
* After the development is completed, if the deployment requires openncc to run independently from the SOC, and there is a mass production version with flash, please consult openncc.

### Q: I have technical problems with model transformation and parsing. Where can I get support?
* Intel ·OpenVINO community could help you.

### Q: Does openncc support developers to change lenses themselves?
* For general applications, developers can choose different shots according to their own scenes. If the image quality can not meet the strict requirements of the scene after lens replacement, you can seek technical customization support from openncc.  


### Q: Does openncc support the development of Ubuntu virtual machine?
* Yes, the USB device of openncc is detected by the virtual machine when it is started. If it is detected by the host first, and then manually switched to the virtual opportunity, the communication exception that the openncc development interface retrieval device fails occurs.
