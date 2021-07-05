// Copyright (C) 2018-2019 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

# pragma once

#include <gflags/gflags.h>
#include <functional>
#include <iostream>
#include <fstream>
#include <random>
#include <memory>
#include <chrono>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>
#include <map>

#include <inference_engine.hpp>

#include <samples/common.hpp>
#include <samples/slog.hpp>

#include <ie_iextension.h>

#include <opencv2/opencv.hpp>

// -------------------------Generic routines for detection networks-------------------------------------------------

struct BaseDetection {
    InferenceEngine::ExecutableNetwork net;
    InferenceEngine::InferRequest::Ptr request;
    std::string topoName;
    std::string pathToModel;
    std::string deviceForInference;
    const size_t maxBatch;
    bool isBatchDynamic;
    const bool isAsync;
    mutable bool enablingChecked;
    mutable bool _enabled;
    const bool doRawOutputMessages;

    BaseDetection(const std::string &topoName,
                  const std::string &pathToModel,
                  const std::string &deviceForInference,
                  int maxBatch, bool isBatchDynamic, bool isAsync,
                  bool doRawOutputMessages);

    virtual ~BaseDetection();

    InferenceEngine::ExecutableNetwork* operator ->();
    virtual InferenceEngine::CNNNetwork read(const InferenceEngine::Core& ie) = 0;
    virtual void submitRequest();
    virtual void wait();
    bool enabled() const;
    void printPerformanceCounts(std::string fullDeviceName);
};

struct FaceDetection : BaseDetection {
    struct Result {
        int label;
        float confidence;
        cv::Rect location;
    };

    std::string input;
    std::string output;
    std::string labels_output;
    double detectionThreshold;
    int maxProposalCount;
    int objectSize;
    int enquedFrames;
    float width;
    float height;
    float network_input_width;
    float network_input_height;
    float bb_enlarge_coefficient;
    float bb_dx_coefficient;
    float bb_dy_coefficient;
    bool resultsFetched;
    std::vector<Result> results;

    FaceDetection(const std::string &pathToModel,
                  const std::string &deviceForInference,
                  int maxBatch, bool isBatchDynamic, bool isAsync,
                  double detectionThreshold, bool doRawOutputMessages,
                  float bb_enlarge_coefficient, float bb_dx_coefficient,
                  float bb_dy_coefficient);

    InferenceEngine::CNNNetwork read(const InferenceEngine::Core& ie) override;
    void submitRequest() override;

    void enqueue(const cv::Mat &frame);
    void fetchResults();
};

//add by duke 2020.3.17
#include "sdk.h"
#include "cameraCtrl.h"
#include "Fp16Convert.h"
#include <opencv2/imgproc/types_c.h>

struct NccFaceDetection : FaceDetection {		
	public:	
    NccFaceDetection(const std::string &pathToModel,
                  const std::string &deviceForInference,
                  int maxBatch, bool isBatchDynamic, bool isAsync,
                  double detectionThreshold, bool doRawOutputMessages,
                  float bb_enlarge_coefficient, float bb_dx_coefficient,
                  float bb_dy_coefficient):FaceDetection(pathToModel,deviceForInference,maxBatch,
                  	isBatchDynamic,isAsync,detectionThreshold,doRawOutputMessages,
                  	bb_enlarge_coefficient,bb_dx_coefficient,bb_dy_coefficient)
    	{	
		std::string blobfile = fileNameNoExt(pathToModel) + ".blob";
		std::cout<<blobfile<<std::endl;
		CameraInfo fd_nn_par =
		{
			-1,  //imageWidth
			-1,  //imageHeight                  
				-1,  //startX
			-1,  //startY                 
			-1,   //endX
			-1,   //endY                  
			0,  //inputDimWidth
			0,   //inputDimHeight                   /* <dim>300</dim>  <dim>300</dim> */
			IMG_FORMAT_BGR_PLANAR,      //IMAGE_FORMAT   
			0,                    //meanValue
			0,
			0,
			1,	//stdValue
			1,	/*enable YUV420 output*/
			1,	/*enable H26X output*/
			1,	/*enalbe MJPE output*/
			ENCODE_H264_MODE,/* H264 encoder*/
		};

		/*1 load firware*/
		int ret = load_fw("../fw/moviUsbBoot","../fw/flicRefApp.mvcmd");
		if (ret < 0)
		{
			printf("init device error! return \n");
			return;
		}

		/* 2. get camera param */
		SensorModesConfig cameraCfg;
		SensorModesList   list;
		camera_control_get_features(&list);
		printf("list num:%d\n",list.num);
		for(int i=0;i<list.num;i++)
		{
			SensorModesConfig features;
			memcpy(&features, &list.mode[i], sizeof(features));
			printf("[%d/%d]camera: %s, %dX%d@%dfps, AFmode:%d, maxEXP:%dus,gain[%d, %d]\n",i,list.num,
				features.moduleName, features.camWidth, features.camHeight, features.camFps,
				features.AFmode, features.maxEXP, features.minGain, features.maxGain);
		}

		int sensorModeId = 0; //0:1080P 1:4k  
		camera_select_sensor(sensorModeId);
		memcpy(&cameraCfg, &list.mode[sensorModeId], sizeof(cameraCfg));//select camera info
		
		/* 3. set area  for detect */
		fd_nn_par.imageWidth = cameraCfg.camWidth;
		fd_nn_par.imageHeight = cameraCfg.camHeight;
		fd_nn_par.startX = 0;
		fd_nn_par.startY = 0;
		fd_nn_par.endX = cameraCfg.camWidth;
		fd_nn_par.endY = cameraCfg.camHeight;

		width = cameraCfg.camWidth;
		height = cameraCfg.camHeight;

		ret = sdk_init(NULL, NULL, (char*)blobfile.c_str(), &fd_nn_par, sizeof(fd_nn_par));
		printf("xlink_init %d  cam %dX%d\n", ret,width,height);
		if (ret < 0) return;
	
 		camera_video_out(YUV420p,VIDEO_OUT_CONTINUOUS);
    	};

