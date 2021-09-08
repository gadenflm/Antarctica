# Motion Detection 接口

[TOC]

## 说明

与安霸技术支持沟通, 目前 motion detection 采用方案如下:

引入库 `libopencv_core.so` `libopencv_imgproc.so` `libtbb.so` `libz.so` `libmdet.so` , 使用amba 官方非开源库 mdet library, 其依赖opencv 实现，因此占用空间较大。

该库没有 sample， 可对应如下目录中程序修改测试该库`ambarella/oryx_v2/base/video/control/vca/plugins/motion_detect`

__默认3.0.3版本的SDK下libmdet不支持当前平台，技术支持在jira上提供了3.0.6版本的SDK下 libmdet.so 使用该库成功实现__

## 流程 

新增加 pass 1 的 Source buffer 2 和 Canvas 8, 分辨率704x576 or 704x480 两种模式, 由于手动模式(manual feed)仅支持配置一个, 采用自动数据流。

SDK初始化时 开启 MotionDt_thread 反复调用 loop data 过程, 该过程频率为5Hz, 每次执行时提取 Canvas 8 中的数据送入，默认取其原始数据中ME0数据用以进行运动检测(YUV数据中Y分量压缩1/8得到),也可更改为 ME1 和 YUV 数据.

`tvt_sdk_motion_dt.c` 中实现 维护锁和整体控制过程, 其中流程控制使用一个业务锁，获取数据单独使用一个锁。

`amba_mdt.c` 维护与 mdetlib 交互的底层代码

获取 yuv me0 me1 三种数据，目前选用压缩率最高的 me0,对应分辨率为88x72， 将me0数据送入 mdet 库的接口。

从库接口中 得到 `int*`类型连续数据,其大小与送入库的buffer大小相同, 每个int类型表示buffer中该像素是否被判定为motion, 即每4个字节为0 则为静止，否则为移动。

提供给上层AP的数据进一步压缩, 手动按序4x4 访问 88x72 的 motion 数据。 每16个像素点 若有3个以上像素点存在motion则返回该块为motion块,最终得到 22x18 的块motion分布。

__SDK对AP暴露接口中可以设定 敏感度即 Sensitivity 参数, 该值在 SDK中被转换为 mdet 库的参数 threshold, 通过设定阈值实现敏感度。 即库处理单个像素点时，根据其运动值是否超出threshold 来划分该点是否为motion 的像素点。 目前使用的算法下，thershold 取值范围为`[2,64]`__

__Mdet 库检测时可以设置4块 ROI 区域，检测时仅对 ROI区域中的像素进行检测， 目前只使用了一块ROI 覆盖整张图像，若有需要可以根据情况调整。__

## 结果检验方法

可通过保存原motion数据, 每个像素点占用一字节, motion的像素点保存为0xff即255, static 的像素点保存为 0x00即0, 接着通过 Matlab 读取文件转为二值图像检查结果

```Matlab
clc
clear
fid =fopen('D:\Work_AM\share\sdk\mdet_mat_test.dat');
data =fread(fid);

data_compress = 32;
data_width = 704;
data_height = 576;
data_size = data_width*data_height/(data_compress*data_compress);
nImage =length(data)/data_size;
data =reshape(data,data_size,[]);

fps = 5;
fig =figure;
for i=1:nImage
   imshow(reshape(data(:,i),data_width/data_compress,data_height/data_compress)');
   frame =getframe(fig);
   im{i} =frame2im(frame);
end
filename ='D:\Work_AM\share\motion_stream_1280_720.gif';
for i =1:nImage
   [A,map] = rgb2ind(im{i},256);
   if(i==1)
       imwrite(A,map,filename,'gif','LoopCount',Inf,'DelayTime',1/fps)
   else
        imwrite(A,map,filename,'gif','WriteMode','append','DelayTime',1/fps);
   end
end

close all
```

![](motion_320_180_max_sensitivity.gif)
