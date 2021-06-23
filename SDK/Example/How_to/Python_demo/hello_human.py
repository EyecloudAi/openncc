#!/usr/bin/env python3

from __future__ import print_function
import sys
import os
from array import array
import cv2
import numpy as np
import time
import openncc as ncc
import struct

min_score = 0.8
media_head ='iII13I'

def  coordinate_is_valid( x1,  y1,  x2,  y2):
    if((x1<0) or (x1>1)):
        return False
    if((y1<0) or (y1>1)):
        return False
    if((x2<0) or (x2>1)):
        return False
    if((y2<0) or (y2>1)):
        return False
    if((x1>=x2) or (y1>=y2)):
        return False
    #print("coordinate_is_valid true")    
    return True   

def print_hex(bytes):
    l = [hex(int(i)) for i in bytes]
    print(" ".join(l))
    
def get_header_info(spec):
	frames=struct.unpack(media_head,bytes(spec))
	return frames[0],frames[1],frames[2]
	
def main():
	res = ncc.load_fw("./moviUsbBoot","fw/flicRefApp.mvcmd")
	if res<0: 
		printf('load firmware error!')
		sys.exit(1)

	print("get usb %d sdk versin %s" % (ncc.get_usb_version() ,ncc.get_sdk_version()))
		
	sensors=ncc.CameraSensor()
	sensor1 = ncc.SensorModesConfig()
	if sensors.GetFirstSensor(sensor1)==0:
				print("camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n" % (
                sensor1.moduleName, sensor1.camWidth, sensor1.camHeight, sensor1.camFps,
                sensor1.AFmode, sensor1.maxEXP, sensor1.minGain, sensor1.maxGain))	

	sensor2 = ncc.SensorModesConfig() 
	while sensors.GetNextSensor(sensor2)==0:
				print("camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n" % (
                sensor2.moduleName, sensor2.camWidth, sensor2.camHeight, sensor2.camFps,
                sensor2.AFmode, sensor2.maxEXP, sensor2.minGain, sensor2.maxGain))		
    
	ncc.camera_select_sensor(0)    #0 1080p 1 4k
	cameraCfg = sensor1
  
	cam_info=ncc.CameraInfo()
	cam_info.inputFormat=ncc.IMG_FORMAT_BGR_PLANAR
	#cam_info.meanValue = [float(0.0)]*3 
	cam_info.stdValue=1
	
	cam_info.isOutputYUV=1
	cam_info.isOutputH26X=1
	cam_info.isOutputJPEG=1
	
	cam_info.imageWidth  = cameraCfg.camWidth
	cam_info.imageHeight = cameraCfg.camHeight
	cam_info.startX      = 0
	cam_info.startY      = 0
	cam_info.endX        = cameraCfg.camWidth
	cam_info.endY        = cameraCfg.camHeight
	cam_info.inputDimWidth =0
	cam_info.inputDimHeight =0
	ncc.SetMeanValue(cam_info,0.0,0.0,0.0)
  
	ret = ncc.sdk_init(None, None, "./blob/2020.3/human-pose-estimation-0001/human-pose-estimation-0001.blob",cam_info, 0) #struct CameraInfo,0 will auto cal
	#metasize=ncc.get_meta_size()#only support one module now
	#print("xlink_init ret=%d  %d" % (ret,metasize))
	#if (ret<0):
	#	return 
	 
	oft_x = cam_info.startX
	oft_y = cam_info.startY
	dis_w = cam_info.endX - cam_info.startX
	dis_h = cam_info.endY - cam_info.startY
	
	offset=struct.calcsize(media_head) 
	size=cameraCfg.camWidth*cameraCfg.camHeight*2		
	yuvbuf = bytearray(size+offset)
	metabuf =  bytearray(300*1024) 	
		
	ncc.camera_video_out(ncc.YUV420p,ncc.VIDEO_OUT_CONTINUOUS)			
	while(True):							
		size = ncc.GetYuvData(yuvbuf)		
		if (size<= 0):
			time.sleep(0.1)     #0.1 second
			continue
		
		numarry = np.array(yuvbuf[offset:size])    #skip head frameSpecOut 64 bytes 
		#print("buf   len=%d/%d" % (numarry.size,size))	
		yuv = numarry.reshape((int(cameraCfg.camHeight*3/2), cameraCfg.camWidth))
	
		size = ncc.GetMetaData(metabuf)			
		if (size>0):
			#spec=metabuf[0:offset]
			#print(spec)
			#type,seq,size=get_header_info(metabuf[0:offset])
			#print("meta type=%d seq=%d  size=%d" % (type,seq,size))
			
			barray = metabuf[offset+ncc.OUTPUT_INDEX_SIZE:size]			
			print("meta len=%d/%d" % (len(barray),size))
			#print_hex(barray[0:14])
			count=len(barray)//2
			sarry = struct.unpack('H'*count,barray)
			#print(sarry[0:7])
			
			farry = array('f')	
			for i in range(count):
				farry.append(ncc.f16Tof32(sarry[i]))  #get all float data from output
			
			dim_paf=[1, 38, 32, 57]     
			dim_heat=[1, 19 , 32 , 57]
			paf_len=dim_paf[0]*dim_paf[1]*dim_paf[2]*dim_paf[3]
			
			print("farry len=%d paf_len =%d" % (len(farry),paf_len))
			
			pafs_arry = np.array(farry[0:paf_len])		
			heat_arry = np.array(farry[paf_len:])		
			print("pafs array len=%d heat array len=%d" %(pafs_arry.size,heat_arry.size))
			pafs_blob=pafs_arry.reshape(dim_paf)
			print("pafs_blob:")
			print(pafs_blob)
			heat_blob=heat_arry.reshape(dim_heat)
			print("heat_blob:")
			print(heat_blob)
	ncc.sdk_uninit()   
	
if __name__ == '__main__':
    sys.exit(main() or 0)
