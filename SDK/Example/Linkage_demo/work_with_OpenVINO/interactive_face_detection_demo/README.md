1. Copy the OpenNCC Demo to OpenVINO
1.1. Remove the original "interactive_face_detection_demo" file from OpenVINO.
You can find it in this path: /opt/intel/openvino/inference_engine/demos

1.2. Copy the "interactive_face_detection_demo" file from the OpenNCC file to OpenVINO file.
Its new path in OpenVINO: /opt/intel/openvino/inference_engine/demos/

[Command Refrence] 
sudo cp -a /home/jerry/Desktop/OPENNCC/ncc_cdk/Samples/Demo/work\ with\ OpenVINO/interactive_face_detection_demo  /opt/intel/openvino/inference_engine/demos/

2. Compile
2.2. Enter the below command：

cd /opt/intel/openvino/inference_engine/demos/

sudo ./build_samples.sh

if no error, you find hint like this:
Build completed, you can find binaries for all demos in the /home/dukezuo/omz_demos_build/intel64/Release subfolder.

2.3. Check the compile resut: enter "~/omz_demos_build/intel64/Release", if there is "interactive_face_detection_demo"，compile succeeded.

3. Setup Run-environment in OpenVINO

3.1. Copy the file named "module" from OpenNCC CDK to OpenVINO.
Its original path in OpenNCC CDK: ncc_cdk/Samples/Demo/work with OpenVINO/Debug
Its new path in OpenVINO: OpenVINO's run category~/omz_demos_build/intel64

3.2. Copy the firmware release file named "fw" from OpenNCC CDK to OpenVINO.
Its original path in OpenNCC CDK: ncc_cdk/Samples/Demo/work with OpenVINO/Debug
Its new path in OpenVINO: OpenVINO's run category~/omz_demos_build/intel64

3.4. Enter ncc_cdk/Tools/deployment, type in "sudo ./install_NCC_udev_rules.sh" to get visiting access for the USB device.

3.5 Restart the operation system.

4. Run command
4.1. Enter OpenVINO's run category ~/omz_demos_build/intel64/Release/
4.2. Type in the below command to run the Demo:
./interactive_face_detection_demo -i cam -m ../module/face-detection-adas-0001.xml -m_ag ../module/age-gender-recognition-retail-0013.xml -m_hp ../module/head-pose-estimation-adas-0001.xml -m_em ../module/emotions-recognition-retail-0003.xml -m_lm ../module/facial-landmarks-35-adas-0002.xml -d CPU

