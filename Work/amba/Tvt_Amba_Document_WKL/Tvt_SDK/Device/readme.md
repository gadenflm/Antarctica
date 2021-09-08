#Amba Tvt IPC SDK  部分外设接口记录

## SPI 模块 (控制电机)

思路为 通过 SPI 与 电机控制芯片 MS41909 建立通信，一次通信发送24bit。 3个字节。

根据 MS41909 芯片文档 和 板子 dts。

SPI 接口配置:
__位顺序为 lsb-first, 收发时注意调整顺序。__  
__模式为CPOL=1, CPHA=1。__  
一次发送三个msg,每个msg均为一个字节即8位,分配如下

`[0]-[5]`: 寄存器地址
`[6]`: 1 表示读， 0表示写
`[7]`: Reserved
`[8]-[23]`: 16位寄存器数据。

__具体寄存器和对应值需要参考 MS41909 芯片手册 设定,测试时初始化值如下(仅用以测试电机正常运转，具体值并未核对):__

```C
    spi_write(0x0b, 0x0280);
    spi_write(0x20, 0x5c0a);

    spi_write(0x22, 0x000d);
    spi_write(0x23, 0xe0e0);
    //0x24 HEX byte [35XX], XX indicates the A motor's steps, if you want to stop it, set as 00.
    spi_write(0x24, 0x350f);
    spi_write(0x25, 0x0162);

    spi_write(0x27, 0x000d);
    spi_write(0x28, 0xe0e0);
    //0x29 HEX byte [35XX], XX indicates the B motor's steps, if you want to stop it, set as 00.
    spi_write(0x29, 0x350f);
    spi_write(0x2a, 0x0162);
```

配置完成后，需要通过芯片 gpio 70 给与 VB 信号，触发电机工作。(交替给高低电平模拟信号)

## PWM 模块

PWM0 控制 红外, PWM1 和 PWM2 控制近光远光灯。

PWM 控制文件位于 `sys/class/pwm`, PWM0, PWM1, PWM2 均为 pwmchip0 控制。 

先使能 3 个通道, 即写入 通道号 至文件 export
``` Shell
echo 0 > export
echo 1 > export
echo 2 > export
```

使能后，目录中出现`pwm0 pwm1 pwm2`等新目录，通过对目录下的 enable、period、duty_circle 等文件写入值，来控制对应 `pwm` 的接口。

__period / duty_circle 的单位为 纳秒, 因此当写入值过小时, pwm模块无法正常工作。 其临界阈值目前并未找出,测试在 5000~50000 中间__

__过程中注意： 程序中写入错误会直接导致 段错误！ 举例 `pwm0/enable` 内的值已经为0了, 这个时候想关闭pwm接口再次写入 0, 会直接报 段错误。 同理超出范围写入值, 如写入 0 到 period, 也会报 段错误。 因此要小心该接口的参数值__

目前 Tvt IPC SDK 仅提供写入 period / duty_circle 的接口，而对外接口不支持 enable 的写入。


