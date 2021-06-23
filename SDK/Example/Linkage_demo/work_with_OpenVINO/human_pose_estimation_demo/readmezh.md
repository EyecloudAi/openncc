1. 复制OpenNCC Demo到OpenVINO
1.1. 检查并清除原来OpenVINO里的"human_pose_estimation_demo" 文件。
您可以通过这个路径里进行检查: opt/intel/openvino/inference_engine/samples/
1.2. 复制OpenNCC文件包里新的 "human_pose_estimation_demo" 文件到 OpenVINO。
它应该放在OpenVINO的路径: /opt/intel/openvino/inference_engine/samples/

[参考命令] 
sudo cp -a ./ncc_cdk/Samples/Demo/work\ with\ OpenVINO/  /opt/intel/openvino/inference_engine/samples/

2. 编译
2.1. 编辑OpenVINO“build_samples.sh”脚本文件：把“cmake -DCMAKE_BUILD_TYPE=Release”改成“cmake -DCMAKE_BUILD_TYPE=Debug”.
2.2. 输入下述命令：

cd /opt/intel/openvino/inference_engine/samples/

sudo ./build_samples.sh

2.3. 检查编译结果: 输入 "~/inference_engine_samples_build/intel64/Debug", 如果有 "human_pose_estimation_demo"文件，说明编译成功。

3. 设置OpenVINO运行环境

3.1. 从OpenNCC CDK复制 "module"这个文件到OpenVINO.
OpenNCC CDK里的路径: ncc_cdk/Samples/Demo/work with OpenVINO/Debug
OpenVINO里的路径: OpenVINO's run category~/inference_engine_samples_build/intel64

3.2. 复制OpenNCC CDK里的USB下载文件"moviUsbBoot" 到OpenVINO。
OpenNCC CDK的路径: ncc_cdk/Samples/bin
OpenVINO的路径: OpenVINO's run category~/inference_engine_samples_build/intel64/Debug

4.3. 复制OpenVINO CDK的固件发布文件 "fw"到OpenVINO。
OpenNCC CDK的路径: samples/bin
OpenVINO的路径: OpenVINO's run category~/inference_engine_samples_build/intel64/Debug

4.4. 进入 ncc_cdk/Tools/deployment, 输入 "sudo ./install_NCC_udev_rules.sh" ，获取USB设备的访问权限。

4.5 重启Linux操作系统。

5. 运行
5.1. 进入 OpenVINO的运行目录~/inference_engine_samples_build/intel64/Debug
5.2. 输入以下命令符，即可跑姿态预测模型:
./human_pose_estimation_demo -i cam -m ../module/human-pose-estimation-0001.xml -d CPU
