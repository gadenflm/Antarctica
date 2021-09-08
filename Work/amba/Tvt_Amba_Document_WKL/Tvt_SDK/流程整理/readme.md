# Tvt IPC SDK Video流程整理
<font size="4.5" style="font-weight: bolder;">
代码结构分为3层

  * `SDK_XXX.c` 用以封装提供给外部的接口, 与 `tvt_sdk_XXX` 对接。 同时多线程初始化也在这一层。
  * `tvt_sdk_XXX.c` 用以实现 接口内部默认逻辑, 与芯片无关。 上层为 `SDK_XXX`, 下层对接 `amba_XXX`。 许多接口调用采用 `枚举类型-函数指针` 的Ctrl接口形式, 业务锁在这一层维护, Tvt IPC SDK 内存分配也在这一层管理。
  * `amba_XXX.c` 用以实现 与驱动交互 代码, 仅该层可以访问 iav驱动 接口, 该层会管理映射内存空间。上层对接 `tvt_sdk_XXX`, 其中暴露给上层的接口全部在`tvt_sdk_chip.h` 中申明。

下方目录按执行顺序排布，理论上SDK依次往下执行。 
</font>

[TOC]

![](img\Tvt_Amba.png)

## SDK自身 初始化过程

### 子模块初始化

主要为模块分配结构体内存，初始化业务锁，包含以下模块

* 升级 Update
* 配置存取 Flash
* 运动检测 MotionDT
* 音频 audio 

---

### 视频初始化 

#### 1 Sensor 信息 
首先从`tvt_sdk_param_E4H_4M.c`获取 Sensor 信息如下

```C
// Sensor输入信息
static TVT_SDK_PROD_SENSOR_S stProdVinInfo[] = 
{
    {
        .enSensorType = TVT_SDK_PROD_SENSOR_IMX464,
        .enSensorHDRMode = TVT_SDK_PROD_SENSOR_HDR_2X,
        .u32VinDev    = 0,
        .u32MaxWidth  = 2592,
        .u32MaxHeight = 1520,
    },
};
```

#### 2 Vin 配置
获取 iav 句柄, 配置vin 参数如下

```C
    // HDR模式 可选Linear 和 2X
    stVinDevMode.hdr_mode = (pstVideoCfg->enWorkMode == ISP_BLC_MODE_WDR) ? AMBA_VIDEO_2X_HDR_MODE : AMBA_VIDEO_LINEAR_MODE; 

    // FPS 可选 N制 P值，分别对应 29.97 和 25
    if ( SDK_VIDEO_INPUT_NTSC == pstVideoCfg->enFormat)
    {
        stVsrcFps.fps = (pstVideoCfg->enWorkMode == ISP_BLC_MODE_WDR) ? AMBA_VIDEO_FPS_59_94 : AMBA_VIDEO_FPS_29_97;
    }
    else
    {
        stVsrcFps.fps = (pstVideoCfg->enWorkMode == ISP_BLC_MODE_WDR) ? AMBA_VIDEO_FPS_50 : AMBA_VIDEO_FPS_25;
    }
```

#### 3 iav 状态 init-->idle

#### 4 dsp 配置
主要集中在 `@amba_vproc.c 中的函数 int amba_vproc_start(const AMBA_VPROC_RES_CFG_S* pstVprocCfg, int s32VinNum)`

__该部分会配置大量参数,主要分为 `chan_cfg, Stream, Canvas, buf_cfg`__

* __`chan_cfg[0]`__

管理 vin --> Source Buffer 的参数,如果新增Source Buffer, 需要对应修改配置。

* __`stream[]`__

此处申明最大Stream数量，同时设定Stream模式， 非efm模式的Stream仅配置 宽,高,MAX_M 三个参数即可

__Encode Stream的具体参数在启动该Stream设置，即SDK初始化后设置__

* __`canvas_cfg[]`__

Canvas 相当于原始Buffer进来后的缓存空间。 __Canvas占用大量DSP Buffer的内存空间，注意DSP Buffer的内存上限(目前设置为320MB)__

Canvas 默认由iav自己维护,除了提供给Vout之外均配置为Encode模式。

iav支持配置一个手动 Canvas, 目前用于 Face Detection 接口的原始数据。(只能配置一个，若后续仍有需要手动获得YUV数据的接口，可以使用同一个Canvas)

获取YUV 等原始数据流即通过请求 Canvas数据。 见 `Amba SDK 个人整理`

* __`buf_cfg[]`__

