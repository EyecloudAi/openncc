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
import math

min_score = 0.8
media_head = 'iII13I'

def coordinate_is_valid(x1, y1, x2, y2):
    if ((x1 < 0) or (x1 > 1)):
        return False
    if ((y1 < 0) or (y1 > 1)):
        return False
    if ((x2 < 0) or (x2 > 1)):
        return False
    if ((y2 < 0) or (y2 > 1)):
        return False
    if ((x1 >= x2) or (y1 >= y2)):
        return False
    # print("coordinate_is_valid true")
    return True


def print_hex(bytes):
    l = [hex(int(i)) for i in bytes]
    print(" ".join(l))


def get_header_info(spec):
    frames = struct.unpack(media_head, bytes(spec))
    return frames[0], frames[1], frames[2]


def calc_similar(face_vec, standardemb):
	dot_out = 0
	vec_sum = 0
	for i in range(256):
		vec_sum += face_vec[i] * face_vec[i];

	vec_sum = math.sqrt(vec_sum);
	for i in range(256):
		face_vec[i] = face_vec[i] / vec_sum;

	dot_out = 0;
	for i in range(256):
		dot_out += face_vec[i] * standardemb[i]

	return dot_out

faceLib = [
-0.054260,
0.968262,
-0.499023,
1.067383,
1.729492,
-1.036133,
0.794922,
-0.025360,
-0.373535,
1.203125,
0.597656,
-0.909668,
0.729980,
-2.687500,
1.304688,
-0.976562,
0.211182,
-0.124268,
-0.927734,
-0.179321,
-1.613281,
-0.815918,
-1.208008,
0.757812,
0.097229,
-1.232422,
1.401367,
0.678223,
0.511230,
-2.101562,
0.104736,
0.359131,
-1.122070,
-0.223022,
-0.830078,
0.831055,
0.697266,
-0.326172,
-1.934570,
1.367188,
-0.572754,
-0.148193,
-1.887695,
0.146118,
0.627930,
-2.746094,
-1.091797,
-0.540039,
-1.600586,
-0.174561,
-1.806641,
1.315430,
0.875977,
2.404297,
-1.711914,
-0.583984,
-0.020615,
-1.338867,
0.907227,
-1.554688,
1.161133,
1.010742,
-0.660645,
0.286865,
-0.551758,
-0.156006,
0.717285,
0.762695,
0.582031,
-0.655273,
1.076172,
-0.012634,
-1.116211,
-0.598633,
-0.724121,
-0.318848,
-0.865234,
-0.186768,
-1.458008,
0.981445,
0.033752,
0.625000,
1.940430,
-0.059143,
0.468262,
1.219727,
0.780762,
-1.144531,
0.183228,
0.107300,
0.169678,
1.334961,
-0.533691,
0.596680,
1.090820,
0.819824,
-1.291992,
-0.210083,
-1.312500,
1.328125,
0.074402,
1.840820,
1.423828,
-0.952148,
-0.435547,
-1.138672,
1.110352,
0.064575,
1.963867,
0.922852,
1.352539,
0.506836,
0.667969,
0.379883,
0.270996,
0.404297,
1.996094,
0.586426,
-0.577148,
-0.348633,
-0.183472,
2.597656,
1.267578,
0.114990,
-0.166992,
-0.837891,
0.866699,
1.536133,
-0.417236,
-0.618652,
-0.160522,
0.309082,
1.122070,
0.682129,
1.201172,
0.149536,
-0.988281,
-0.074097,
-0.661133,
-1.432617,
0.964355,
2.667969,
0.714844,
0.419434,
-0.346191,
-0.835938,
-0.358154,
0.773926,
0.934082,
1.106445,
-0.321289,
0.235962,
0.033875,
0.051910,
-0.373291,
0.143188,
0.878418,
0.281494,
-0.912598,
0.174072,
-0.836426,
0.053772,
-0.829590,
-0.257080,
0.883789,
-0.397949,
-0.842285,
-0.030838,
0.337158,
-0.343262,
-0.094788,
0.832031,
-0.095398,
-0.973633,
-1.229492,
1.606445,
-0.383301,
-0.528809,
-0.077332,
-0.737305,
-1.667969,
-0.333252,
1.216797,
-0.499756,
1.037109,
-0.812012,
2.025391,
-0.271240,
0.761719,
0.614746,
0.728027,
0.122437,
0.248779,
-0.465820,
0.947266,
-0.069214,
-0.401611,
-0.135864,
-0.512695,
1.193359,
-0.024048,
-0.530273,
1.072266,
0.209839,
1.353516,
0.825684,
-0.003265,
1.353516,
0.270020,
-0.050385,
-1.509766,
0.148193,
-0.539551,
0.088074,
0.062073,
-0.111877,
-1.545898,
-1.327148,
-1.704102,
0.880859,
-1.027344,
-0.336182,
-0.052551,
1.624023,
-0.321045,
0.809082,
0.913574,
-0.441162,
0.329590,
0.252197,
0.077759,
-0.316406,
-1.588867,
-2.382812,
-0.415283,
-0.748535,
1.193359,
-0.019989,
0.984375,
-1.216797,
1.408203,
-1.426758,
-1.748047,
-1.165039,
-0.017349,
1.116211,
0.912598,
1.005859,
0.542480,
-0.289551,
0.886230,
-0.002472,
-1.326172,
-1.346680,
0.936035,
-1.043945,
]
SIMI_VAL=7.0
def main():
    global  faceLib
    res = ncc.load_fw("./moviUsbBoot", "fw/flicRefApp.mvcmd")
    if res < 0:
        printf('load firmware error!')
        sys.exit(1)

    print("get usb %d sdk versin %s" % (ncc.get_usb_version(), ncc.get_sdk_version()))

    print("get fw version: %s and ncc id %s" % (ncc.camera_get_fw_version(),
                                                ncc.camera_get_ncc_id()))

    sensors = ncc.CameraSensor()
    sensor1 = ncc.SensorModesConfig()
    if sensors.GetFirstSensor(sensor1) == 0:
        print("camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n" % (
            sensor1.moduleName, sensor1.camWidth, sensor1.camHeight, sensor1.camFps,
            sensor1.AFmode, sensor1.maxEXP, sensor1.minGain, sensor1.maxGain))

    sensor2 = ncc.SensorModesConfig()
    while sensors.GetNextSensor(sensor2) == 0:
        print("camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n" % (
            sensor2.moduleName, sensor2.camWidth, sensor2.camHeight, sensor2.camFps,
            sensor2.AFmode, sensor2.maxEXP, sensor2.minGain, sensor2.maxGain))

    ncc.camera_select_sensor(0)  # 0 1080p 1 4k
    cameraCfg = sensor1

    net1_info = ncc.Network1Par()
    net1_info.inputFormat = ncc.IMG_FORMAT_BGR_PLANAR
    net1_info.meanValue = [0.0, 0.0, 0.0]
    net1_info.stdValue = 1

    net1_info.isOutputYUV = 1
    net1_info.isOutputH26X = 0
    net1_info.isOutputJPEG = 0

    net1_info.imageWidth = cameraCfg.camWidth
    net1_info.imageHeight = cameraCfg.camHeight
    net1_info.startX = 0
    net1_info.startY = 0
    net1_info.endX = cameraCfg.camWidth
    net1_info.endY = cameraCfg.camHeight
    net1_info.inputDimWidth = 300
    net1_info.inputDimHeight = 300

    # extInputs = np.zeros(ncc.MAX_EXTINPUT_SIZE,dtype = np.uint8)
    # print('input size {}'.format(extInputs.size))
    net1_info.extInputs = [0] * ncc.MAX_EXTINPUT_SIZE  # tobytes()
    net1_info.modelCascade = 1
    net1_info.inferenceACC = 0

    net2_info = ncc.Network2Par()
    net2_info.startXAdj = -5
    net2_info.startYAdj = -5
    net2_info.endXAdj = 5
    net2_info.endYAdj = 5

    labelMask = [0]*ncc.MAX_LABEL_SIZE
    labelMask[1]=1
    net2_info.labelMask = labelMask
    net2_info.minConf = 0.7

    net2_info.inputDimWidth = 128
    net2_info.inputDimHeight = 128
    net2_info.inputFormat = ncc.IMG_FORMAT_BGR_PLANAR

    net2_info.meanValue = [0.0, 0.0, 0.0]
    net2_info.stdValue = 1

    count=ncc.MAX_EXTINPUT_SIZE//2
    extInputs = array('h',[0]*count)
    net2_info.extInputs = extInputs.tobytes()
    net2_info.modelCascade = 0
    #print("input  0={} 1={} type={}".format(int(ncc.f32Tof16(1.0)),extInputs[1],type(extInputs[1])));
    blob1 = "./blob/2020.3/face-detection-retail-0004/face-detection-retail-0004.blob";
    blob2 = "./blob/2020.3/face-reidentification-retail-0095/face-reidentification-retail-0095.blob";
    ret = ncc.sdk_net2_init(None, None, \
                            blob1, net1_info, 0, \
                            blob2, net2_info, 0)
    metasize = 3 * 1024 * 1024
    print("xlink_init ret=%d  %d" % (ret, metasize))
    if (ret < 0):
        return

    oft_x = net1_info.startX
    oft_y = net1_info.startY
    dis_w = net1_info.endX - net1_info.startX
    dis_h = net1_info.endY - net1_info.startY

    offset = struct.calcsize(media_head)  # 64
    size = cameraCfg.camWidth * cameraCfg.camHeight * 2
    yuvbuf = bytearray(size + offset)
    metabuf = bytearray(metasize)

    ncc.camera_video_out(ncc.YUV420p, ncc.VIDEO_OUT_CONTINUOUS)
    bmeta=False
    bsave_face=False
    if os.access("face.txt", os.F_OK):
    	faceLib=np.loadtxt('face.txt', dtype=float).tolist()
    	print(faceLib)
    	
    while (True):
        size = ncc.GetYuvData(yuvbuf)
        if (size <= 0):
            time.sleep(0.1)  # 0.1 second
            continue

        numarry = np.array(yuvbuf[offset:size])  # skip head frameSpecOut 64 bytes
        # print("buf   len=%d/%d" % (numarry.size,size))
        yuv = numarry.reshape((int(cameraCfg.camHeight * 3 / 2), cameraCfg.camWidth))
        bgr = cv2.cvtColor(yuv, cv2.COLOR_YUV2BGR_I420, 3)

        size, outsize0,outsize1,item_num = ncc.GetMetaDataExt(metabuf)
        if (size > 0)and (item_num>0):
            bmeta=True
            print("ret=%d size0=%d size1=%d,num=%d" %(size,outsize0,outsize1,item_num ))
            # spec=metabuf[0:offset]
            #type1,seq,m_size=get_header_info(metabuf[0:offset])
            #print("meta type=%d seq=%d  size=%d" % (type1,seq,m_size))

            #f = open("sample.txt", "wb")
            #f.write(metabuf)
            #f.close()

            from_i=offset + ncc.OUTPUT_INDEX_SIZE
            count=outsize0//2
            detMetadata = struct.unpack('H'*count,metabuf[from_i:from_i+outsize0])

            from_i+=outsize0   #skip output 0
            count=outsize1//2
            print("out2 count %d from %d to %d" % (count,from_i,size))
            secondMeta = struct.unpack('H'*count*item_num,metabuf[from_i:size])

            for i in range(item_num):
                image_id = int(ncc.f16Tof32(detMetadata[i * 7 + 0]))
                if (image_id < 0):
                    break

                label = int(ncc.f16Tof32(detMetadata[i * 7 + 1]))
                score = ncc.f16Tof32(detMetadata[i * 7 + 2])
                x0 = ncc.f16Tof32(detMetadata[i * 7 + 3])
                y0 = ncc.f16Tof32(detMetadata[i * 7 + 4])
                x1 = ncc.f16Tof32(detMetadata[i * 7 + 5])
                y1 = ncc.f16Tof32(detMetadata[i * 7 + 6])
               # print("item sise=%d score:%.2f<->min:%.2f  rec:(%.3f,%.3f)<->(%.3f,%.3f) " %(item_num,score,min_score,x0,y0,x1,y1))
                if ((not coordinate_is_valid(x0, y0, x1, y1)) or (score < min_score) or (labelMask[label]==0)):
                    continue

                x = int(x0 * dis_w + oft_x)
                y = int(y0 * dis_h + oft_y)
                w = int((x1 - x0) * dis_w)
                h = int((y1 - y0) * dis_h)

                cv2.rectangle(bgr, (x, y), (x + w, y + h), (255, 128, 128), 2)
                regMetadata=secondMeta[i*count:i*count+count]
                
                regRet = array('f')
                              	
                for j in range(256):
                	regRet.append(ncc.f16Tof32(regMetadata[j]))
                
                if 	bsave_face:
                		print("!!!!save face!!!!")
                		cv2.imwrite('face.png', bgr)
                		nf=np.array(regRet) 
                		np.savetxt('face.txt', np.array(nf), fmt='%.2f')
                		bsave_face=False
                		
                similarVal = calc_similar(regRet, faceLib)
                result = ''
                print("get similar {:.2f}".format(similarVal))
                if similarVal > SIMI_VAL:
                	result = 'RET{:2.2f}, OK'.format(similarVal);
                else:
                	result = 'RET{:2.2f}, NG'.format(similarVal);
                cv2.putText(bgr, result, (x, y - 20), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255), 1)

        img_scaled = cv2.resize(bgr, None, fx=0.7, fy=0.7, interpolation=cv2.INTER_CUBIC)
        cv2.namedWindow('openncc', cv2.WINDOW_AUTOSIZE)
        cv2.imshow('openncc', img_scaled)
        c=cv2.waitKey(10)
        if c==115:    #s
        	bsave_face=True
        elif ( c== 27):
            break
    ncc.sdk_uninit()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    sys.exit(main() or 0)
