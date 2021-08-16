[TOC]
## 关键全局变量整理

### `g_stSdkCfg` SDK参数, `SDK_Init()` 中初始化
```C
typedef struct tvt_sdk_cfg_s
{
    unsigned char                       u8Init;
    int                                 s32SDKVersion;      ///< 版本号
    unsigned long long                  u64DevFeature;      ///< 设备能力集
    TVT_SDK_PROD_DEV_INFO               stDevModelInfo;     ///< 机型
    SDK_DAYNIGHT_STATE                  enDayNightState;    ///< 日夜状态
    int                                 s32WorkMode;        ///< 线性/HDR模式
    int                                 s32VinCnt;          ///< Sensor个数
    pthread_t                           s32SdkThread;
    int                                 s32SdkThreadRun;
}TVT_SDK_CFG_S;
```

### `g_pstSdkVideoInfo` 视频业务结构体！ `tvt_sdk_video_init()` 中初始化

```C 
/**
 * @brief 业务结构体
 */
typedef struct tvt_sdk_video_info_s
{
    char                                 u8Init;                            ///< 初始化标志位
    char                                 u8Exit;                            ///< 去初始化标志位，线程退出标志
    pthread_mutex_t                      s32Mutex;                          ///< 业务锁
    char                                 szMutexName[32];                   ///< 业务锁名称
    unsigned int                         u32MutexLockCnt;                   ///< 业务锁锁定次数
    pthread_t                            s32ThreadID;                       ///< 业务线程ID
    int                                  s32VinCnt;                         ///< sensor个数
    TVT_SDK_VIN_INFO_S                   stVinInfo[CHIP_MAX_VI_NUM];        ///< 视频输入信息
    TVT_SDK_VIDEO_BUFFER_S               stStreamBuff[CHIP_MAX_VI_NUM];     ///< 各路编码流缓存数据
    TVT_SDK_VIDEO_BUFFER_S               stJpegBuff[CHIP_MAX_VI_NUM][CHIP_VENC_JPEG_NUM];   ///< 各路JPEG流缓存数据
    TVT_SDK_VIDEO_ENC_CFG_S              stEncChnInfo[CHIP_MAX_VENC_NUM];   ///< 编码通道信息
    TVT_SDK_VIDEO_WATERMARK_S            stWaterMark;                       ///< 编码水印信息
    TvtSdkProdCb                         pSdkParamCbFunc;                   
    int                                  s32ReserveArr[4];
}TVT_SDK_VIDEO_INFO_S;

```

# SDK_Init

## 初始化参数业务(空函数)

## 初始化外设 `tvt_sdk_device_init`

1. 触发 `tvt_sdk_param.c` 中的回调函数，获取 `tvt_sdk_param_E4h_4M.c` 中的对应信息。  
2. 将对应信息保存在 `g_stSdkCfg.stDevModelInfo` 中。
3. 初始化得到 "tvt_sdk_device_lock" 锁

## 初始化按键(未实现)

## 初始化音频业务 tvt_sdk_audio_init (调试没有音频设备，注释并跳过)

## 初始化视频及相关业务

`unit_test.c` 传入参数 `enInputFormat = SDK_VIDEO_INPUT_PAL`

---

### 初始化Sensor相关信息

触发回调` tvt_param_getVinInfo` , 最终获得 `tvt_sdk_param_E4h_4M.c`  中信息如下，并传给 `struct TVT_SDK_VIDEO_CFG_S` 中保存，并传给 `tvt_sdk_video_init()`。

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

---

### 初始化视频业务 `tvt_sdk_video_init()`

#### 初始化水印数据 `g_pstSdkVideoInfo->stWaterMark` 
存储如下
` char szBuf[8] = {0x0, 0x0, 0x0, 0x1, 0x66, 0xe5, 0x01, 0x00};`   

初始化得到 "watermark_buf_mtx" 锁

#### 申请码流缓存空间

1. 主要根据已获得的Sensor等配置信息，使用 `malloc()` 申请相应大小的内存(全局变量 `g_pstSdkVideoInfo` 中)  
    * `g_pstSdkVideoInfo->stStreamBuff[]`
    * `g_pstSdkVideoInfo->stJpegBuff[0][]` 0表示只有一个Sensor 输入，多Sensor输入时需要对应添加。
2. 初始化得到  "stream_buf_mtx_%d"、"jpeg_buf_mtx_%d" 锁 (%d 为对应编号)

#### (Key) 初始化视频模块，集合全部配置信息，传给`chip_video_init()`

1. 打开iav模块 `open("/dev/iav", O_RDWR, 0);`
2. 将iav的fd传给`amba_image_init()` 
    * 调用amba接口过程参考 amba 官方sample `ambarella/unit_test/private/idsp_test/arch_v5_custom_img_flow_op.c` 中`register_custom_img_flow_op()`函数
    * __开启3A线程，调用官方接口：阻塞函数`create_img_flow(s32Fd)`__
    * 全局变量中存储fd和线程信息

3. 配置sensor，对应信息传给`amba_vin_dev_start()`
    * __过程完成 视频输入模块(vin) 模块初始化!__
    * 先调用 `amba_vin_checkSourceInfo()` 获取Amba SDK中定义的信息。  
    * __模式配置:__ 官方SDK 结构体 `struct vindev_mode` (`iav_vin_common.h`中定义),通过 `ioctl()` 写入  
    * __最大帧率配置:__ 官方SDK 结构体 `struct vindev_fps`,通过 `ioctl()` 写入。

