# Amba Tvt Ipc SDK FaceDt 接口

[TOC]

## 说明

该接口为仿照 Hisi Tvt SDK 中的 HUANN Face 接口,实现相关过程。

过程中跳过了 走廊模式, 车牌识别 等接口。 仅打通了 抓取YUV数据 和从 内存中获取 buffer 编码两个环节。

__该接口仅用测试程序 尝试了yuv 抓取和编码流程的实现,均只测试了各抓取两张YUV和编码成两张JPEG的情况，未实际测试多张图片,多个编码结果__

__目前抓取的YUV图像每编码一次都需要在内存中进行一次拷贝, 使用 EFM(encode from memory) 模式送入 Stream 获得 JPEG图像。 TODO:寻找办法取消拷贝__

__本模块Canvas 模式为 Manual Feed, 即手动控制缓存, Amba 目前只能配置一个Canvas 为manual feed，如果其他模块也需要此功能, 可以考虑设置成同一个Canvas__

<font color="red" size="4.5"> TODO: 目前只是逻辑通了，根据实际使用时优化</font>

## 流程记录

### 初始化

调用Start HUANN 接口才初始化本模块，分别会 映射 Canvas_Pool 的内存 和 DSP 的内存。

---

### YUV 图像控制 

底层接口为 `amba_yuv_GetFrame()`, 每次调用时，会从当前时刻的Source buffer中申请一帧送入 Canvas缓存，并将对应详细逐层传出给AP。(传出地址为物理地址)

__获得YUV的一帧并不会自动释放，必须底层调用 `amba_yuv_ReleaseFrame()` 释放对应内存__

---

### EFM 编码控制

EFM 模式下，__每次编码会先请求一个 内存区用以存放此次编码的buffer，此时需要将Canvas 中的数据 拷贝一帧进入该内存区。__

接着触发编码, 因为一次编码会输出 4帧以上的图像，其中仅有一帧包含编码后结果. 所以编码触发后, 会先请求所需 Jpeg图像，将结果拷贝至新malloc的内存后，送出该区域。

__获取Jpeg帧后，下次编码前，因为BSB的内存中会存放上次编码残留的无信息图像，目前每次取编码结果通过时间戳对应。一直从BSB中获取所需图像,直到时间戳对应。__

__因为每次编码窗的大小、位置等参数都需要重启，所以每次需要重置 Stream 参数。 目前思路为每次编码后 关闭该Stream, 下次编码开启前会用死循环等待！__

__TODO: 目前Jpeg Buffer 的内存在 `amba_yuv.c` 中malloc申请，应该移植到 `tvt_sdk_face_dt.c` 中申请。 其释放统一在 `tvt_sdk_face_dt.c`中__

## 使用注意

### 关于 pitch 与 width

其canvas图像(矩阵)在内存中为按行存取,因为对齐需要,其在 1920 x 1080 下读取一行所占字节为
1984 (1920 + 64), 依次读取1080 行，输出图像如下 (仅读取Y分量)

![](img\yuv_DRAM.png)

左侧 1920x1080 对应 buffer 提供给 canvas 的 图片数据流。  
__width 为提前设定的图片宽度1920。__ 

右侧 64x1080 为多出部分，其值全为0。  
__pitch 为具体存储一行占用的字节数(两行数据的首地址差值)为 1984 (1920 + 64)__

获得图像数据地址后需要根据 pitch 和 width 关系对应处理数据。

* `pitch > width` 对应读取每行后,舍去 (pitch - width) 个数的字节
* `pitch == width` 无需处理 

---

### 关于 Y 与 UV 地址。

DSP 内存中, Y 与 UV 连续存放, 针对具体YUV编码种类，可根据图像大小算出UV所占字节，本次测试使用 YUV420NV12, UV的字节为图像大小的1/2。

__生成YUV 彩图时需要根据 y部分的地址提取对应y分量大小的数据，舍弃pitch中的对齐字节后，再拼接UV分量的数据，同样都要针对`pitch` 和 `width` 进行处理。__