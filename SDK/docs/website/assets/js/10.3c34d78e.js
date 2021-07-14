(window.webpackJsonp=window.webpackJsonp||[]).push([[10],{386:function(e,t,o){"use strict";o.r(t);var v=o(44),d=Object(v.a)({},(function(){var e=this,t=e.$createElement,o=e._self._c||t;return o("ContentSlotsDistributor",{attrs:{"slot-key":e.$parent.slotKey}},[o("h2",{attrs:{id:"overview"}},[o("a",{staticClass:"header-anchor",attrs:{href:"#overview"}},[e._v("#")]),e._v(" Overview")]),e._v(" "),o("p",[e._v(" OpenNCC View is a software used to quickly experience the OpenNCC development kit . The default model under OpenNCC View does not depend on OpenVINO. OpenNCC View integrates all APIs of NCC SDK, realizes connection to OpenNCC, downloads of firmware and AI models, video stream display, and post-processing of algorithm results. Users can operate and control the camera through a friendly interface.\n"),o("img",{attrs:{src:"/openncc/docimg/view_figure1.png",alt:"Figure-1"}})]),e._v(" "),o("h2",{attrs:{id:"model-analysis"}},[o("a",{staticClass:"header-anchor",attrs:{href:"#model-analysis"}},[e._v("#")]),e._v(" Model analysis")]),e._v(" "),o("p",[e._v(" After the model is deduced by OpenNCC, the real-time deduction result is obtained through the OpenNCC CDK API. OpenNCC View implements a general parser for the deduction result for Object Detection Models."),o("br"),e._v("\n OpenNCC View Post-inference processing supports output in the following formats:")]),e._v(" "),o("ul",[o("li",[e._v("The output data form:[1, 1, N, 7]")]),e._v(" "),o("li",[e._v("N is the number of object frames detected in the current frame")]),e._v(" "),o("li",[e._v("For each detection frame ,contains the following information format:")]),e._v(" "),o("li",[e._v("[image_id, label, conf, x_min, y_min, x_max, y_max]:")]),e._v(" "),o("li",[e._v("image_id - ID of the image in the batch")]),e._v(" "),o("li",[e._v("label - predicted class ID")]),e._v(" "),o("li",[e._v("conf - confidence for the predicted class")]),e._v(" "),o("li",[e._v("(x_min, y_min) - coordinates of the top left bounding box corner")]),e._v(" "),o("li",[e._v("(x_max, y_max) - coordinates of the bottom right bounding box corner."),o("br"),e._v("\n Users can train their own models and define the output layer according to the above format, and can use the model import function of OpenNCC View to add object detection models without programming. If users need to add models in other output formats,You need to refer to the demo in ncc_cdk/Samples/How_to/load a model and write post-processing code in combination with your own application scenario.")])]),e._v(" "),o("h2",{attrs:{id:"functions-detail"}},[o("a",{staticClass:"header-anchor",attrs:{href:"#functions-detail"}},[e._v("#")]),e._v(" Functions detail")]),e._v(" "),o("ol",[o("li",[e._v("Get Device Info：")])]),e._v(" "),o("p",[e._v(" Obtain the USB connection information of the device and the computer, and the NCC device module information. If the connected device supports it, you can unlock more functional options. (For example, if NCC is connected to a computer via USB 3.0, you can unlock the yuv output format; if NCC is equipped with a 4K module, you can unlock the 4K resolution display)")]),e._v(" "),o("p",[e._v("2）Stream Format：")]),e._v(" "),o("p",[e._v(" Choose NCC camera USB video output format, currently supports YUV420P, H.264, MJPEG format. (You need to click Get Device Info before selecting YUV420P, which is only available when USB 3.0 is connected)")]),e._v(" "),o("p",[e._v("3）Stream Resolution：")]),e._v(" "),o("p",[e._v(" Change the video resolution of NCC camera, 1080P or 4K can be selected. 4K resolution support requires optional 4K camera module. (Get Device Info before selecting 4K, only available if the module supports 4K)")]),e._v(" "),o("p",[e._v("4）1st network model：")]),e._v(" "),o("p",[e._v(" Select the algorithm model, select None to not load the model, and only display the video stream. After selecting the algorithm model to load, you can select the ROI area by frame to identify only the scenes in the area.")]),e._v(" "),o("p",[e._v("5）Model Score：")]),e._v(" "),o("p",[e._v(" After loading the model, the recognition calculation result of the item is scored. When it exceeds a certain score threshold, it will be framed in the video, and the Model Score is the option to control the threshold. The minimum score of the recognition can be adjusted according to user needs. (The default value is 0.5)")]),e._v(" "),o("p",[e._v("6）ROI：")]),e._v(" "),o("p",[e._v(" Used in conjunction with the algorithm model. After loading the model, if you need to perform algorithm identification only for a certain area, you can manually select the area of interest and only identify the scenes in the area.")]),e._v(" "),o("p",[e._v("7）Display Scaler：")]),e._v(" "),o("p",[e._v(" When the video stream is displayed, due to the different resolutions of different computers, if the 1080P or 4K video is displayed at the original size, the video size may exceed the desktop size. The user can control the video display window size in real time through the Display Scaler (default value 0.5)")]),e._v(" "),o("p",[e._v("8）Add model：")]),e._v(" "),o("p",[e._v(' Click "add model" to import the generated .blob model file. Note here that you need to put the .blob file and the corresponding IR file (.xml&&.bin) in the same location and with the same file name. Refer to the “OpenNCC Software Manual.docx” for generating .blob files. After adding, you can select the corresponding model in 1st network.')]),e._v(" "),o("p",[e._v("9）Del model：")]),e._v(" "),o("p",[e._v(" Delete the model file imported by the user, and it will be removed in the 1st network model after deletion")]),e._v(" "),o("p",[e._v("10）Start running model：")]),e._v(" "),o("p",[e._v(" Click to start loading firmware and model, and display video window according to user's options"),o("br"),e._v("\n11）Log：")]),e._v(" "),o("p",[e._v(" Display the log of OoenNCC DK working, the user can view the running status of the device, and the prompt information about the abnormal state of the device")]),e._v(" "),o("p",[e._v("12）Algo Results：")]),e._v(" "),o("p",[e._v(" The user chooses to load the algorithm model, and the algorithm recognizes an object in the scene, the algorithm result will be printed here (such as the coordinate information of the identified model relative to the current window, and the recognition score calculated by the algorithm, etc. )")]),e._v(" "),o("h2",{attrs:{id:"algorithms-speed-up-test-results"}},[o("a",{staticClass:"header-anchor",attrs:{href:"#algorithms-speed-up-test-results"}},[e._v("#")]),e._v(" Algorithms speed up test results")]),e._v(" "),o("table",[o("thead",[o("tr",[o("th",[e._v("Version")]),e._v(" "),o("th",[e._v("AR0234 60Hz 1080P 3.0usb")]),e._v(" "),o("th",[e._v("YUV")]),e._v(" "),o("th",[e._v("YUV")]),e._v(" "),o("th",[e._v("H.264")]),e._v(" "),o("th",[e._v("H.264")]),e._v(" "),o("th",[e._v("MJPEG")]),e._v(" "),o("th",[e._v("MJPEG")])])]),e._v(" "),o("tbody",[o("tr",[o("td"),e._v(" "),o("td",[e._v("Number of Acceleration Engines")]),e._v(" "),o("td",[e._v("2")]),e._v(" "),o("td",[e._v("1")]),e._v(" "),o("td",[e._v("2")]),e._v(" "),o("td",[e._v("1")]),e._v(" "),o("td",[e._v("2")]),e._v(" "),o("td",[e._v("1")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("Model")]),e._v(" "),o("td",[e._v("AI FPS")]),e._v(" "),o("td",[e._v("AI FPS")]),e._v(" "),o("td",[e._v("AI FPS")]),e._v(" "),o("td",[e._v("AI FPS")]),e._v(" "),o("td",[e._v("AI FPS")]),e._v(" "),o("td",[e._v("AI FPS")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("classification-fp16")]),e._v(" "),o("td",[e._v("25")]),e._v(" "),o("td",[e._v("14")]),e._v(" "),o("td",[e._v("25")]),e._v(" "),o("td",[e._v("14")]),e._v(" "),o("td",[e._v("25")]),e._v(" "),o("td",[e._v("14")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("face-detection-retail-0004-fp16")]),e._v(" "),o("td",[e._v("80")]),e._v(" "),o("td",[e._v("43")]),e._v(" "),o("td",[e._v("80")]),e._v(" "),o("td",[e._v("55")]),e._v(" "),o("td",[e._v("80")]),e._v(" "),o("td",[e._v("52")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("face-detection-adas-0001-fp16")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("7")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("8")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("8")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("person-detection-retail-0013-fp16")]),e._v(" "),o("td",[e._v("12")]),e._v(" "),o("td",[e._v("5")]),e._v(" "),o("td",[e._v("12")]),e._v(" "),o("td",[e._v("6")]),e._v(" "),o("td",[e._v("12")]),e._v(" "),o("td",[e._v("6")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("person-vehicle-bike-detection-crossroad-0078-fp16")]),e._v(" "),o("td",[e._v("6")]),e._v(" "),o("td",[e._v("2")]),e._v(" "),o("td",[e._v("6")]),e._v(" "),o("td",[e._v("3")]),e._v(" "),o("td",[e._v("6")]),e._v(" "),o("td",[e._v("3")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("face-person-detection-retail-0002-fp16")]),e._v(" "),o("td",[e._v("10")]),e._v(" "),o("td",[e._v("4.5")]),e._v(" "),o("td",[e._v("10")]),e._v(" "),o("td",[e._v("5")]),e._v(" "),o("td",[e._v("10")]),e._v(" "),o("td",[e._v("5")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("pedestrian-detection-adas-0002-fp16")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("7")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("9")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("9")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("vehicle-detection-adas-0002-fp16")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("8")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("9")]),e._v(" "),o("td",[e._v("15")]),e._v(" "),o("td",[e._v("9")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("vehicle-license-plate-detection-barrier-0106-fp16")]),e._v(" "),o("td",[e._v("45")]),e._v(" "),o("td",[e._v("24")]),e._v(" "),o("td",[e._v("45")]),e._v(" "),o("td",[e._v("28")]),e._v(" "),o("td",[e._v("45")]),e._v(" "),o("td",[e._v("29")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("license-plate-recognition-barrier-0001")]),e._v(" "),o("td",[e._v("40")]),e._v(" "),o("td",[e._v("24")]),e._v(" "),o("td",[e._v("40")]),e._v(" "),o("td",[e._v("23")]),e._v(" "),o("td",[e._v("40")]),e._v(" "),o("td",[e._v("23")])]),e._v(" "),o("tr",[o("td"),e._v(" "),o("td",[e._v("pedestrian-and-vehicle-detector-adas-0001-fp16")]),e._v(" "),o("td",[e._v("12")]),e._v(" "),o("td",[e._v("7")]),e._v(" "),o("td",[e._v("12")]),e._v(" "),o("td",[e._v("7")]),e._v(" "),o("td",[e._v("12")]),e._v(" "),o("td",[e._v("7")])])])])])}),[],!1,null,null,null);t.default=d.exports}}]);