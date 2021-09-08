# AmbaSDK 个人整理

[TOC]

## 重要目录

```
ambarella/
    amboot/                     # AMBOOT 相关
    boards/cv28m_cashewnut/     # 硬件配置相关
    include/                    # 通用头文件接口
    kernel/linux-5.4            # 内核源码
    kernel/private              # Amba相关 内核层代码, 包括iav驱动,cavalry驱动,SOC配置等等
    packages/                   # Amba 提供的开源库 源码。 包括 img_aaa,vproc等
    prebuild/                   # Amba 提供的非开源库 库文件。 包括 img_algo,mdet 等
    unit_test/                  # 测试程序
```

## iav 驱动

源码目录:`kernel/private/drivers/iav/arch_v5`

编译后输出目录: `out/cv28m_cashewnut/kernel/private/drivers/iav/arch_v5/iav.ko`

定位具体 ioctl 指令执行过程 可从`iav_enc_api.c` 中函数 `int iav_encode_ioctl(struct ambarella_iav **iav_context, unsigned int cmd, unsigned long arg)`  寻找, 其通过 `ambnl` 驱动与用户层程序通信。

`ambnl` 源码目录: `kernel/private/drivers/ambnl`

如果遇到 IAV_IOC_XXX 等驱动出错。
可结束程序运行，在 console 输入 `dmesg` 查看 内核层输出信息，定位问题原因。

### Tvt SDK 使用驱动注意

![](img\iav_state_model.jpg)

__iav 中很多限制均在 Doxygen 的 FEATURE SETS 说明,出错时可以对比查看。__

#### 句柄获得位置，在此之前不能调用驱动接口

获得句柄在 `amba_video.c` 中
```C
int chip_video_init(const CHIP_VIDEO_CFG_S* pstVideoCfg, int s32VinNum)
{
    ...
    // 打开iav模块
    if (g_stVideoCfg.s32Fd < 0)
    {
        g_stVideoCfg.s32Fd = open("/dev/iav", O_RDWR, 0);
        if (g_stVideoCfg.s32Fd < 0)
        {
            perror("/dev/iav");
            TIPC_PRINTF_ERR("/dev/iav open failed[%d]", g_stVideoCfg.s32Fd);
            return -1;
        }
    }
    ...
```

#### Vin 和 DSP 模块配置

__这两个模块的配置部分均为写入参数过程，会检查非法参数。但此过程必须保证 IAV 状态为IDLE, 故配置参数并未实际生效，调用 `IAV_IOC_ENABLE_PREVIEW` 过程,才会 使能3A lib库, 划分内存, 打开通道。 故部分配置报错可能反应在这个阶段__

* Vin 配置在 `amba_vin.c` 中，注意FPS为单独设定。
* DSP 模块配置主要集中在`amba_vproc.c` 中函数 __`vproc_start()`__

具体配置细节可参考 `Tvt IPC SDK 部分`。

__该模块若配置成功且正常运行 ，Vin --> Src_Buffer --> Canvas 的过程即正常。__

__此时,每个Canvas 中会缓存 4 帧 Yuv 数据。__

提取数据思路如下( 若Canvas 为manual feed即手动模式，需要修改过程。 手动模式下 Canvas不会自动存储 YUV 帧数据，而是需要调用特定接口抓取，获取后再Release掉 )

__一、映射 DSP_Buffer 内存__

每个Canvas 均在DSP Buffer 中开辟空间, 根据 分辨率, cached_items, extra_buffer 等配置占用不同。 一个 2592x1520 的 Canvas 会占用 25MB 内存空间，DSP_Buffer 总内存空间目前分配为 320 MB，使用时需要注意内存分配。

```C
{
    struct iav_querymem query_mem={0};
    struct iav_mem_part_info *part_info;

    query_mem.mid = IAV_MEM_PARTITION;
    part_info = &query_mem.arg.partition;
    part_info->pid = IAV_PART_DSP;
    AM_IOCTL(iav_fd, IAV_IOC_QUERY_MEMBLOCK, &query_mem);

    dsp_size = part_info->mem.length;
    dsp_vir_addr = mmap(NULL, dsp_size, PROT_READ, MAP_SHARED, iav_fd, part_info->mem.addr);
    if (dsp_vir_addr == MAP_FAILED) {
        TIPC_PRINTF_ERR("Can not mmap dsp partiton to virtual adress");
        return -1;
    }
    dsp_phy_addr = part_info->mem.addr;
}
```

`mmap` 的内存必须在 过程关闭时 `unmmap` 释放掉

__二、请求 Canvas 的信息__