	virtual ~NccFaceDetection()
	{
		sdk_uninit();
	};       
	
	void enqueue(const cv::Mat &frame) 
	{	
		//printf("****%s****\n",__FUNCTION__);
		return;
	};

	bool ReadVideo(cv::Mat& frame)
	{
		int size = sizeof(yuv420p);
		if (read_yuv_data(yuv420p, &size, 1) < 0)
			return false ;
		cv::Mat yuvImg;
		yuvImg.create(height * 3 / 2, width, CV_8UC1);	
		yuvImg.data = (unsigned char*)yuv420p + sizeof(frameSpecOut);
		//int bufLen = height*width*3/2; 
		//char* pYuvBuf =  yuv420p + sizeof(frameSpecOut);
		//memcpy(frame.data, pYuvBuf,bufLen); 
		cv::cvtColor(yuvImg, frame, CV_YUV2BGR_I420);
		return true;
	};		
	
	virtual void submitRequest() 
	{

		//printf("****%s****\n",__FUNCTION__);
		return;
	};
	virtual void wait()
	{
		//printf("****%s****\n",__FUNCTION__);
		return;
	}
	void get_cam_size(int & w,int & h)
	{
		w = width;
		h = height;
	};
	
	void fetchResults()
	{
		//if (!enabled()) return;
		int size = sizeof(metadata);
		int ret = read_meta_data(metadata, &size, 0);
		if (ret<0) 
		return;

		results.clear();
		//if (resultsFetched) return;
		resultsFetched = true;
		//printf("****%s****\n",__FUNCTION__);
		//const float *detections = request->GetBlob(output)->buffer().as<float *>();

		//printf("read read_meta_data size=%d success!\n",size);

		frameSpecOut *out = (frameSpecOut *)metadata;
		uint16_t* cls_ret = (uint16_t*)(metadata+sizeof(frameSpecOut)+OUTPUT_INDEX_SIZE);
		size -= sizeof(frameSpecOut)+OUTPUT_INDEX_SIZE;	
		printf("read read_meta_data size=%d out size=%d success!\n",out->size,size);
	
		for (int i = 0; i < size/2; i++) 
			detections[i]=f16Tof32(cls_ret[i]);
				
		for (int i = 0; i < maxProposalCount; i++) 
		{		
			float image_id = detections[i * objectSize + 0];
			if (image_id < 0) {
			    break;
			}
			Result r;
			r.label = static_cast<int>(detections[i * objectSize + 1]);
			r.confidence = detections[i * objectSize + 2];

			if (r.confidence <= detectionThreshold && !doRawOutputMessages) {
			    continue;
			}

	
			if (!coordinate_is_valid(detections[i * objectSize + 3],detections[i * objectSize + 4],
						detections[i * objectSize + 5],detections[i * objectSize + 6])) //add by duke 2020.3.18
			{
				//printf("invalid metadata x1:%f y1:%f x2:%f y2:%f ,ignore \n",detections[i * objectSize + 3],detections[i * objectSize + 4],
				//		detections[i * objectSize + 5],detections[i * objectSize + 6]);
				continue;
			}

			r.location.x = static_cast<int>(detections[i * objectSize + 3] * width);
			r.location.y = static_cast<int>(detections[i * objectSize + 4] * height);
			r.location.width = static_cast<int>(detections[i * objectSize + 5] * width - r.location.x);
			r.location.height = static_cast<int>(detections[i * objectSize + 6] * height - r.location.y);


			// Make square and enlarge face bounding box for more robust operation of face analytics networks
			int bb_width = r.location.width;
			int bb_height = r.location.height;

			int bb_center_x = r.location.x + bb_width / 2;
			int bb_center_y = r.location.y + bb_height / 2;

			int max_of_sizes = std::max(bb_width, bb_height);

			int bb_new_width = static_cast<int>(bb_enlarge_coefficient * max_of_sizes);
			int bb_new_height = static_cast<int>(bb_enlarge_coefficient * max_of_sizes);

			r.location.x = bb_center_x - static_cast<int>(std::floor(bb_dx_coefficient * bb_new_width / 2));
			r.location.y = bb_center_y - static_cast<int>(std::floor(bb_dy_coefficient * bb_new_height / 2));

			r.location.width = bb_new_width;
			r.location.height = bb_new_height;

			if (doRawOutputMessages) {
			    std::cout << "[" << i << "," << r.label << "] element, prob = " << r.confidence <<
					 "    (" << r.location.x << "," << r.location.y << ")-(" << r.location.width << ","
				      << r.location.height << ")"
				      << ((r.confidence > detectionThreshold) ? " WILL BE RENDERED!" : "") << std::endl;
			}
			if (r.confidence > detectionThreshold) {
			    results.push_back(r);
			}
	    }			
	};
	private:
		bool coordinate_is_valid(float x1, float y1, float x2, float y2)
		{
		     if((x1<0) || (x1>1))
			return false;
		     if((y1<0) || (y1>1))
			return false;
		    if((x2<0) || (x2>1))
			return false;
		    if((y2<0) || (y2>1))
			return 0;
		    if((x1>=x2) || (y1>=y2))
			return false;
		    return true;
		}

