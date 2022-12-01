
Face detection model compilation:
./myriad_compile -m face-detection-retail-0004-fp16.xml -o face-detection-retail-0004-fp16.blob  -VPU_PLATFORM VPU_2480 -VPU_NUMBER_OF_SHAVES 8 -VPU_NUMBER_OF_CMX_SLICES 8

./myriad_compile -m face-detection-adas-0001-fp16.xml -o face-detection-adas-0001-fp16.blob  -VPU_PLATFORM VPU_2480 -VPU_NUMBER_OF_SHAVES 8 -VPU_NUMBER_OF_CMX_SLICES 8

./myriad_compile -m person-detection-retail-0002-fp16.xml -o person-detection-retail-0002-fp16.blob  -VPU_PLATFORM VPU_2480 -VPU_NUMBER_OF_SHAVES 8 -VPU_NUMBER_OF_CMX_SLICES 8

//////////////////////////////////////////////////////////////NG///////////////////////////////////////////////////////
./myriad_compile -m person-detection-retail-0002-fp16.xml -o person-detection-retail-0002-fp16.blob -VPU_PLATFORM VPU_2480 -VPU_NUMBER_OF_SHAVES 8 -VPU_NUMBER_OF_CMX_SLICES 8 -iop "data:U8, im_info:FP32" -op FP32 

./myriad_compile -m person-detection-retail-0013-fp16.xml -o person-detection-retail-0013-fp16.blob -VPU_PLATFORM VPU_2480 -VPU_NUMBER_OF_SHAVES 8 -VPU_NUMBER_OF_CMX_SLICES 8 


Object classification model compilation:
./myriad_compile -m cls.xml -o cls.blob  -VPU_PLATFORM VPU_2480 -VPU_NUMBER_OF_SHAVES 8 -VPU_NUMBER_OF_CMX_SLICES 8

./myriad_compile -m person-vehicle-bike-detection-crossroad-0078-fp16.xml -o person-vehicle-bike-detection-crossroad-0078-fp16.blob -VPU_PLATFORM VPU_2480 -VPU_NUMBER_OF_SHAVES 8 -VPU_NUMBER_OF_CMX_SLICES 8 

