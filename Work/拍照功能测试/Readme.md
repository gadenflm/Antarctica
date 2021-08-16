
# OHOS 1.1.0 + Hi3516DV300 + IMX335 调试记录

### 相机配置

`\\foundation\multimedia\media_lite\service` 文件夹下有 imx335、imx307、jxf23 三种sensor对应配置文件。

鸿蒙内置的 Camera_lite 会在`OHOS#> \storage\data\cameradev.ini`中读取配置信息，所以需要将对应文件放入所定位位置(先放入nfs，用cp从nfs中拷贝如storage中)

__key__ :

* ls storage 为空，说明没有挂载userfs，该文件需要烧录到rootfs后面的地址。
* userfs.img 的大小可以在 `\\build\lite\gen_rootfs.py` 中修改
  `cmd = [ mkfs,userfs,[fs_type],[byteCount] ]`

### 编译配置

见`src\build.gn`

### 测试源码

见`src\main.cpp`, [官方文档](https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/guide/%E6%8B%8D%E7%85%A7%E5%BC%80%E5%8F%91%E6%8C%87%E5%AF%BC.md)

主要过程为重载 CameraStateCallback 类和 FrameStateCallback类 等，重写回调函数，在同步触发回调实现。

### 结果

见 `data\capture.jpg`

