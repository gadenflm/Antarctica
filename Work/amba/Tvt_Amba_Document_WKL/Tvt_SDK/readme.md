# Amba 个人接口整理

## 说明 
<font size="4.5">
    此部分接口为在原有 SDK_func 上添加或修改，测试多数都在 自己编写的测试程序中验证接口功能正常即通过，实际使用时可能出现问题。 同时部分接口存在未完成功能，在此记录。
</font>

## 部分接口随记

* OSD 接口 : 
    - 测试时发现存在传入参数的区域宽高均为 0 的情况，直接添加 `H\W <=0 ? continue; ` 的过程跳过，未追踪参数产生原因。

* Flash 存取参数 接口 :
    - 移植 Hisi SDK 的参数存取接口, 因目前分区 amboot 占用不足3MB, 而 kernel及以后分区 从 0x1300000 开始, 因此 Flash Cfg 的存取为 mmcblk0 开始，偏移量为 0x300000.
    - 过程中跳过了 encryption 相关，即加密与解密
    - 过程中跳过了 SPI相关(仅实现了emmc相关)

* Flash 升级 接口 :
    - 移植 Hisi SDK 升级接口, 因 Amba 平台 amboot 拆分成四个分区存取，跳过了Uboot升级, 需要在过程中补齐。
    - 过程中跳过了 encryption 相关，即加密与解密
    - 过程中跳过了 SPI相关(仅实现了emmc相关)

* HDR-2X 模式设置 :
    - 修改了 img_aaa_flow 的库文件源码，在aaa prepare 过程中添加 `ik_set_exp1_front_wb_gain()` 相关 和 `ik_set_hdr_blend()` 相关, __其配置值 随便给的,使用需要更正__ 。 测试时，发现 `hdr_blend()` 的`enable` 必须置0关闭。否则 vin 会 丢弃进 sensor 的每一帧而不送入 source buffer。
    - HDR 模式会占用 DSP Buffer 内存，目前320mB内存情况吃紧，若报错可以检查内存。
    - HDR 模式在高帧率的情况下, 可能会超出 芯片设定编码资源上限 而报错。

### [Face Dt 接口(YUV 数据存取/ EFM Stream)](FaceDt/readme.md)
### [Motion Dt 接口](MotionDt/readme.md)
### [Audio 接口](Audio/readme.md)
### [Device 接口](Device/readme.md)