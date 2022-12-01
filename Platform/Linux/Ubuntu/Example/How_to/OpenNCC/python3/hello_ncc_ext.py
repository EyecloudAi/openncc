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
	
	print("get fw version: %s and ncc id %s" % (ncc.camera_get_fw_version() ,
					ncc.camera_get_ncc_id()))
		
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
  
	#cam_info=ncc.CameraInfo()
	cam_info=ncc.Network1Par()
	cam_info.inputFormat=ncc.IMG_FORMAT_BGR_PLANAR
	cam_info.meanValue = [0.0,0.0,0.0]
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
	
	#extInputs = np.zeros(ncc.MAX_EXTINPUT_SIZE,dtype = np.uint8)
	#print('input size {}'.format(extInputs.size))
	cam_info.extInputs=[0]*ncc.MAX_EXTINPUT_SIZE     #tobytes()     
	cam_info.modelCascade=0
	cam_info.inferenceACC=1
  
	ret = ncc.sdk_init_ex(None, None, "./blob/2020.3/face-detection-retail-0004/face-detection-retail-0004.blob",cam_info, struct.calcsize("13I4f")) #struct CameraInfo
	metasize=ncc.get_meta_size()
	print("xlink_init ret=%d  %d" % (ret,metasize))
	if (ret<0):
		return 
	 
	oft_x = cam_info.startX
	oft_y = cam_info.startY
	dis_w = cam_info.endX - cam_info.startX
	dis_h = cam_info.endY - cam_info.startY
	
	
	offset=struct.calcsize(media_head) 
	size=cameraCfg.camWidth*cameraCfg.camHeight*2		
	yuvbuf = bytearray(size+offset)
	metabuf =  bytearray(metasize+offset) 	
		
	ncc.camera_video_out(ncc.YUV420p,ncc.VIDEO_OUT_CONTINUOUS)			
	while(True):							
		size = ncc.GetYuvData(yuvbuf)		
		if (size<= 0):
			time.sleep(0.1)     #0.1 second
			continue
		
		numarry = np.array(yuvbuf[offset:size])    #skip head frameSpecOut 64 bytes 
		#print("buf   len=%d/%d" % (numarry.size,size))	
		yuv = numarry.reshape((int(cameraCfg.camHeight*3/2), cameraCfg.camWidth))
		bgr = cv2.cvtColor(yuv, cv2.COLOR_YUV2BGR_I420, 3)	
		
		size = ncc.GetMetaData(metabuf)			
		if (size>0):
			#spec=metabuf[0:offset]
			#print(spec)
			#type,seq,size=get_header_info(metabuf[0:offset])
			#print("meta type=%d seq=%d  size=%d" % (type,seq,size))
			
			barray = metabuf[offset+ncc.OUTPUT_INDEX_SIZE:size]			
			#print("meta len=%d/%d" % (len(barray),size))
			#print_hex(barray[0:14])
			count=len(barray)//2
			sarry = struct.unpack('H'*count,barray)
			#print(sarry[0:7])
			
			for i in range(100):
				image_id = ncc.f16Tof32(sarry[i*7+0])
				#print("face id %d" % image_id)
				if (image_id < 0):
					break
				
				score =ncc.f16Tof32(sarry[i*7+2])				
				x0 = ncc.f16Tof32(sarry[i*7+3])							
				y0 = ncc.f16Tof32(sarry[i*7+4])
				x1 = ncc.f16Tof32(sarry[i*7+5])
				y1 = ncc.f16Tof32(sarry[i*7+6])
				#print("score:%.2f<->min:%.2f  rec:(%.3f,%.3f)<->(%.3f,%.3f) " %(score,min_score,x0,y0,x1,y1))
				if((not coordinate_is_valid(x0, y0, x1, y1)) or (score < min_score)):
					continue
	
				x = int(x0 * dis_w + oft_x)
				y = int(y0 * dis_h + oft_y)
				w  = int((x1 - x0) * dis_w)
				h = int((y1 - y0) * dis_h)
				cv2.rectangle(bgr, (x,y), (x+w,y+h), (0, 255, 0), 2)

				result=("score:%d")%(int(100*score))
				#print("%d,%d[%dx%d] score:%s" %(x,y,w,h,result))
				cv2.putText(bgr, result,(x,y+32), cv2.FONT_HERSHEY_COMPLEX, 1, (255, 0, 0), 1)		
			
		img_scaled = cv2.resize(bgr,None,fx=0.7, fy=0.7, interpolation = cv2.INTER_CUBIC)
		cv2.namedWindow('openncc', cv2.WINDOW_AUTOSIZE)
		cv2.imshow('openncc',img_scaled)
		if (cv2.waitKey(20)==27): break
	ncc.sdk_uninit()
	cv2.destroyAllWindows()	    
	
if __name__ == '__main__':
    sys.exit(main() or 0)
