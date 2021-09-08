#Harmony 1.1.0 I2C调试记录

## 关键

+ 鸿蒙系统驱动框架为HDF(Harmony Drivers Framework)。具体详见docs/HarmonyOS驱动加载过程分析.pdf 
+ 鸿蒙内置了通用器件的驱动，如GPIO、I2C等等，可在$Harmony/drivers/framwork/下寻找，如I2C的接口头文件为i2c_if.h。见file/i2c_key_file/
+ 通用器件的驱动本身不一定能适配所有芯片，可以单独针对性在HDF框架下开发适配。本次调试使用的Hi3516DV300 I2C的寄存器信息已经内置于HarmonyOS 1.1.0 文件内，见file/i2c_key_file/i2c_config.hcs
+ HDF本身在鸿蒙系统中为一个子系统，且驱动加载后，上层（用户态）无法直接访问底层（内核态）代码过程，需要通过加载系统服务来完成接口调用。
+ 调试过程因为权限等问题，所以在仅有内核的情况下添加了shellcmd 的debug指令 wkldebug，具体文件见file/shellcmd,方法见：https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/kernel/%E8%B0%83%E6%B5%8B.md

## 实现源码
---
####application/wkl_debug(自由命名)
用户态下的测试应用

```
    i2c.busId = I2C_BUSID;
    i2c.addr = (OS05A_I2C_ADDR >> 1);
    i2c.handle = I2cOpen(i2c.busId);
```
调用接口，打开I2C0，底层代码为寻找驱动服务中对应I2CMannager。

+ I2C初始化时，地址要右移一位（8位->7位) ,对应os08a 地址 (0x6c >> 1)

```
    bufWrite[index++] = ( regAddr >> 8 ) & 0xFF;
    bufWrite[index++] = ( regAddr ) & 0xFF;

    msgs[0].buf = bufWrite;
    msgs[0].len = 2;
    msgs[0].addr = (i2c-> addr);
    msgs[0].flags = 0;
```
Write 和 Read 前都需要先向总线上写入这次操作的地址，对应os08a，地址长度为2，例如 0x4600。

+ msgs 的具体定义见i2c_if.h。
+ &0xFF 操作为保证截取出 16位即2个字符对应1字节。
+ flags = 0 表示写入操作

```
        bufWrite[index++] =  regData[0] & 0xFF;
        msgs[0].buf = bufWrite;
        msgs[0].len = 3;
        msgs[0].addr = (i2c-> addr);
        msgs[0].flags = 0;
        ret = I2cTransfer(i2c->handle, msgs, 1);

```

Write 流程为在原有msg上再补上一字节长度的数据 （`regData[0]`）
总长度为3一次性调用I2cTransfer 写入

```
    msgs[1].buf = regData;
    msgs[1].len = dataLen; // 1
    msgs[1].addr = (i2c-> addr);
    msgs[1].flags = (flag == WKL_DB_I2C_READ) ? I2C_FLAG_READ : 0 ;
    if (flag == WKL_DB_I2C_READ ) {
        ret = I2cTransfer(i2c->handle, msgs, 2);
    }
```

Read 流程为加入新的msgs，flags 为 I2C_FLAG_READ (i2c_if.h 中定义)。 
regData作用为存储数据。两个msgs一起发送。

---

####i2c_if.c  接口
```
static void *I2cManagerGetService(void)
{
    static void *manager = NULL;

    if (manager != NULL) {
        return manager;
    }
    manager = (void *)HdfIoServiceBind(I2C_SERVICE_NAME);
    if (manager == NULL) {
        HDF_LOGE("I2cManagerGetService: fail to get i2c manager!");
    }
    return manager;
}
```
此函数获得i2c_core中提供的驱动服务，三个接口函数(Open,Close,Transfer)都为调用驱动接口过程（需使用hdf_sbuf格式传输信息）。使用方法见头文件。

---

####i2c_core.c 驱动服务文件及实现

```
struct HdfDriverEntry g_i2cManagerEntry = {
    .moduleVersion = 1,
    .Bind = I2cManagerBind,
    .Init = I2cManagerInit,
    .Release = I2cManagerRelease,
    .moduleName = "HDF_PLATFORM_I2C_MANAGER",
};
HDF_INIT(g_i2cManagerEntry);
```
文件最后为驱动绑定信息。 可以沿此分析源码。

关键全局变量：
`static struct I2cManager *g_i2cManager = NULL;`
