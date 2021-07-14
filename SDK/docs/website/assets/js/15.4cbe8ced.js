(window.webpackJsonp=window.webpackJsonp||[]).push([[15],{371:function(e,t,a){"use strict";a.r(t);var o=a(44),n=Object(o.a)({},(function(){var e=this,t=e.$createElement,a=e._self._c||t;return a("ContentSlotsDistributor",{attrs:{"slot-key":e.$parent.slotKey}},[a("h3",{attrs:{id:"q-are-other-versions-of-openvino-supported"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#q-are-other-versions-of-openvino-supported"}},[e._v("#")]),e._v(" Q: Are other versions of openvino supported?")]),e._v(" "),a("ul",[a("li",[e._v("In addition to 2020.3.194 is officially supported by openncc, other new versions of openvino can also be supported by openncc, but there is no complete coverage evaluation, and some models run abnormally.")])]),e._v(" "),a("h3",{attrs:{id:"q-how-to-ensure-that-openncc-camera-has-firmware-algorithm-acceleration-enabled"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#q-how-to-ensure-that-openncc-camera-has-firmware-algorithm-acceleration-enabled"}},[e._v("#")]),e._v(" Q：How to ensure that openncc camera has firmware algorithm acceleration enabled?")]),e._v(" "),a("ul",[a("li",[e._v('First of all, only openncc camera with ar0234 module can enable firmware dual engine algorithm acceleration function. With openncc view software, it can be activated directly by checking the "inference acceleration" box. If developers need to enable firmware algorithm acceleration in their own programs, they can set the last member of the structure network1par, inferenceacc, to 1 when the SDK is initialized. In addition to observing the frame rate change, you can also test the higher voltage confirmation at the USB interface when accelerating.')])]),e._v(" "),a("h3",{attrs:{id:"q-what-is-the-effect-of-firmware-dual-engine-algorithm-acceleration"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#q-what-is-the-effect-of-firmware-dual-engine-algorithm-acceleration"}},[e._v("#")]),e._v(" Q: What is the effect of firmware dual engine algorithm acceleration？")]),e._v(" "),a("ul",[a("li",[e._v("Firmware dual engine algorithm acceleration has a good effect on all models in openncc view, most of which can improve the performance by nearly 100%. 0004 face recognition model can be stable at about 80 frames in the test process, but it can only reach 43 frames before accelerating. In addition, the models that developers call based on openncc software development framework can be used correctly.")])]),e._v(" "),a("h3",{attrs:{id:"q-what-deep-learning-frameworks-and-models-does-openvino-support"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#q-what-deep-learning-frameworks-and-models-does-openvino-support"}},[e._v("#")]),e._v(" Q: What deep learning frameworks and models does openvino support?")]),e._v(" "),a("ul",[a("li",[a("p",[e._v("Caffe* (most public branches)")])]),e._v(" "),a("li",[a("p",[e._v("TensorFlow*")])]),e._v(" "),a("li",[a("p",[e._v("MXNet*")])]),e._v(" "),a("li",[a("p",[e._v("Kaldi*")])]),e._v(" "),a("li",[a("p",[e._v("ONNX*")]),e._v(" "),a("p",[e._v("more："),a("a",{attrs:{href:"https://docs.openvinotoolkit.org/2020.3/_docs_IE_DG_Introduction.html",target:"_blank",rel:"noopener noreferrer"}},[e._v("Supported Frameworks and Formats "),a("OutboundLink")],1)])])]),e._v(" "),a("h3",{attrs:{id:"q-does-openncc-have-a-hardware-version-that-supports-booting-from-flash"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#q-does-openncc-have-a-hardware-version-that-supports-booting-from-flash"}},[e._v("#")]),e._v(" Q: Does openncc have a hardware version that supports booting from Flash?")]),e._v(" "),a("ul",[a("li",[e._v("After the development is completed, if the deployment requires openncc to run independently from the SOC, and there is a mass production version with flash, please consult openncc.")])]),e._v(" "),a("h3",{attrs:{id:"q-i-have-technical-problems-with-model-transformation-and-parsing-where-can-i-get-support"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#q-i-have-technical-problems-with-model-transformation-and-parsing-where-can-i-get-support"}},[e._v("#")]),e._v(" Q: I have technical problems with model transformation and parsing. Where can I get support?")]),e._v(" "),a("ul",[a("li",[e._v("Intel ·OpenVINO community could help you.")])]),e._v(" "),a("h3",{attrs:{id:"q-does-openncc-support-developers-to-change-lenses-themselves"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#q-does-openncc-support-developers-to-change-lenses-themselves"}},[e._v("#")]),e._v(" Q: Does openncc support developers to change lenses themselves?")]),e._v(" "),a("ul",[a("li",[e._v("For general applications, developers can choose different shots according to their own scenes. If the image quality can not meet the strict requirements of the scene after lens replacement, you can seek technical customization support from openncc.")])]),e._v(" "),a("h3",{attrs:{id:"q-does-openncc-support-the-development-of-ubuntu-virtual-machine"}},[a("a",{staticClass:"header-anchor",attrs:{href:"#q-does-openncc-support-the-development-of-ubuntu-virtual-machine"}},[e._v("#")]),e._v(" Q: Does openncc support the development of Ubuntu virtual machine?")]),e._v(" "),a("ul",[a("li",[e._v("Yes, the USB device of openncc is detected by the virtual machine when it is started. If it is detected by the host first, and then manually switched to the virtual opportunity, the communication exception that the openncc development interface retrieval device fails occurs.")])])])}),[],!1,null,null,null);t.default=n.exports}}]);