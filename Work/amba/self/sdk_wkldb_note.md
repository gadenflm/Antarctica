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
    (将 `canvas[2].max.width` 和 `canvas[2].max.height` ) 改为 1920 * 1080,
    7-28 舍去该方案

2. 修改 提取 yuv数据部分，成功调试得到UV彩色数据。主要修改在`amba_yuv.c`。 在整体执行过程中触发一次`wkl_test3`,进行测试，通过拷贝`canvas`数据到`dma`内存
中，实现任意尺寸抓取yuv数据。

#### 2021-7-28

1. 修改文件 `amba_vproc.c` ，添加新的buffer 与 canvas,用以专门生成处理YUV数据供人脸识别使用。 buffer 为`Pass 0 下 SRCBUF_5`,
canvas 为 `canvas 7`, 固定此 buffer 和 canvas 为facedt 专用。

2. 在`amba_yuv.c` 中添加抓拍过程中修改 buffer 分辨率的接口，初步调试通过，但存在以下问题
    * 调用该接口后立马 请求canvas数据 并不能成功得到低分辨率数据，存在一定延迟后可以成功得到修改后的分辨率
    * 接口仅修改buffer分辨率，不修改canvas，准备添加接口，在初始化阶段完成统一设定。

#### 2021-7-29 ~ 2021-7-31

1. 修改初始化,增加 canvas pool 的内存分配，修改`amba_vproc.c` 中的初始化方式为manual feed

2. 修改exit, 检查内存

3. 在amba_yuv_info_s 中添加结构体数组，用以维护 8个feed 格子的信息

4. 针对mannul feed 重写 get capture 过程， 思路 先调用`IAV_IOC_FEED_CANVAS_BUF` 再 query

5. 增加 release接口 

6. 测试AP，发现大量问题，需要依次修改

7. 修改timeosd 代码 和 SDK接口 `SetEncParam` `SDK_VideoExit` 等，针对 两个jpeg 码流对应调整。


#### 2021-8-1

1. 移植 Face Dt 相关到 AP版本的新libsdk.so

TODO:
发现问题 COMM OSD 传参问题, AP 传给接口`SDKSetOSDPara` 的参数有问题。

TODO: In HUANN_FACE_START
The Struct SDK_FACE_DT_CFG is not uesd !! only saved the width and height, but the YUV width and height is defined as 1920x1080 and can not changed

#### 2021-8-2 ~ 2021-8-4

1. 测试完成facedt 初步接口，共可同时抓取8个YUV图像，分别对应release后可再抓取。 仅测试了2个YUV 图像，调用外部接口保存正常

2. 完成facedt的crop Img 接口，可选择任意一张YUV图像，抓取其中特定位置、大小的Jpeg图像。 仅测试 单张YUV下抓取不同Pos的两种Jpeg。 调用内部接口保存正常。

TODO： 
过程中存在大量未实现接口，如facedt目前仅能在1080p下工作。 start接口的宽高调整仍存在问题。 

#### 2021-8-5 

针对更新后的AP，发现 SetEncParam 接口调用后启动 存在两个问题(已解决)

#### 2021-8-6 

1. 修改 Start过程，跳过 RequestIDR 的Flag  设置，防止由于时间过长导致缓存中I帧丢失问题
2. 完成facedt的整理测试，已初步通过。
3. 在SDKReleaseFrame 中添加跳过条件

TODO: 
SetEncParam() 接口调用后,  会设置请求I帧 flag 为1，若Get 接口调用较晚， 内存中I帧会被覆盖，出现报错，对码流读取本身并无影响。

内存中缓存大小配置说明 见 `Amba Doxygen / Featrue Set / Basic-Stream / 3. Dual BSB`

目前具体值见 `tvt_build/platform/ambarella/V3_00_003/cv2x_linux_sdk_3.0/ambarella/boards/cv28m_cashewnut/config/tvt_ipcam_config`
目前值大小如下
```
DSP_BSB_SIZE=0x0A00000
DSP_INT_BSB_SIZE=0x0000000
```

TODO:
存在问题，SDK_thread 中两个jpeg 控制函数会随着 重启Jpeg Stream 而停止。

原因: 过程中调用接口`SDKReleaseFrame()` ,传来错误参数如下，该参数`ucFrameType = 2` 指示为Jpeg类型，但`ucStreamIndex = 1` 并不是Jpeg码流，因而触发Jpeg释放过程，导致全局互斥锁对应释放出错。

