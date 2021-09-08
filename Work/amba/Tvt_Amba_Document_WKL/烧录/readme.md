# 烧录总结

[TOC]

## 通用烧录步骤

1. 使用官方工具 Amba-USB, 通过 USB 口烧录 AMBoot。
2. 板子端 Amboot 网络配置及 tftp 配置
3. 主机端 tftp 服务器配置
4. 通过指令烧写特定分区

## USB 口烧录 

__板子注意区分USB口和Debug口, Debug口为调试串口, 接错会导致板子烧糊。__

### AmbaUSB 烧录工具

本文版本: Ambarella_AmbaUSB_v4.2.8

安装包内有对应 Mac、Ubuntu、Windows 等系统版本, __注意需要安装对应驱动才能通过USB口识别到板子设备__ ,同时 __个人测试时发现仅能通过 USB2.0 口烧录, 若主机识别不到板子，可以换2.0口尝试。__

### USB烧录方法

__首先将板子 进入 USB Download Mode 启动: 按住板子背面的按钮(唯一)的状态下给板子上电即可__

若板子成功进入 USB Download Mode, 且驱动安装正确, 则可以在AmbaUSB 工具中看到对应板子信息，同时设备管理器中也会出现。

将编译后output目录下 `tvt_build/output/E4H_4M/cv28m_cashewnut/AmbaUSB/*.bin` 中选择想要烧录的部分进行烧录。

理论上只需烧录 AMboot 部分，即 `bst_bld_env_dtb_release.bin` 文件。  
若未对内核做出修改, 则内核部分也可通过USB口一同烧录进入。

__注意: Amba平台目前烧录 EMMC 为按分区烧录,  分区信息 见 `编译-- 编译参数 及 硬件相关配置`。 若烧录分区的文件大小超出分区大小(安霸官方SDK编译后输出的文件系统squashfs即超出大小)，则目标分区会烧录失败。此时AmbaUSB 会陷入反复烧录尝试死循环，此情况待第一次烧录完成后，直接断电即可。__

## Amboot

连接串口，__按住主机键盘上的enter，重新上电。进入amboot。__    
__使用`help` 和 `help [command]` 熟悉指令。__

### 常用指令

* `show meta` 查看 emmc 分区信息
* `show netboot` 查看网络配置信息
* `tftp write` tftp烧录指定分区
* `setenv` 配置环境变量

### 配置板子网络

指令参照如下

```Shell
# 板端网络配置
setenv eth 0 ip 10.10.100.112
setenv eth 0 mask 255.255.240.0
setenv eth 0 gw 10.10.0.1
setenv eth 0 mac 00:18:ae:c1:b2:a3  # 随便敲的

# tftp服务器配置
setenv tftpd 10.10.100.114

# ping 测试
ping 10.10.100.114
```

### tftp 烧写

主机端通过 tftp 工具指定ip地址和tftp服务器目录即可，板子端如下。

#### 烧写指令
```Shell 
# 格式
tftp write [文件名] [分区名字] [分区大小字节数]
# 示例
tftp write E4H_4M_uImage kernel 15728640      #15*1024*1024
tftp write flash_rootfs rootfs 4194304        #4*1024*1024
tftp write flash_appfs appfs 50331648         #48*1024*1024
```

#### 指令源码

该指令为 Tvt 自身添加指令
__`tvt_build/platform/ambarella/V3_00_003/cv2x_linux_sdk_3.0/ambarella/amboot/src/bld/cmd_tftp.c` 中修改了amboot tftp指令，添加了tftp write__ 

```C
if (strcmp(argv[1], "write") == 0)
    {
        // tvt add 2021-01-07
        const char *pfile_name = "bld_release.bin";
        const char* pszFlashType = "bld";
        u32 u32FlashSize = 0;
        u32 u32SaveAddr = MEMFWPROG_RAM_START;
        if (argc >= 5)
        {
            pfile_name = (const char *)argv[2];
            pszFlashType = (const char *)argv[3];
            strtou32(argv[4], &u32FlashSize, NULL);
            // 先清空内存区
            printf("clear mem area\n");
            memset((void*)u32SaveAddr, 0, u32FlashSize);
            // tftp 获取
            printf("recv file[%s]\n", pfile_name);
            ret_val = bld_netprogram(pfile_name, u32SaveAddr, 0, 1);
            if (ret_val == 0)
            {
                if (strcmp(pszFlashType, "all") == 0)
                {
                    // 烧写flash_all
                    /// TODO:未实现
                    printf("flash all...\n");
                    return ret_val;
                }
                // 写入emmc
                printf("program %s into mmc, flash size[%u]\n", pszFlashType, u32FlashSize);
                sdmmc_write_partition(pszFlashType, (u8*)u32SaveAddr, u32FlashSize, 1);
                printf("\n");
            }
        }
        else
        {
            printf("err command\n");
            ret_val = -2;
        }
    }
```

