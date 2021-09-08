# 音频模块整理 

[TOC]

## 说明

安霸平台下, 音频 I/O, 编解码等 通过三方库 ALSA 访问驱动控制。

如果出现问题，可以考虑先用 ALSA 官方程序`amixer` `arecord`验证 驱动和硬件 是否正常工作。(`arecord` 需要通过`ln` 建立 `aplay` 的链接后运行)

__目前代码调用ALSA 操作的设备号为"default", 即该句柄下的参数会被修正后送入 驱动最底层，因此会出现设置参数与 `/proc` 目录下读取参数不同的情况__

参考：

* https://www.alsa-project.org/alsa-doc/alsa-lib/index.html
* https://alsa.opensrc.org/

## 流程

### 驱动配置值

先通过 `snd_ctl_elem_value_set_XXX()` 函数设定 驱动配置值，该接口调用需要 访问多层 `snd_ctl_XXX` 的结构体, 如果需要更改,请参照官方网站说明。

目前配置如下:

* `Left Line Mux` 设置为1 (该值为硬件接口序号，不可更改)
* `Analog In Volume` 设置为8, 范围`[0,8]`
* `Analog Out Volume` 设置为29, 范围`[0,29]`
* `Capture Volume` 设置为192, 范围`[0,192]`
* `Playback Volume` 设置为192, 范围`[0,192]`

### 通道配置值

Ap打开音频时 传给 SDK 的参数, 最终送入 `chip_audio_setParam()` 函数中，注意此部分配置如果平台不支持，可能被自动修正成近似值而不会出现报错

同时 __会根据设定值 自动计算软硬件参数, 主要包括 帧的数据大小 块的数据大小 等等, 上层AP 每次提取 一块, 开辟空间需要与这部分值相对应。(chunk_size)__

### 录音与播放

`snd_pcm_readi` `snd_pcm_writei` 两个接口实现。 注意录音时会出现原始数据送入过快导致 Buffer 区已满的情况，此时会调用 `xrun()` 根据情况修正驱动, 实际使用中应尽量避免反复触发此种情况。

## 补充

### 单声道 与 双声道 可通过如下方式分离

```C 
    const unsigned int size = g_chunk_bytes;
    char* buffer = (char*) malloc (size);

    // TODO: 填充音频数据进入 buffer

    const unsigned int half_size = g_chunk_bytes >> 1;
    char* left_buff = (char*) malloc (half_size);
    char* right_buff = (char*) malloc (half_size);

    memset(left_buff, 0, half_size);
    memset(right_buff, 0, half_size);

    for (int i = 0; i < (half_size / 2); i++) {
        *(left_buff + i * 2) = *(buffer + i * 4 );
        *(left_buff + i * 2 + 1) = *(buffer + i * 4 + 1);

        *(right_buff + i * 2) = *(buffer + i * 4 + 2);
        *(right_buff + i * 2 + 1) = *(buffer + i * 4 + 3);
    }

```

