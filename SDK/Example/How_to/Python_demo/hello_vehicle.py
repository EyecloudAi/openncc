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

def main():
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
    net1_info.isOutputH26X = 1
    net1_info.isOutputJPEG = 1

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
    net1_info.inferenceACC = 1

    net2_info = ncc.Network2Par()
    net2_info.startXAdj = -5
    net2_info.startYAdj = -5
    net2_info.endXAdj = 5
    net2_info.endYAdj = 5

    labelMask = [0]*ncc.MAX_LABEL_SIZE
    labelMask[2]=1
    net2_info.labelMask = labelMask
    net2_info.minConf = 0.7

    net2_info.inputDimWidth = 94
    net2_info.inputDimHeight = 24
    net2_info.inputFormat = ncc.IMG_FORMAT_BGR_PLANAR

    net2_info.meanValue = [0.0, 0.0, 0.0]
    net2_info.stdValue = 1

    count=ncc.MAX_EXTINPUT_SIZE//2
    extInputs = array('h',[0]*count)
    extInputs[0] = int(ncc.f32Tof16(0))
    for i in range(1, 88):
        extInputs[i] = int(ncc.f32Tof16(1.0))
    net2_info.extInputs = extInputs.tobytes()
    net2_info.modelCascade = 0
    print("input  0={} 1={} type={}".format(int(ncc.f32Tof16(1.0)),extInputs[1],type(extInputs[1])));
    blob1 = "./blob/2020.3/vehicle-license-plate-detection-barrier-0106/vehicle-license-plate-detection-barrier-0106.blob";
    blob2 = "./blob/2020.3/license-plate-recognition-barrier-0001/license-plate-recognition-barrier-0001.blob";  # if par_Len=0 , cal param_size auto
    ret = ncc.sdk_net2_init(None, None, \
                            blob1, net1_info, 0, \
                            blob2, net2_info, 0)
    metasize = 2 * 1024 * 1024
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
    bmeta=False;
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
        #if(bmeta):
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
            #print("out1 count %d from %d to %d" % (count,from_i,size))
            secondMeta= struct.unpack('H'*count*item_num,metabuf[from_i:size])

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
                
                regRet = array('i')
                for j in range(88):
                    regRet.append(int(ncc.f16Tof32(regMetadata[j])))

                items = [
                    "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", \
                    "<Anhui>", "<Beijing>", "<Chongqing>", "<Fujian>", \
                    "<Gansu>", "<Guangdong>", "<Guangxi>", "<Guizhou>", \
                    "<Hainan>", "<Hebei>", "<Heilongjiang>", "<Henan>", \
                    "<HongKong>", "<Hubei>", "<Hunan>", "<InnerMongolia>", \
                    "<Jiangsu>", "<Jiangxi>", "<Jilin>", "<Liaoning>", \
                    "<Macau>", "<Ningxia>", "<Qinghai>", "<Shaanxi>", \
                    "<Shandong>", "<Shanghai>", "<Shanxi>", "<Sichuan>", \
                    "<Tianjin>", "<Tibet>", "<Xinjiang>", "<Yunnan>", \
                    "<Zhejiang>", "<police>", \
                    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", \
                    "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", \
                    "U", "V", "W", "X", "Y", "Z" \
                    ];

                result = ''
                for j in range(0, len(regRet)):
                    if (regRet[j] == -1):
                        break
                    #result = result.join(items[regRet[j]])
                    result = result+items[regRet[j]]
                cv2.putText(bgr, result, (x, y - 20), cv2.FONT_HERSHEY_COMPLEX, 1, (0, 0, 255), 1)

        img_scaled = cv2.resize(bgr, None, fx=0.7, fy=0.7, interpolation=cv2.INTER_CUBIC)
        cv2.namedWindow('openncc', cv2.WINDOW_AUTOSIZE)
        cv2.imshow('openncc', img_scaled)
        if (cv2.waitKey(20) == 27):
            break
    ncc.sdk_uninit()
    cv2.destroyAllWindows()

if __name__ == '__main__':
    sys.exit(main() or 0)
