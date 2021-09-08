#Doxygen 文档

[TOC]

### 说明

安霸SDK 整体官方信息多数都再Doxygen 文档中，需要在安装好环境后自己编译生成。

### 指令 

官网安装: http://www.doxygen.nl/download.html 

安装后, 进入amberalla目录。
```Shell 
cd ambarella
source build/env/arrch64-linaro-gcc.env
cd boards/cv28m_cashewnut
make sync_build_mkcfg
make tvt_ipcam_config
make sdk_doc
```

编译完成后,会在ambarella/document/doc 下输出对应文件。 访问amba/index.html 即可打开官方文档。

__多数指令或结构体或预定义等，都可以直接在该文档内通过搜索定位，查看其注释说明__

### 文档重要目录

* `IAV Driver API` , IAV 为 CV2x 系列 SOC 控制驱动,通过 ioctl() 系统指令调用。
* `FEATURE SETS` , 该目录下介绍了 安霸平台基础视频流 的基本特性，规则，约束 以及对应 sample测试方法等等. 其中下列子目录涉及了 TVT IPC SDK 的重要部分
    - BASIC - Video Input
    - BASIC - Video Output
    - BASIC - Source Buffer
    - BASIC - Canvas 
    - BASIC - Stream
    - BASIC - H.264 / H.265 Codec
    - ADVANCED - Multi-Pass
    - ADVANCED - Encode from Memory

