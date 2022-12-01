(window.webpackJsonp=window.webpackJsonp||[]).push([[9],{367:function(e,t,n){"use strict";n.r(t);var o=n(44),r=Object(o.a)({},(function(){var e=this,t=e.$createElement,n=e._self._c||t;return n("ContentSlotsDistributor",{attrs:{"slot-key":e.$parent.slotKey}},[n("h2",{attrs:{id:"preparatory-work"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#preparatory-work"}},[e._v("#")]),e._v(" Preparatory work")]),e._v(" "),n("h3",{attrs:{id:"download-software-development-kit-sdk"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#download-software-development-kit-sdk"}},[e._v("#")]),e._v(" Download Software Development Kit(SDK)")]),e._v(" "),n("ul",[n("li",[e._v("Go to "),n("a",{attrs:{href:"https://www.openncc.com/",target:"_blank",rel:"noopener noreferrer"}},[e._v("official website"),n("OutboundLink")],1)]),e._v(" "),n("li",[e._v("Click "),n("strong",[e._v("Documentation")])])]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/website_support.png",alt:"img"}})]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/website_download.png",alt:"image-20210702134237102"}})]),e._v(" "),n("ul",[n("li",[e._v("Download the zip file or copy the address and use Git to clone it.")])]),e._v(" "),n("p",[e._v("After download completed, go to "),n("code",[e._v("openncc/Platform")]),e._v("，the directory is as follows：")]),e._v(" "),n("div",{staticClass:"language- extra-class"},[n("pre",{pre:!0,attrs:{class:"language-text"}},[n("code",[e._v("└─ Platform\n\t├─ Linux\n\t|   └─ NVIDIA\n\t|   └─ RaspberryPi\n\t|   └─ Ubuntu\n\t├─ Windows\n\t├─ Custom\n\t├─ README.md\n")])])]),n("p",[e._v("Select the required file directory to enter, the corresponding folder contains the scripts for environment building.")]),e._v(" "),n("hr"),e._v(" "),n("p",[n("font",{attrs:{color:"red"}},[e._v("Warning: The environment builder script will automatically generate and overwrite related files, please make sure it is the first time you run it or have completed a backup before running it.")])],1),e._v(" "),n("hr"),e._v(" "),n("h2",{attrs:{id:"quick-start-to-linux"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#quick-start-to-linux"}},[e._v("#")]),e._v(" Quick Start to Linux")]),e._v(" "),n("h3",{attrs:{id:"environment-construction"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#environment-construction"}},[e._v("#")]),e._v(" Environment Construction")]),e._v(" "),n("ul",[n("li",[e._v("Go to the directory"),n("code",[e._v("opennccc/Platform/Linux/Ubuntu")]),e._v(".")]),e._v(" "),n("li",[e._v("Right click to open the terminal.")]),e._v(" "),n("li",[e._v("Enter the command "),n("code",[e._v("./build_ubuntu.sh")]),e._v(".")])]),e._v(" "),n("p",[e._v("The generated directory is as follows.：")]),e._v(" "),n("div",{staticClass:"language- extra-class"},[n("pre",{pre:!0,attrs:{class:"language-text"}},[n("code",[e._v("└─ Ubuntu\n\t├─ Example\n\t├─ Source\n\t├─ Viewer\n\t├─ build_Ubuntu.sh\n\t├─ README.md\n")])])]),n("h3",{attrs:{id:"openncc-linux-operation-demo"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#openncc-linux-operation-demo"}},[e._v("#")]),e._v(" OpenNCC_Linux Operation Demo")]),e._v(" "),n("ul",[n("li",[n("p",[e._v("Go to the directory: openncc/Platform/Linux/Ubuntu/Viewer/OpenNcc_Linux.")])]),e._v(" "),n("li",[n("p",[e._v("Right-click to open the terminal and execute "),n("code",[e._v("sudo ./AppRun")]),e._v("l to start the software.")])]),e._v(" "),n("li",[n("p",[e._v("Connect the OpenNCC camera to the USB 3.0 port of your computer and click on the Get device info button to get the device information, at this point there will be two prompts in the log area.：")]),e._v(" "),n("p",[e._v("- USB3.0：USB interface is 3.0, yuv outflow is currently available。")]),e._v(" "),n("p",[e._v("- USB2.0：USB interface is not 3.0, yuv outflow will be disabled。")])])]),e._v(" "),n("hr"),e._v(" "),n("p",[n("font",{attrs:{color:"red"}},[e._v("Tip: The OpenNCC TYPE-C interface has two modes of positive and negative access, corresponding to USB3.0 and USB2.0 respectively. The USB2.0 mode temporarily disables the YUV420P format video stream output due to the transfer rate.")])],1),e._v(" "),n("hr"),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view1.png",alt:"image-20210702142441399"}})]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view2.png",alt:"image-20210702142529686"}})]),e._v(" "),n("p",[e._v("​\t\tIf you need to switch USB mode, rotate the TPYE-C port to access it again and click the Get device info button again. If there is still no change, please check your computer USB port type or "),n("a",{attrs:{href:"https://www.openncc.com/contact",target:"_blank",rel:"noopener noreferrer"}},[e._v("contact us"),n("OutboundLink")],1),e._v(".")]),e._v(" "),n("ul",[n("li",[n("p",[e._v("Choose any of the video streaming formats yuv420p/H.264/H.265/mjpeg.")])]),e._v(" "),n("li",[n("p",[n("code",[e._v("Stream Resolution")]),e._v("：Two resolutions are available, 1080p and 4K.(Specifically determined by OpenNCC product type)")])])]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view3.png",alt:"image-20210702142649007"}})]),e._v(" "),n("ul",[n("li",[n("code",[e._v("1st network model")]),e._v("：Select the algorithm model.")])]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view4.png",alt:"img"}})]),e._v(" "),n("p",[e._v("Currently supports more than 10 models to choose. If choose None that will not load models, only the original video stream could be displayed.")]),e._v(" "),n("ul",[n("li",[n("p",[e._v("The algorithm area can be restricted by boxing the ROI region, and the algorithm will only recognize scenes within the region.(See the [running result](#_Running result) demonstration for details)")])]),e._v(" "),n("li",[n("p",[n("code",[e._v("2nd network model")]),e._v("：Select the secondary algorithm model.")])])]),e._v(" "),n("p",[e._v("Example model：vehicle-license-plate-detection-barrier-0106-fp16")]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view5.png",alt:"img"}})]),e._v(" "),n("ul",[n("li",[n("p",[n("code",[e._v("Model Score")]),e._v(" ：Set the minimum score for the algorithm to recognize and reach the threshold before the recognition result will be framed in the screen.")])]),e._v(" "),n("li",[n("p",[n("code",[e._v("Display Scaler")]),e._v(" ： Set the video display window size, and you can adjust the display window resolution.")])]),e._v(" "),n("li",[n("p",[n("code",[e._v("Exposure control")]),e._v("：Set the exposure, you can choose Auto and Manual, when you choose Manual, you can set your own Exposure times and iso.")])])]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view6.png",alt:"img"}})]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view7.png",alt:"image-20210702143055575"}})]),e._v(" "),n("ul",[n("li",[e._v("Switch "),n("code",[e._v("show state")]),e._v("，l  Select whether to display current status information on the screen, including video stream frame rate, algorithm frame rate, resolution, and device id.")]),e._v(" "),n("li",[e._v("Switch "),n("code",[e._v("inference accelerate")]),e._v("，Select whether to enable algorithm acceleration. (Must be selected before loading the algorithm model).")])]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view8.png",alt:"img"}})]),e._v(" "),n("ul",[n("li",[e._v("Switch  "),n("code",[e._v("save_avi")]),e._v("，l  The video will be saved to the directory     openncc/Platform/Linux/Viewer/OpenNcc_Linux/avi folder with the time named avi file after opening the video stream. When the video stream is closed, the video will stop saving. (yuv420p does not have this item)")])]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/linux_view9.png",alt:"img"}})]),e._v(" "),n("ul",[n("li",[e._v("Click  "),n("code",[e._v("Start running models")]),e._v(", Open the video stream. See [running result](# Running result) for running results")])]),e._v(" "),n("h2",{attrs:{id:"quick-start-to-windows"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#quick-start-to-windows"}},[e._v("#")]),e._v(" Quick Start to Windows")]),e._v(" "),n("h3",{attrs:{id:"environment-construction-2"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#environment-construction-2"}},[e._v("#")]),e._v(" Environment Construction")]),e._v(" "),n("ul",[n("li",[n("p",[e._v("Go to openncc/Platform/Windows.")])]),e._v(" "),n("li",[n("p",[e._v("Double-click to run "),n("code",[e._v("build_windows.bat")]),e._v(".")])]),e._v(" "),n("li",[n("p",[e._v("The results are as follows：")])])]),e._v(" "),n("div",{staticClass:"language- extra-class"},[n("pre",{pre:!0,attrs:{class:"language-text"}},[n("code",[e._v("└─ Windows\n\t├─ Source\n\t├─ Viewer\n\t├─ build_windows.sh\n\t├─ README.md\n")])])]),n("ul",[n("li",[e._v("After the script runs successfully, go to the openncc/Platform/Windows/Drivers directory and install the Windows USB driver. Detailed installation steps can be found in "),n("font",{attrs:{color:"blue"}},[e._v("OpenNCC_USB_Driver_install_guide_win.pdf")]),e._v("。")],1)]),e._v(" "),n("h3",{attrs:{id:"openncc-windows-operation-demo"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#openncc-windows-operation-demo"}},[e._v("#")]),e._v(" OpenNCC_Windows Operation Demo")]),e._v(" "),n("ul",[n("li",[e._v("Go to the directory: openncc/Platform/Windows/Viewer/OpenNcc_Windows.")]),e._v(" "),n("li",[e._v("Double-click to run "),n("code",[e._v("OpenNCC.exe")]),e._v(".")]),e._v(" "),n("li",[e._v("Following steps are the same as "),n("a",{attrs:{href:"#OpenNCC_Linux%E6%93%8D%E4%BD%9C%E6%BC%94%E7%A4%BA"}},[e._v("OpenNCC_Linux")])])]),e._v(" "),n("h2",{attrs:{id:"quick-start-to-raspberry-pi"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#quick-start-to-raspberry-pi"}},[e._v("#")]),e._v(" Quick Start to Raspberry Pi")]),e._v(" "),n("h3",{attrs:{id:"environment-construction-3"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#environment-construction-3"}},[e._v("#")]),e._v(" Environment Construction")]),e._v(" "),n("ul",[n("li",[n("p",[e._v("Open terminal.")])]),e._v(" "),n("li",[n("p",[e._v("Go to openncc/Platform/Linux/RaspberryPi.")])]),e._v(" "),n("li",[n("p",[e._v("Enter command "),n("code",[e._v("./build_raspberrypi.sh")]),e._v(".")])])]),e._v(" "),n("p",[e._v("After running successfully, the following directory will be generated:")]),e._v(" "),n("div",{staticClass:"language- extra-class"},[n("pre",{pre:!0,attrs:{class:"language-text"}},[n("code",[e._v("└── RaspberryPi\n\t├── Example\n\t├── Source\n\t├── Viewer\n\t├── build_raspberrypi.sh\n\t├── README.md\n")])])]),n("h3",{attrs:{id:"openncc-raspberry-operation-demo"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#openncc-raspberry-operation-demo"}},[e._v("#")]),e._v(" OpenNCC_Raspberry Operation Demo")]),e._v(" "),n("hr"),e._v(" "),n("p",[n("font",{attrs:{color:"red"}},[e._v("Tip: Before running OpenNCC_Raspberry, please check the device number and "),n("a",{attrs:{href:"https://www.openncc.com/contact",target:"_blank",rel:"noopener noreferrer"}},[e._v("contact us"),n("OutboundLink")],1),e._v(" for the key, see openncc/Platform/Raspberry/ReadMe.md for details.")])],1),e._v(" "),n("hr"),e._v(" "),n("ul",[n("li",[e._v("Copy the key file (eyecloud.key) to the directory: openncc/Platform/Linux/RaspberryPi/Viewer/OpenNcc_Raspberry/Configuration/fw.")]),e._v(" "),n("li",[e._v("Go back to the directory:  openncc/Platform/Linux/RaspberryPi/Viewer/OpenNcc_Raspberry.")]),e._v(" "),n("li",[e._v("Open a terminal and execute "),n("code",[e._v("sudo ./AppRun")]),e._v("to start the software.")]),e._v(" "),n("li",[e._v("Following steps are the same as "),n("a",{attrs:{href:"#OpenNCC_Linux%E6%93%8D%E4%BD%9C%E6%BC%94%E7%A4%BA"}},[e._v("OpenNCC_Linux")]),e._v("。")])]),e._v(" "),n("h2",{attrs:{id:"quick-start-to-nvidia"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#quick-start-to-nvidia"}},[e._v("#")]),e._v(" Quick Start to NVIDIA")]),e._v(" "),n("h3",{attrs:{id:"environment-construction-4"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#environment-construction-4"}},[e._v("#")]),e._v(" Environment Construction")]),e._v(" "),n("ul",[n("li",[n("p",[e._v("Open terminal.")])]),e._v(" "),n("li",[n("p",[e._v("Go to openncc/Platform/Linux/NVIDIA.")])]),e._v(" "),n("li",[n("p",[e._v("Enter command "),n("code",[e._v("./build_nvidia.sh.")])])])]),e._v(" "),n("p",[e._v("After running successfully, the following directory will be generated:")]),e._v(" "),n("div",{staticClass:"language- extra-class"},[n("pre",{pre:!0,attrs:{class:"language-text"}},[n("code",[e._v("└─ NVIDIA\n\t├─ Example\n\t├─ Source\n\t├─ build_nvidia.sh\n\t├─ README.md\n")])])]),n("h2",{attrs:{id:"custom-customization"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#custom-customization"}},[e._v("#")]),e._v(" Custom（Customization）")]),e._v(" "),n("h3",{attrs:{id:"environment-construction-5"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#environment-construction-5"}},[e._v("#")]),e._v(" Environment construction")]),e._v(" "),n("ul",[n("li",[n("p",[e._v("Go to openncc/Platform/Custom.")])]),e._v(" "),n("li",[n("p",[e._v("Right click to open the terminal.")])]),e._v(" "),n("li",[n("p",[e._v("Enter command "),n("code",[e._v("./custom.sh")])])])]),e._v(" "),n("hr"),e._v(" "),n("p",[n("font",{attrs:{color:"red"}},[e._v("Tip: To run on a custom platform, please "),n("a",{attrs:{href:"https://www.openncc.com/contact",target:"_blank",rel:"noopener noreferrer"}},[e._v("contact us"),n("OutboundLink")],1),e._v(" for customization services.")])],1),e._v(" "),n("hr"),e._v(" "),n("h2",{attrs:{id:"running-result"}},[n("a",{staticClass:"header-anchor",attrs:{href:"#running-result"}},[e._v("#")]),e._v(" Running result")]),e._v(" "),n("p",[e._v("Take the object classification algorithm model as an example.：")]),e._v(" "),n("p",[e._v("Banana results in the algorithm area：")]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/bannna.png",alt:"image-20210702145523142"}})]),e._v(" "),n("p",[e._v("Apple results in the algorithm area：")]),e._v(" "),n("p",[n("img",{attrs:{src:"/openncc/docimg/ch/apple.png",alt:"image-20210702145621696"}})])])}),[],!1,null,null,null);t.default=r.exports}}]);