```
[TIPC_SDK_CHK]<<"idx = 0">> @SDKReleaseFrame,main/sdk_video.c,295
[TIPC_SDK_CHK]<<"pts = 1890286">> @SDKReleaseFrame,main/sdk_video.c,296
[TIPC_SDK_CHK]<<"ucFrameType = 2">> @SDKReleaseFrame,main/sdk_video.c,297
[TIPC_SDK_CHK]<<"ucStreamIndex = 1">> @SDKReleaseFrame,main/sdk_video.c,298
```

#### 2021-8-9

1. 开始调试audio 模块  `bugdt.sh` 中添加
`export LD_LIBRARY_PATH=/mnt/nfs/sdk:$LD_LIBRARY_PATH`

2. 问题 (硬件配置问题，已解决)

#### 2021-8-10 ~ 2021-8-11

1. 发现问题： ALSA lib 下的配置参数与 `proc/asound` 里存在差异，原因为参数不支持，被自动修改成支持的参数
2. 修改ALSA配置device 从`default` 改为 `hw:0,0`。(`amba_audio.c`) 验证可以支持的参数如下
```
access: RW_INTERLEAVED
format: PCM_S16_LE
channels: 2
```

3. 测试audio无法通过。 重写ALSA全过程，新增函数`amba_audio_wkl_debug()` 及其相关。

* Capture(录音)： 全过程无报错，最终保存成文件没有声音。
* PLAYBACK(播放): 下载PCM格式文件 `8K_16bit_mono.pcm`,对应读取并转换成 交错模式 下的 双声道PCM 数据，成功播放，从板子 HP 出口外接 耳机得到正常播放声音。

#### 2021-8-12

1. 与安霸技术支持沟通，问题原因在于：默认line mux错误, 需要做如下更改:
```Shell 
<!-- amixer 为 ASAL 官方程序 -->
./amixer cset numid=7 1
<!-- 接着使用 ALSA 官方程序 arecord 测试保存可以得到音频文件 -->
./arecord -Dhw:0,0 -d 10 -f S16_LE -r 8000 -c 2 test.pcm
```

其中 amixer 和 arecord 均从ambaralla 官方SDK中获取，拷贝进入 VFS后运行。
`arecord` 需要通过`ln` 建立 `aplay` 的链接后运行

#### 2021-8-13

1. 参考 ALSA 官方说明，加入初始化中 ctrl 修改过程
主要添加函数 `amba_audio_ctrl_value_set`  `amba_auio_set_snd_ctrl`

2. 修改整体 音频过程，将 录音 与 播放 环回整体调通。

#### 2021-8-14 ~ 2021-8-17

调试 motion dection 相关,添加文件 `amba_mdt` `tvt_sdk_motion_dt`

思路为 引入smartlib 库， 配置其roi level ，打开线程后通过读取 qp roi matrix 信息反映motion 信息

需要 引入 两个头文件 `libsmartrc.h` `libsmartrc_common.h` 
和相关库 libsamrtrc_2_0 以及 preanalysis相关

最后测试得到 1280x720 下，二值图像与实际motion 成功对应。  

#### 2021-8-18

与安霸技术支持沟通后，舍弃上述方案，启用新的方案，基于opencv 的 libmdet.so 。

需要引入库 `libopencv_core.so` `libopencv_imgproc.so` `libtbb.so` `libz.so` `libmdet.so` 

该库没有 sample， 对应如下目录中程序修改测试该库`ambarella/oryx_v2/base/video/control/vca/plugins/motion_detect`

#### 2021-8-19~2021-8-20 

初版即SDK 3.0.3 的版本下，libmdet 不支持该平台，后联系技术支持获取了 3.0.6 版本的 libmdet 库。

大致搭建完整体 motion detect 的API。

sdk_func.c 中 通过 sdk_thread 反复调用 loop data 

接口写在 sdk_device.c 中

tvt_sdk_motion_dt 相关文件 维护锁和整体控制过程。

amba_mdt 文件 维护与 mdetlib 交互的底层代码

目前过程为 ,从 alarm jpeg 通道 获取 yuv me0 me1 三种数据，目前选用压缩率最高的 me0(1/8)，同时修改alram jpeg 通道buffer 和 canvas 的分辨率为 704x576,

对应 me0 数据为 88x72

在me0 数据上按像素点统计motion 移动信息。

得到 (int*)类型连续数据,每4个字节为0则为静止，否则为移动。(88x72)