Source Buffer 主要配置 输出的宽高和对应Canvas缓存。

__不同的Source Buffer 有自身的分辨率限制等等, 设置时需要结合Doxygen 文档。__

Tvt IPC SDK 启用了Amba 的 Multi Pass 功能，Src_Buffer 0 作为 Main Buffer 进入后，供有两个通道(Pass 0, Pass 1)， 分别提供5个 Src_Buffer。  一共10个buffer， 现已经使用9个。

#### 5 映射 BSB 内存

BSB( Bit Stream Buffer ) 为编码输出的缓存空间。 目前分配大小为(10MB)。 

编码结果的获取通过映射BSB内存到虚拟地址后，请求帧信息中包含 帧数据的内存偏移量。通过 起始地址 + 偏移量 的方式得到帧数据的地址。

__注意BSB 空间上限为10MB,如果数据进入过多且未被取走释放，会出现 覆盖现象，即存在编码后的帧丢失。详见 Doxygen 文档 FEATURE SETS 的 BASIC-STREAM / 3.Dual BSB__

```C
 // map bsb
    struct iav_querymem stQueryMem;
    struct iav_mem_part_info* pstPartInfo;
    stQueryMem.mid = IAV_MEM_PARTITION;
    pstPartInfo = &stQueryMem.arg.partition;
    pstPartInfo->pid = IAV_PART_BSB;
    AM_IOCTL(s32IavFD, IAV_IOC_QUERY_MEMBLOCK, &stQueryMem);

    g_stVencCfg.u32BsbSize = pstPartInfo->mem.length;
    g_stVencCfg.pszBsbMem = mmap(NULL, g_stVencCfg.u32BsbSize * 2, PROT_READ, MAP_SHARED, s32IavFD, pstPartInfo->mem.addr);
```

#### 6 iav 状态 idle-->preview 

__上述 vin dsp 等模块的配置均在这一步依次生效，同时这一步会访问 aaa 库的接口，初始化并使能 3A 库。 这一步报错，可以通过 `dmesg` 查看 iav 报错信息。 定位问题原因。__

#### 7 依次初始化 Video 相关子模块

先在 `amba_XXX` 中初始化对应模块与 芯片 相关的结构体，内存等等。 再初始化 `tvt_sdk_XXX` 中对应结构体和业务锁。

* Vout 
* OSD
* Mask
* ROI
* Device
* Motion Detection

---

### 开启线程维护子过程

共创建3个线程如下

* MotionDT 负责循环Motion Detection 原始数据，更新Motion Detection的结果。
* TimeOSD 负责刷新Osd，包括根据时间刷新 字符OSD 以及根据 内存数据 刷新 点阵 OSD。
* VencStream 负责维护编码数据，取出各个Stream的编码结果。

## AP --> SDK 开启编码

上层AP通过调用 SDKSetVEncPara() 接口创建编码通道, 配置具体编码信息,使能编码,__这一步结束后,各个Stream 会开始编码并持续产生码流输出__

### 关闭原通道，重新设定部分Vproc资源

关闭通道 会依次调用iav的 STOP 和 ABORT 接口。

__Amba DSP 部分参数支持 On-the-Fly Setting,即无需将iav重置为 idle状态，可以一边运行一边修改。具体详见 Doxygen 文档__,因此这部分会根据 接口获得参数，重新设置 DSP 的相应资源:

* Source Buffer 的输出分辨率与对应Stream相同。
* Canvas 的 刷新帧率与对应Stream 相同。

__此时 Canvas 缓存数据大小大于 Buffer数据大小，提取Canvas中YUV 数据仍然需要按 Canvas最大分辨率提取。__

### 设定 编码参数

默认编码窗大小为满大小，此处可以设计根据需求仅编码数据中的部分子图输出。

__此处根据 MJpeg 和 H26x 分别设置具体编码参数，详见 `amba_venc.c` 中函数`amba_venc_setH26xEncodeParam()` 和 `amba_venc_setMjpegEncodeParam()`__

__其中 H264/H265 的参数大致参考 sample 直接设置，且使用了大量默认参数。TODO：优化及确认部分参数的设置合理性__

最后会设置码流输出的绝对帧率。

### 开启编码通道,恢复 OSD 和 ROI

这一步调用 `chip_video_startStream()` 使能编码，同时会设置请求I帧标识。 若BSB缓存区满而出现覆盖情况时，请求的I帧被丢弃，此时会出现报错要求重新请求I帧。

