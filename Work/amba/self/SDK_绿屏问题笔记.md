[TOC]

# unit_test 调用 libsdk.so, `fwrite()` 直接保存码流后播放测试

__unit_test测试下均不重启两个Jpeg码流，即 Stream 3 Stream4__

### 仅 APP配置Main Stream 0, OK
单 Main Stream, APP 传入配置测试没有问题
```C
s32Ret = SDKSetVEncPara(0, SDK_VENC_TYPE_H265, 2592, 1520, 25, SDK_VENC_MODE_VBR, 3072, 50, SDK_VENC_PROFILE_HIGH);
```

### APP配置Main Stream 0 \ Second Stream 1,  Error
此时第二码流输出正确， 主码流输出显示错误, 无报错

```C
s32Ret += SDKSetVEncPara(1, SDK_VENC_TYPE_H265, 704, 576, 25, SDK_VENC_MODE_VBR, 512, 50, SDK_VENC_PROFILE_HIGH);
```

### APP配置 Stream 0 , 修改配置 Stream 1, OK

Stream1 参数修改如下,仅修改分辨率和比特率

```C 
s32Ret += SDKSetVEncPara(1, SDK_VENC_TYPE_H265, 1280, 720, 25, SDK_VENC_MODE_VBR, 2048, 50, SDK_VENC_PROFILE_HIGH);
```

### APP配置 Stream 0 \ 2 , 修改配置 Stream 1, Error

__此时若仅get Stream0 和 1的结果并保存，两码流均无错误！！！， get Stream2 再保存后, Stream2 正确， Stream 0 和 Stream 1 出错__

```C 
s32Ret += SDKSetVEncPara(1, SDK_VENC_TYPE_H265, 1280, 720, 25, SDK_VENC_MODE_VBR, 2048, 50, SDK_VENC_PROFILE_HIGH);
s32Ret += SDKSetVEncPara(2, SDK_VENC_TYPE_H265, 704, 576, 25, SDK_VENC_MODE_CBR, 512, 50, SDK_VENC_PROFILE_HIGH);
```

### APP配置 Stream 0, 修改配置 Stream 1 \ 2 , OK

三个码流输出保存后均正确，但此时没有重启两个JPEG码流！！！！！！！


```C
s32Ret = SDKSetVEncPara(0, SDK_VENC_TYPE_H265, 2592, 1520, 25, SDK_VENC_MODE_VBR, 3072, 50, SDK_VENC_PROFILE_HIGH);
s32Ret += SDKSetVEncPara(1, SDK_VENC_TYPE_H265, 1280, 720, 25, SDK_VENC_MODE_VBR, 2048, 50, SDK_VENC_PROFILE_HIGH);
s32Ret += SDKSetVEncPara(2, SDK_VENC_TYPE_H265, 1280, 720, 25, SDK_VENC_MODE_VBR, 2048, 50, SDK_VENC_PROFILE_HIGH);
```