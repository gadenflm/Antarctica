### 2021-7-19
1. 注释掉了audio 相关 (sdk_init 中的初始化调用和 unit_test 中的调用)
2. 添加debug 信息得到：

    Stream 0 1 2 ： encode stream   
    Stream 3 4 : capture stream

    * `VinNum = 1`  Sensor 数量
    * `TVT_SDK_VIDEO_ENC_STREAM_BUTT = 5` 一个Sensor 调用5个Stream
    * `TVT_SDK_VIDEO_ENC_JPEG_STREAM = 3` 3号为默认 encode jpeg 的 Stream 

3. 默认只初始化了3号Stream，添加初始4号Stream 过程(`sdk_video.c`)

4. `tvt_sdk_print.h` #define WKL_DEBUG 1

### 2021-7-20

1. 注释掉了osd相关 (unit_test 中的调用)  
2. 注释掉了mask相关 (unit_test 中的调用)  
3. 开启osd  
4. Time osd 添加 H\W <=0 ? continue;  
5. Comm osd 修改 ctrl 函数调用(SDKSetOSDPara)  
6. unit_test RunOSD 测试修改Comm相关,最后得到在图片左上角加上点阵后的实现。 

#### 2021-7-21

1. 添加文件 `sdk_flash.c` `tvt_sdk_flash.c` `tvt_sdk_flash.h` 
2. 将海思SDK 中的 接口 移植到当前SDK 并测试通过 
    * `SDKGetFlashParamItem`
    * `SDKSetFlashParamItem`

3. 随记:
    * 写入的 device 为 mmcblk0
    * 写入的 offset 为 0x300000

4. __过程中留空(`///TODO`)__
    * SPI相关(本次仅实现了emmc相关)
    * encryption 相关 (未引入，过程中注释掉跳过)。

#### 2021-7-22 ~ 2021-7-23 

1. 添加文件 `tvt_sdk_update.h` `tvt_sdk_update.c`

2. 将海思SDK 中 `flashcpm.c` `flashcpm.h` 的接口统一移植到 `tvt_sdk_flash.c` 
`tvt_sdk_flash.h` 

3. 将海思SDK 中的 接口 移植到当前SDK 并测试通过 
    * `char *SDKGetUpdateMem(void);` 
    * `int SDKUpdateFlash(char *pUpdateMem, unsigned int iUpdateSize, unsigned int iUpdateType);`
    * `int GetSdkUpdateFlag(void);`

4. 随记:
    * 初始化参数没做针对性适配，仅配置了测试用参数。
    * 原SDK接口有两个版本`prev` `comm`，本次仅移植了新版本`comm`
    * 过程中大量留空(`//TODO`)！ 

5. __过程中留空(`///TODO`)__ 
    * SPI相关(本次仅实现了emmc相关)
    * encryption 相关 (未引入，过程中注释掉跳过)。
    * memory 相关!
    * Uboot 相关!

#### 2021-7-26

1. 添加文件  `amba_yuv.c` `amba_yuv.h` `tvt_sdk_video_facedt.c` `tvt_sdk_video_facedt.h` `sdk_face_dt.c` 
 
2. 修改文件 `tvt_sdk_video.c` ,在 `tvt_sdk_video_init()`  和 `tvt_sdk_video_exit()`  上添加 face_dt 相关.

    修改文件 `amba_video.c` ,在 `chip_video_init()`  和 `chip_video_exit()`上添加 face_dt 相关.

3. 测试Canvas中提取YUV数据，主要代码在`amba_yuv.c`中，通过在`amba_video.c` 中添加 debug 调用测试。 最终实现调用接口，获得1张capture数据后存入dma 空间，再读取dma空间中的对应位置保存，通过YUV viewer 查看数据。

4. 存在问题，仅有Y分量，没有UV分量！

#### 2021-7-27

1. 修改文件 `amba_vproc.c`, 在修改对应canvas的分辨率，用以抓yuv图像 
    (将 `canvas[2].max.width` 和 `canvas[2].max.height` ) 改为 1920 * 1080

2. 修改 提取 yuv数据部分，成功调试得到UV彩色数据。