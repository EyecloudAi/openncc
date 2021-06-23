1 support over python3.5

2 python interface file openncc.py£¬python run shared file _openncc.so

3 example file 
hello_ncc.py: test for one ai net module
hello_ncc_ext.py: test for one ai net module and for 2 acc speed up 
hello_vehicle.py: test for 2 ai net module.

4 run method: 
1)copy run relation files from how_to_use_sdk to current:
cp -a ../how_to_use_sdk/bin/blob .
cp -a ../how_to_use_sdk/bin/fw .

2) run test file
sudo python3 hello_ncc.py

and also we support human_pose_estimation example,just print the output data from ai module
when run : sudo python3 hello_human.py

and also we support human_pose_estimation example,just print the output data from ai module
when run : sudo python3 hello_human.py

hello_face.py it is a recognize example for 2 ai net
run :sudo python3 hello_face.py
tab key 's' save grasp face to a png and vector file 'face.txt'
if you run again(tab ESC exit),it will recognized the new face