接着 手动按序4x4 访问 88x72 的motion 数据。 每16个像素点 若有9个以上的像素点 存在motion 则返回认定该 块 为移动块,最终得到 22x18 的块移动分布.

#### 2021-8-23

针对motion 完成 describe获取 接口 和 setconfig 接口。
其中 灵敏度取值范围为 1--7。 对应 mdet lib 的 threshold 取值范围 64--2。

开始测试 电机模块

#### 2021-8-24

测试通过 电机模块，为SPI控制。

1. 寄存器配置： __位顺序为 lsb-first, 收发时注意调整顺序。 模式为CPOL=1, CPHA=1__
通过 SPI 与 电机控制芯片 MS41909 建立通信，一次通信发送24bit。 3个字节。

`[0]-[5]`: 寄存器地址
`[6]`: 1 表示读， 0表示写
`[7]`: Reserved
`[8]-[23]`: 16位寄存器数据。

目前配制的寄存器如下(仅用以测试电机正常运转，具体值并未核对)
```C
    spi_write(0x0b, 0x0280);
    spi_write(0x20, 0x5c0a);

    spi_write(0x22, 0x000d);
    spi_write(0x23, 0xe0e0);
    //0x24 last HEX byte [35XX], XX indicates the A motor's steps, if you want to stop it, set as 00.
    spi_write(0x24, 0x350f);
    spi_write(0x25, 0x0162);

    spi_write(0x27, 0x000d);
    spi_write(0x28, 0xe0e0);
    //0x29 last HEX byte [35XX], XX indicates the B motor's steps, if you want to stop it, set as 00.
    spi_write(0x29, 0x350f);
    spi_write(0x2a, 0x0162);
```

2. 通过 gpio 70 给与 VB 信号，触发电机工作。

#### 2021-8-25

测试 GPIO 相关模块，均没有发现问题。结果记录在excel 表格内，并未发现问题。

#### 2021-8-26

调整motion 数据源，新增加 pass 1 的buffer 2，704x576 or 704x480 两种模式，由于 手动模式(mannual feed)仅支持配置一个,采用自动数据流。
canvas(缓存)并未配置对应Stream用以编码，取其原始数据中ME0数据用以进行运动检测(YUV数据中Y分量压缩1/8得到)。

#### 2021-8-27

测试并完成PWM 模块。

PWM驱动控制：
在`sys/class/pwm` 内找到对应芯片，本设备为`pwmchip0`, 进入文件夹后,先使能对应通道,
本设备共有3个pwm 通道。使能过程如下
```Shell 
echo 0 > export
echo 1 > export
echo 2 > export
```

使能后，目录中出现`pwm0 pwm1 pwm2`等新目录，通过对目录下的 enable、period 等文件写入值，来控制对应 `pwm` 的接口。

修改 `amba_device` 中pwm 接口, 添加使能，去使能。

过程中注意： 错误状态下写入会直接导致 段错误！ 举例 `pwm0/enable` 内的值已经为0了, 这个时候想关闭pwm接口再次写入 0, 会直接报 段错误。

#### 2021-8-28

测试HDR 相关， 修改HDR mode 为 2X 后，  IAV_IOC_ENABLE_PREVIEW 失败，

针对 aaa_img_flow lib 报错，修改 aaa prepare 的过程，在aaa prepare 过程中添加 exp1_front_wb_gain 相关配置 和 hdr_blend 相关配置, 目前配置值均取 范围内中值

aaa 不报错状态下，仍然存在原问题，定位到 iav 驱动中，为 等待  vin captrue state 进入 standby state 超时。 猜测为 vin 配置错误，在jira上提问技术支持。

#### 2021-8-30

追踪驱动代码，定位HDR 模式下 存在 DSP 内存不足问题，编译内核驱动时，分配为320MB。 修改 amba_vproc_start() , 压缩了两个视频子码流的缓存分辨率，以及jpeg 通道一的buffer 和 缓存分辨率。

接着测试发现 vin 的数据无法进入 canvas 缓存中，追踪代码发现 aaa 传回的信息被识别为直接舍弃。修改 aaa prepare 的初始化过程， 把 hdr_blend 的参数enable 置为0， 测试通过。可以正常获得 hdr 码流并播放。

注意： HDR 模式配置后，实际vin -> canvas 的逻辑初始化在 `IAV_IOC_ENABLE_PREVIEW` 中完成。