4. 通过 ioctl 设置 如下参数
    * IAV 状态为 IDLE (官方SDK 结构体`struct iav_idle_params`)
    * pyramid 关闭 (官方SDK 结构体`struct iav_pyramid_cfg`)

5. 配置视频处理资源, 集合cfg信息传给 `amba_vproc_start()`
    * __过程完成 视频处理模块(DSP) 初始化！__
    * 官方SDK 结构体 `struct iav_system_resource` 初始化如下，功能上 __设置系统资源__！ 主要包括如下类别信息的配置：
        - encode_mode
        - __chan_cfg__
        - __stream__
        - __canvas__
    * 通过 ioctl 写回`struct iav_system_resource` (初始化前通过ioctl 从 fd 获得的)
    * __配置各个编码通道的Buffer__
        - 先处理3个编码流 (main ; second ;third )
            + 主Buffer，运行期间不允许修改分辨率，不用来编码, IAV_SRCBUF_FIRST
            + 主码流, IAV_SRCBUF_3
            + 第二码流, IAV_SRCBUF_4
            + 第三码流, IAV_SRCBUF_2,此Buffer的分辨率有限制
        - 再处理2个抓拍流 (jpeg stream ; alarm jpeg stream)
            + jpeg, IAV_SRCBUF_3
            + alarm jpeg, IAV_SRCBUF_4
        - 最后处理 Vout
            + 配置 channel_id = 0，对应chan_cfg[0],目前仅针对Sensor0
            + buf_id = IAV_SRCBUF_5 (Vout)

6. 通过 ioctl 切换 IAV 为 PREVIEW 状态
7. 初始化 VOut 模块, `amba_vout_init`
    * `amba_vout_cfgBuffer()` 更新Vout Buffer的cfg信息
    * 官方SDK 结构体`struct voutdev_format` 初始化
 
8. 初始化 OSD 模块, 调用 `amba_osd_mapOverlay()` 分配OSD内存
9. 初始化 Mask 模块 
    * `amba_mask_mapMem` 分配Mask内存
    * 官方SDK 结构体`struct iav_video_proc` 初始化
        - enable = 0
        - y = 0
        - u = 128
        - v = 128

10. 初始化ROI模块，调用`amba_roi_mapQpMatrix()` 分配ROI内存

#### OSD,MASK,ROI 业务初始化

1. 过程基本类似
2. 先`malloc`申请对应 `struct TVT_SDK_XXX_INFO_S` 的内存空间
3. 再初始化得到 "tvt_sdk_xxx_lock" 的锁
4. 各模块对应Info 结构体使能并填入Vin信息

#### 将初始化中 动态信息集中在全局变量 `g_pstSdkVideoInfo` , 并初始化互斥锁

---

### 创建默认Jpeg 通道，固定CIF分辨率

默认仅仅初始化 Stream 3 为 `352*288` JPEG FrameRate=2,  
个人添加初始化 Stream 4 为 `352*288` JPEG FrameRate=2

#### 触发回调 `tvt_sdk_video_createEncStream()` 

1. `tvt_sdk_video_getVinIDAndEncIDX(...,int* ps32VinDevID, int* ps32EncIDX)` 获得设备编号和编码索引
2. 将所有信息集中在如下的struct中，传给`chip_video_createChn()`
```C 
typedef struct chip_video_enc_cfg_s
{
    int                         s32VinDevID;   ///< 所属Sensor设备号
    int                         s32StreamIDX;  ///< 通道索引(指CHIP_VIDEO_ENC_INDEX_E)
    SDK_VIDEO_ENCODE_TYPE       enEncType;     ///< 编码类型
    unsigned int                u32Width;      ///< 宽
    unsigned int                u32Height;     ///< 高
    unsigned int                u32FrameRate;  ///< 帧率
    SDK_VENC_MODE               enBitRateMode; ///< 码率模式
    unsigned int                u32BitRate;    ///< 码率
    unsigned int                u32GOP;        ///< GOP
    unsigned int                u32Profile;    ///< Profile
    int                         s32Resv[2];    ///
}CHIP_VIDEO_ENC_CFG_S;
```

#### 芯片视频模块创建编码通道 `chip_video_createChn()` 

1. 先调用 `amba_venc_destroyChn()` 关闭已打开通道
2. 调用 `amba_vproc_chnCfg()`  重新配置通道
    * Buffer
    * 分辨率
    * 帧率
3. 初始化 `struct AMBA_VENC_CHN_CFG_S`中配置信息,整合传给`amba_venc_createChn()`
4. 创建编码通道 `amba_venc_createChn()`
    * 第一步 `chip_video_getVencStreamID()` 得到 `StreamID`
    * 第二步 维护官方 SDK 结构体，`struct iav_stream_cfg` 和 `struct iav_stream_format*`
        - type 指定编码类型
        - enc_win 指定编码窗
    * 第三步 调用 `amba_venc_setH26xEncodeParam` 或 `amba_venc_setMjpegEncodeParam` 设置编码配置参数！__(过程主要参照Sample，具体使用需要更改！)__
    * 第四步 设置帧率
    * 第五步 __开始编码__，会打印对应Stream 信息。

#### 更新参数信息,并设置请求I帧标识为1，SDK Video 初始化结束

---

## 启动线程 sdk_thread

线程主要负责更新 OSD， 并刷新JPEG 数据

#SDK 对外函数