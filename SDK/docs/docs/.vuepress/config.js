module.exports = {
  base: '/openncc/',
//  themeConfig: {
//    logo: 'web/ncclogo.jpg',
//  }
locales: {
    // 键名是该语言所属的子路径
    // 作为特例，默认语言可以使用 '/' 作为其路径。
    '/': {
      lang: 'English', // 将会被设置为 <html> 的 lang 属性
      title: 'OpenNCC Docs',
      description: 'Open AI Camera'
    },
    '/ch/': {
      lang: '中文',
      title: 'OpenNCC技术文档',
      description: '开放人工智能摄像机',
    }
  },

  themeConfig: {
    locales:{
      '/':{
        nav: [
          { text: 'OpenNCC', link: 'https://www.openncc.com'},
          { text: 'Github', link: 'https://github.com/EyecloudAi/openncc'},
          { text: 'OpenVINO', link: 'https://docs.openvinotoolkit.org/'},
        ],
       searchMaxSuggestions: 10,
       displayAllHeaders: true, // 默认值：false
       sidebar: [
          ['Introduction.md','Introduction of OpenNCC'],
          ['HardwareManual.md','OpenNCC Hardware Manual'],
          ['GettingStart.md','OpenNCC Getting Start'],
          ['nccview.md','OpenNCC View\'s Manual'],
          ['Software_Manual.md','Software User\'s Manual'],
          ['Openncc_sdk_api.md', 'SDK API'],
          ['openvino_install.md','How to install OpenVINO'],
          ['DownloadLink.md','Download Released Packets'],
          ['FAQ.md', 'FAQ'],
        ],
      },
      '/ch/':{
        nav: [
          { text: 'OpenNCC', link: 'https://www.openncc.com.cn'},
          { text: 'Gitee', link: 'https://gitee.com/eyecloud/openncc'},
          { text: 'OpenVINO', link: 'https://docs.openvinotoolkit.org/'},
        ],
       searchMaxSuggestions: 10,
       displayAllHeaders: true, // 默认值：false
       sidebar: [
          ['/ch/1_Introduction_ch.md','OpenNCC介绍'],
          ['/ch/2_HardwareManual_ch.md','OpenNCC 硬件手册'],
          ['/ch/3_GettingStart_ch.md','OpenNCC 快速开始'],
          ['/ch/4_NccView_ch.md','OpenNCC View说明'],
          ['/ch/5_SoftwareManual_ch.md','软件开发包手册'],
          ['/ch/6_SdkApi_ch.md','OpenNCC SDK接口文档'],
          ['/ch/7_OpenvinoInstall_ch.md','OpenVINO安装说明'],
          ['/ch/8_DownloadLink_ch.md','安装包和源码下载地址'],
          ['/ch/9_FAQ_ch.md', '常见问题'],
        ],
      }
    }
  }
}