		int width;
		int height;
		char metadata[1024*5];
		float detections[50*7];//max 50 face
		char yuv420p[sizeof(frameSpecOut) + 1920 * 1080 * 2];		  		    
};
//add end

struct AgeGenderDetection : BaseDetection {
    struct Result {
        float age;
        float maleProb;
    };

    std::string input;
    std::string outputAge;
    std::string outputGender;
    size_t enquedFaces;

    AgeGenderDetection(const std::string &pathToModel,
                       const std::string &deviceForInference,
                       int maxBatch, bool isBatchDynamic, bool isAsync,
                       bool doRawOutputMessages);

    InferenceEngine::CNNNetwork read(const InferenceEngine::Core& ie) override;
    void submitRequest() override;

    void enqueue(const cv::Mat &face);
    Result operator[] (int idx) const;
};

struct HeadPoseDetection : BaseDetection {
    struct Results {
        float angle_r;
        float angle_p;
        float angle_y;
    };

    std::string input;
    std::string outputAngleR;
    std::string outputAngleP;
    std::string outputAngleY;
    size_t enquedFaces;
    cv::Mat cameraMatrix;

    HeadPoseDetection(const std::string &pathToModel,
                      const std::string &deviceForInference,
                      int maxBatch, bool isBatchDynamic, bool isAsync,
                      bool doRawOutputMessages);

    InferenceEngine::CNNNetwork read(const InferenceEngine::Core& ie) override;
    void submitRequest() override;

    void enqueue(const cv::Mat &face);
    Results operator[] (int idx) const;
};

struct EmotionsDetection : BaseDetection {
    std::string input;
    std::string outputEmotions;
    size_t enquedFaces;

    EmotionsDetection(const std::string &pathToModel,
                      const std::string &deviceForInference,
                      int maxBatch, bool isBatchDynamic, bool isAsync,
                      bool doRawOutputMessages);

    InferenceEngine::CNNNetwork read(const InferenceEngine::Core& ie) override;
    void submitRequest() override;

    void enqueue(const cv::Mat &face);
    std::map<std::string, float> operator[] (int idx) const;

    const std::vector<std::string> emotionsVec = {"neutral", "happy", "sad", "surprise", "anger"};
};

struct FacialLandmarksDetection : BaseDetection {
    std::string input;
    std::string outputFacialLandmarksBlobName;
    size_t enquedFaces;
    std::vector<std::vector<float>> landmarks_results;
    std::vector<cv::Rect> faces_bounding_boxes;

    FacialLandmarksDetection(const std::string &pathToModel,
                             const std::string &deviceForInference,
                             int maxBatch, bool isBatchDynamic, bool isAsync,
                             bool doRawOutputMessages);

    InferenceEngine::CNNNetwork read(const InferenceEngine::Core& ie) override;
    void submitRequest() override;

    void enqueue(const cv::Mat &face);
    std::vector<float> operator[] (int idx) const;
};

struct Load {
    BaseDetection& detector;

    explicit Load(BaseDetection& detector);

    void into(InferenceEngine::Core & ie, const std::string & deviceName, bool enable_dynamic_batch = false) const;
};

class CallStat {
public:
    typedef std::chrono::duration<double, std::ratio<1, 1000>> ms;

    CallStat();

    double getSmoothedDuration();
    double getTotalDuration();
    double getLastCallDuration();
    void calculateDuration();
    void setStartTime();

private:
    size_t _number_of_calls;
    double _total_duration;
    double _last_call_duration;
    double _smoothed_duration;
    std::chrono::time_point<std::chrono::high_resolution_clock> _last_call_start;
};

class Timer {
public:
    void start(const std::string& name);
    void finish(const std::string& name);
    CallStat& operator[](const std::string& name);

private:
    std::map<std::string, CallStat> _timers;
};
