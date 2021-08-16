#amboot

###进入amboot方法(开发板 CV28M)

若无已有amboot烧录，现将POC[6] 打到 0，POC[10] 打到 1，安装驱动后，通过AmbaUSB烧录 elf 或者 bin文件。

烧录amboot后，将POC[6] 打到 1，POC[10] 打到 0, POC[5:4] = 10 时为emmc 启动，
POC[5:4] = 01 时为nand 启动，本次测试在emmc下。

连接串口，上电检验通信正常后，按住host下键盘上的enter，按下开发板上reset键。进入amboot。 使用`help` 和 `help [command]` 熟悉指令。 

### 网络配置指令

#### 显示网络信息

`show netboot`

#### 配置网络信息
```
<!-- 板端网络配置 -->
setenv eth 0 ip 10.10.100.112
setenv eth 0 mask 255.255.240.0
setenv eth 0 gw 10.10.0.1
setenv eth 0 mac 00:18:ae:c1:b2:a3 // 随便敲的

<!-- tftp服务器配置 -->
setenv tftpd 10.10.100.111

<!--测试链接 -->
ping 10.10.100.111
```

### tftp 烧写
```
tftp write E4H_4M_u-boot-ok.bin bst 3145728             #3*1024*1024
tftp write E4H_4M_uImage kernel 15728640                #15*1024*1024
tftp write E4H_4M_squashfs_rootfs rootfs 4194304        #4*1024*1024
tftp write E4H_4M_squashfs_appfs appfs 50331648         #48*1024*1024
```

### tftp 修改
<font color="red" size="4">
`tvt_build/platform/ambarella/V3_00_003/cv2x_linux_sdk_3.0/ambarella/amboot/src/bld/cmd_tftp.c`

中修改了amboot tftp指令，添加了tftp write功能。具体添加如下
</font>

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