每个 Canvas 的帧信息包含3个部分 YUV, ME1, ME0。 其中 YUV 为Source Buffer 传给 Canvas 的原始 YUV 数据。 ME1 为 宽高各压缩 1/4 的 Y 数据。 ME0 为 宽高各压缩 1/8 的 Y 数据。 

```C
{
    //请求
    struct iav_querydesc desc = {0};
    desc.qid = IAV_DESC_CANVAS;
    desc.arg.canvas.canvas_id = CANVAS_ID;
    desc.arg.canvas.skip_cache_sync = 1;
    desc.arg.canvas.non_block_flag = 0;
    AM_IOCTL(g_stMdtCfg.s32Fd, IAV_IOC_QUERY_DESC, &desc);
    //TODO: 检查参数

    //以 YUV 为例
    struct iav_yuv_cap *yuv_src;
    yuv_src = &desc.arg.canvas.yuv;
    height = yuv_src->height;
    width = yuv_src->width;
    pitch = yuv_src->pitch;
    frame_dts = yuv_src->dsp_pts;
    (uint8_t*)(ydata_vir_addr) = dsp_vir_addr + yuv_src->y_addr_offset;

    //TODO: 数据处理
}
```


#### 编码码流 Stream 设置

Stream 主要为从 Canvas 中提取数据的码流, 可配置成 `Preview` `Encode` 两种模式, 其中 `Preview` 主要作用是 提供给 Vout, `Encode` 对应编码过程。

(Stream 也可以配置成 `efm(encode from memory)` `efr(encode from raw)` 两种模式。 其原始帧不从 Canvas中获取 )

Encode Stream 存在大量可配置参数, 目前Tvt IPC SDK主要使用 H26X, MJpeg 两种编码格式。 其配置过程集中在 `amba_venc.c` 中, __其中 H26X 的 qp Mat等部分参数为直接套用 unit_test, 可能存在不合理之处__

获取编码后结果信息已经在 Tvt IPC SDK 中封装接口, 大致过程类似于 YUV 原始数据获取。

概况如下:
__先映射 BSB 部分内存得到虚拟地址后,  请求 编码结果信息，得到偏移量 时间戳等等，根据虚拟地址和偏移量 取得数据。__

### 常见错误

* `IAC_IOC_ENABLE_PREVIEW` 过程报错: 资源暂时不可访问, 注意 DSP_Buffer 内存分配大小为320 MB, 目前已经大面积占用, 申请新的 Canvas 或者 Stream 的过程均可能导致 内存不足。 通过 `dmesg | grep MB` 查看内存输出。

相关文档 _`Ambarella_CV2x_DG_Flexible_Linux_SDK3.0_DRAM_Optimization.pdf`_

* `修改视频配置后出错 Invalid Arguments` , 可检查Doxygen 文档中对应 FEATURE SETS 部分，以及其接口函数信息。 核对目前模式下,接口有无限制。 如 manual feed 模式的 Canvas 数量上限为1, 不同 Source Buffer 有缩放分辨率限制等等。 

## 官方 unit_test 测试方法

以下思路为在公司板子上, 通过 官方sample 还原 Tvt IPC SDK 思路:

__test_encode 默认会初始化 vout, 这个过程如果报错，可以在`test_encode.c`中干掉再编译运行__

1. 前置条件: 将库文件集中，并加入环境变量 `export LD_LIBRARY_PATH=/mnt/nfs/lib:$LD_LIBRARY_PATH`
2. 后台运行 3a 库的unit_test： `./test_aaa_service -a&`
3. 加载配置文件，初始化 iav, 配置文件为 `tvt.lua` : `./test_encode --resource-cfg tvt.lua`
4. 开启3个码流。 注意由于官方sample 里没有 2592x1520 的分辨率，此处用 1920x1080代替
``` Shell
./test_encode -A -h 1080p -b 1 -e 
./test_encode -B -h 1080p -b 2 -e 
./test_encode -C -h 720p -b 3 -e 

# test_encode 常用参数如下 
# -A 往后的配置在特定stream下 A 对应 Stream 0, -B -C -D -E 依次顺延 也可以用 -S [0~12]
# -h [resolusion] H264 模式，可选分辨见unit_test中 @stream_cfg.c __encode_res[]
# -H [resolusion] H265 模式，可选分辨见unit_test中 @stream_cfg.c __encode_res[]
# -b [CanvasId]   选择数据源 13为efm模式
# -e 开始编码
# -s 停止编码
```
5. 开启进程取stream数据并保存检查
```Shell 
./test_stream -f /mnt/nfs/&
```

__结束后需要关闭码流编码：`./test_encode -A -s`也要关闭对应进程`test_stream` `test_aaa_service`__