#驱动学习笔记

##一、HDF框架使用测试

### 驱动文件及驱动代码
1. /vendor/hauwei/hdf/
2. mkdir wkl_debug
3. /vendor/hauwei/hdf/wkl_debug/sample_driver.c

## 二、添加驱动配置信息

### HDF框架下的驱动设备描述（必有）

/vendor/hisilicon/*/config/device_info/device_info.hcs

可选择新建节点或者在原有platform 下添加：

本次测试选择新建

plolicy = 2 表示对用户态和内核态都发布该service

### 驱动私有信息（optional）

本次测试未测试

## 三、添加编译配置

驱动编译使用Makefile模板
