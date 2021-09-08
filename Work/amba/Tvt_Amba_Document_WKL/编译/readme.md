## 编译环境搭建

__参考:__  

* _`Ambarella_CV2x_UG_Flexible_Linux_SDK3.0_Code_Building_and_Debug_Environment.pdf`_
* _`Ambarella_CV2x_UG_Flexible_Linux_SDK3.0_EVK_Getting_Started_Guide.pdf`_

[TOC]

### CIFS 文件系统相关
其中共享文件系统使用 CIFS, 板子启动后需要配置网络环境, 挂载共享文件系统, 若正常烧写 APFS 后, `#Tvt_Board#/mnt/cfg/ext/bugbt.sh` 为启动时初始化 Shell 脚本， CIFS及 网络配置等封装在其中。

```Shell
# 网络配置举例 ( ip 地址 / 网关 / MAC地址 )
ifconfig eth0 down
ifconfig eth0 10.10.100.113 netmask 255.255.240.0 hw ether 00:18:ae:c1:b2:a3
ifconfig eth0 up
route add default gw 10.10.0.1 dev eth0

# CIFS 挂载指令 (用户名 密码 主机地址 等需要根据实际情况更换)
mount -t cifs -o domain=ambarella,sec=ntlmssp,username=dell,password=xxx,uid=0,gid=0,file_mode=0755,dir_mode=0755 //10.10.100.111/share /mnt/nfs
```

### 编译脚本

编译相关脚本已经封装在 `build_release.sh` 中, 其后续 args 参数分别对应以下编译过程:

* `sdk` 安霸官方 SDK 编译,输出包含 内核及驱动, 文件系统相关, AMBOOT(等价于Uboot), 各种 AMBA官方库及第三方库    
    相关脚本 `platform/ambarella/build.sh


* `so` tvt IPC SDK 编译,输出为 `libsdk.so` `unit_test`(测试程序)  
    相关脚本`driver/Sdkfunc/libsdk/build.sh`

* `img` __(已修改,默认tvt_build内方案舍弃)__ 打包得到文件系统,AP文件,uboot等等  
    相关脚本 `burn_img/ambarella/build.sh`
            `burn_img/ambarella/script/*.sh`

### 编译工具链

`linaro-aarch64-2020.09-gcc10.2-linux5.4`, 推荐安装在目录 `/opt/ambarella_tools/tools`

工具链指定可在 `platform/ambarella/build.sh` 做如下修改
```Shell
#platform/ambarella/build.sh

#Line 121
if [ -d $SDK_NAME/ambarella/boards/$CHIP_NAME ];then
    cd $SDK_NAME/ambarella/boards/$CHIP_NAME
    export LINUX_TOOLCHAIN_PREFIX=/opt/ambarella_tools/tools
    # export TOOLCHAIN=1
    ...
fi
```

### 编译参数 及 硬件相关配置

目录 `ambarella/boards/cv28m_cashewnut/` 下存放 E4H_4M 编译相关配置文件。

常用:

* `./config/tvt_ipcam_config` 编译config (可修改内存区大小, 功能使能...) 
* `./bsp/cv28m_cashewnut.dts` device tree文件, 存放 内外设 硬件配置 (i2c,SD,PWM,eth0...)
* `./bsp/bsp.h` 存放 EMMC 分区信息

### 部分已知可能存在的问题

#### libasound.so 缺失

音频相关三方库拷贝过程存在问题,需要`platform/ambarella/build.sh`做如下修改, 否则 tvt IPC SDK 编译会报错。

```Shell
#platform/ambarella/build.sh
#Line 75 -
cp -a fakeroot/usr/lib/libasound.so.2 $PLATFORM_OUTPUT_DIR/library/libasound.so

#Line 75 +
cp -a fakeroot/usr/lib/libasound.so.2.0.0 $PLATFORM_OUTPUT_DIR/library/libasound.so
```

注意检查 soft linked 文件和 shared library 文件区别。

---

#### 修改源码后编译输出测试没有改变

原 tvt_build 下 编译官方sdk 时会重写解压补丁覆盖部分源文件, 可以把这个过程干掉

```Shell
#platform/ambarella/build.sh
#Line 114
    cd $SDK_ROOTPATH/$SDK_SRC_DIR
    # wkl
    # for file in $PATH_LIST
    # do
    #   echo "patch $file"
    #   tar -zxf $SDK_ROOTPATH/$SDK_SRC_DIR/patch/$file -C ./
    # done
```

---

#### libsdk.so / unit_test 编译时提示 库文件/头文件 缺失

默认 libsdk.so / unit_test 库文件存放位置为 `tvt_build/output/E4H_4M/cv28m_cashewnut` 下的 `library` `include` 文件夹。

后又增加 `tvt_build/driver/Sdkfunc/libsdk/hardware/ambarella/extlib` 

可去官方目录下寻找对应库文件放入。

同时可能需要修改 `libsdk/Makefile.Amba` `unit_test/Makefile` 等等,添加对库文件的引用

注意检查 soft linked 文件和 shared library 文件区别。

### 编译输出 

`tvt_build/output/E4H_4M/cv28m_cashewnut/` 下为 安霸官方 SDK 编译结果的拷贝。  
其中 __`./AmbaUSB` 下为可以通过 安霸官方烧录工具烧录的二进制文件。常用的3a 等库会拷贝至`./library` 下__

`tvt_build/driver/Sdkfunc/libsdk/output` 下为 libsdk.so 的输出目录

