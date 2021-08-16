# OHSP 启动流程提速

## Uboot 阶段

修改 `u-boot/common/autoboot.c`

注释掉__abortboot，强制autoboot直接执行bootcmd.

```C 
static int abortboot(int bootdelay)
{
    int abort = 0;

// wkl
    // if (bootdelay >= 0)
    //  abort = __abortboot(bootdelay);

#ifdef CONFIG_SILENT_CONSOLE
    if (abort)
        gd->flags &= ~GD_FLG_SILENT;
#endif

    return abort;
}
```

## liteos-a 内核启动的C语言阶段

`kernal/liteos-a/platform/main.c` 为启动开始文件

启动开始产生如下输出

```
******************Welcome******************

Processor   : Cortex-A7 * 2
Run Mode    : SMP
GIC Rev     : GICv2
build time  : Jul  9 2021 15:31:25
Kernel      : Huawei LiteOS 2.0.0.37/debug

*******************************************

main core booting up...
```

过程中` SystemInit()` 占用时间较长，源文件为 `device/hisilicon/hi3516dv300/sdk_litos/mpp/module_init/src/system_init.c`

其中配置参数可通过修改`OHOS/kernel/liteos_a/tools/build/config` 中对应文件实现。

SystemInit() 函数下

因Camera没有SPI Flash，`spinor_init()`时间较长。

`net_init()` 时间较长。

该函数将线程锁住，等待回调，占用时间 0.5s 左右。

修改net_init()如下:

```C
void net_init(void)
{
    //wkl
    // struct netif *pnetif;

    extern void tcpip_init(tcpip_init_done_fn initfunc, void *arg);
    dprintf("\ntcpip_init start\n");
    tcpip_init(NULL, NULL);
    dprintf("\ntcpip_init end\n");

    // wkl
    // static unsigned int overtime = 0;
    PRINTK("Ethernet start.");

#ifdef LOSCFG_DRIVERS_HIGMAC
    extern int ethnet_higmac_init(void);
    (void)ethnet_higmac_init();
#endif

#ifdef LOSCFG_DRIVERS_HIETH_SF
    extern int ethnet_hieth_init(void);
    (void)ethnet_hieth_init();
#endif

//wkl
    /*    extern void get_defaultNetif(struct netif **pnetif);
        get_defaultNetif(&pnetif);

        (void)netifapi_netif_set_up(pnetif);
        do {
            LOS_Msleep(SLEEP_TIME_MS);
            overtime++;
            if (overtime > NETIF_SETUP_OVERTIME) {
                PRINTK("netif_is_link_up overtime!\n");
                break;
            }
        } while (netif_is_link_up(pnetif) == 0);*/
}
```

`OnMountRootfs()` 绑定文件系统，因为没有烧录userfs，所以报错如下

```
[ERR]Failed to mount /storage, errno 2: No such file or directory
```

`SDK_init` 时间长

源文件为`device/hisilicon/hi3516dv300/sdk_litos/mpp/module_init/src/sdk_init.c`

输出如下
```

==========chip: hi3516dv300==========
==========sensor0: os05a==========
==========sensor1: os05a==========osal_proc_mkdir - parent is NULL! proc=0x40667324 
g_mmz_start=0x88000000, g_mmz_size=0x180
mmz param= anonymous,0,0x88000000,384M
<6>Hisilicon Media Memory Zone Manager
load sys.ko for Hi3516CV500...OK!
load region.ko for Hi3516CV500...OK!
load gdc.ko for Hi3516CV500...OK!
load vgs.ko for Hi3516CV500...OK!
load dis.ko for Hi3516CV500...OK!
load vi.ko for Hi3516CV500...OK !
load isp.ko for Hi3516CV500...OK !
load vpss.ko for Hi3516CV500...OK!
load vo.ko for Hi3516CV500...OK!
load chnl.ko for Hi3516CV500...OK!
load vedu.ko for Hi3516CV500...OK!
load rc.ko for Hi3516CV500...OK!
load venc.ko for Hi3516CV500...OK!
load h264e.ko for Hi3516CV500...OK!
load h265e.ko for Hi3516CV500...OK!
load jpege.ko for Hi3516CV500...OK!
load jpegd.ko for Hi3516CV500...OK!
load vdec.ko for Hi3516CV500...OK!
load ive.ko for Hi3516CV500...OK!
load nnie.ko for Hi3516CV500...OK!
load tde.ko for Hi3516CV500...OK!
Load hifb.ko OK!
Do not support in hmos fun[osal_register_reboot_notifier]
load hi_piris.ko for Hi3516CV500...OK!
load hdmi.ko for Hi3516CV500...OK!
load mipi_rx driver successful!

```

## 第一个用户进程 init 进程启动

位置 `/ohos/base/startup/init_lite` ，接着快速进入shell 系统

