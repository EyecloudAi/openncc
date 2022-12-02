(window.webpackJsonp=window.webpackJsonp||[]).push([[25],{383:function(e,n,t){"use strict";t.r(n);var r=t(44),a=Object(r.a)({},(function(){var e=this,n=e.$createElement,t=e._self._c||n;return t("ContentSlotsDistributor",{attrs:{"slot-key":e.$parent.slotKey}},[t("h3",{attrs:{id:"q-其他版本的openvino被支持吗"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#q-其他版本的openvino被支持吗"}},[e._v("#")]),e._v(" Q: 其他版本的OpenVINO被支持吗？")]),e._v(" "),t("ul",[t("li",[e._v("除了2020.3.194被OpenNCC正式支持外，其他新版本的OpenVINO也可以被OpenNCC支持，但是未做完整的覆盖性评测，部分模型存在运行异常情况。")])]),e._v(" "),t("h3",{attrs:{id:"q-如何确保openncc相机开启了固件算法加速"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#q-如何确保openncc相机开启了固件算法加速"}},[e._v("#")]),e._v(" Q: 如何确保OpenNCC相机开启了固件算法加速？")]),e._v(" "),t("ul",[t("li",[e._v('首先只有搭配AR0234模组的OpenNCC相机才能开启固件双引擎算法加速功能，使用OpenNCC View软件，可以直接通过勾选"inference acceleration"框激活，如果开发者需要在自己开发的程序当中开启固件算法加速，可以在sdk初始化时将传入结构体Network1Par的末位成员inferenceACC置1。除了观察画面帧率变化，开启加速时也可以测试USB接口处变高的电压确认。')])]),e._v(" "),t("h3",{attrs:{id:"q-固件双引擎算法加速有何效果"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#q-固件双引擎算法加速有何效果"}},[e._v("#")]),e._v(" Q： 固件双引擎算法加速有何效果？")]),e._v(" "),t("ul",[t("li",[e._v("固件双引擎算法加速对OpenNCC View当中所有的模型都有不俗的效果，大部分能提升近百分百的性能，0004人脸识别模型在测试过程中可以稳定在80帧左右，而未开启加速时只能达到43帧。此外，开发者基于OpenNCC软件开发框架调用的模型都可以被正确作用。")])]),e._v(" "),t("h3",{attrs:{id:"q-openncc支持的openvino支持哪些深度框架和模型"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#q-openncc支持的openvino支持哪些深度框架和模型"}},[e._v("#")]),e._v(" Q: OpenNCC支持的OpenVINO支持哪些深度框架和模型？")]),e._v(" "),t("ul",[t("li",[e._v("Caffe* (most public branches)"),t("br"),e._v("\nTensorFlow*"),t("br"),e._v("\nMXNet*"),t("br"),e._v("\nKaldi*"),t("br"),e._v("\nONNX*"),t("br"),e._v("\n更多信息见："),t("a",{attrs:{href:"https://docs.openvinotoolkit.org/2020.3/_docs_IE_DG_Introduction.html",target:"_blank",rel:"noopener noreferrer"}},[e._v("Supported Frameworks and Formats "),t("OutboundLink")],1)])]),e._v(" "),t("h3",{attrs:{id:"q-openncc有支持自启动的硬件版本吗"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#q-openncc有支持自启动的硬件版本吗"}},[e._v("#")]),e._v(" Q: OpenNCC有支持自启动的硬件版本吗？")]),e._v(" "),t("ul",[t("li",[e._v("开发完成后，如果部署需要OpenNCC脱离主控独立运行，有带Flash的量产版本，请向OpenNCC咨询。")])]),e._v(" "),t("h3",{attrs:{id:"q-我有模型转换和解析的技术问题-哪里可以得到支持"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#q-我有模型转换和解析的技术问题-哪里可以得到支持"}},[e._v("#")]),e._v(" Q: 我有模型转换和解析的技术问题，哪里可以得到支持？")]),e._v(" "),t("ul",[t("li",[t("a",{attrs:{href:"http://openvinodev.org.cn/",target:"_blank",rel:"noopener noreferrer"}},[e._v("Intel ·OpenVINO中文社区"),t("OutboundLink")],1),e._v("可以得到支持")])]),e._v(" "),t("h3",{attrs:{id:"q-openncc支持开发者自己更换镜头吗"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#q-openncc支持开发者自己更换镜头吗"}},[e._v("#")]),e._v(" Q: OpenNCC支持开发者自己更换镜头吗？")]),e._v(" "),t("ul",[t("li",[e._v("对于一般应用，开发者可以根据自己场景选配不同的镜头。如果更换镜头后，图像质量不能满足场景的有严苛要求，可以向OpenNCC寻求技术定制支持。")])]),e._v(" "),t("h3",{attrs:{id:"q-openncc支持ubuntu虚拟机开发吗"}},[t("a",{staticClass:"header-anchor",attrs:{href:"#q-openncc支持ubuntu虚拟机开发吗"}},[e._v("#")]),e._v(" Q: OpenNCC支持Ubuntu虚拟机开发吗?")]),e._v(" "),t("ul",[t("li",[e._v("支持，需要把OpenNCC的USB设备在启动时就被虚拟机检测到，如果先被主机检测到，再手工切换到虚拟机会出现OpenNCC开发接口检索设备失败的通信异常。")])])])}),[],!1,null,null,null);n.default=a.exports}}]);