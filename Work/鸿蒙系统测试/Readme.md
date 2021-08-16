#鸿蒙系统测试报告
---
## 使用的测试 shell 指令

###cpup
`cpup [mode] [taskID]` 查询系统CPU的占用率。

mode：

+ 0 显示系统最近10s内CPU占用频率
+ 1 显示系统最近1s内CPU占用频率
+ 其他 显示系统至今总的CPU占用频率

taskID：任务ID号

###free
`free \[-k|-m\]` 

free命令可显示系统内存的使用情况，同时显示系统的text段、data段、rodata段、bss段大小。

+ 无参数 以Byte为单位显示
+ -k 以KB为单位
+ -m 以MB为单位

###systeminfo
`systeminfo` 
用于显示当前操作系统内资源使用情况，包括任务、信号量、互斥量、队列、定时器等。

---
## OpenHarmony 1.1.0 官方原版编译测试
烧写文件目录：./Off_110_Out/

```
>OHOS # cpup 
>OHOS # cpup -1

SysCpuUsage in 10s: 1.89
SysCpuUsage in alltime: 2.57

>OHOS # free -k 

        total        used          free          heap
Mem:    130048       22424         107624        1809      

        text         data          rodata        bss
Mem:    5818         48            1135          1774   

>OHOS# free -m 

        total        used          free          heap
Mem:    127          22            105           2         

        text         data          rodata        bss
Mem:    6            0             1             2    

>OHOS # systeminfo

   Module    Used      Total     Enabled
--------------------------------------------
   Task      62        128       YES
   Sem       374       1024      YES
   Queue     5         1024      YES
   SwTmr     11        1024      YES

```

---

## OpenHarmony 2.0.0_Canary 官方原版编译测试
烧写文件目录：./Off_200_Out/

```
>OHOS # cpup 0
>OHOS # cpup -1

SysCpuUsage in 10s: 1.88
SysCpuUsage in all time: 3.29

>OHOS # free -k

        total        used          free          heap
Mem:    130048       22395         107653        1809      

        text         data          rodata        bss
Mem:    5818         48            1135          1774      

>OHOS # free -m

        total        used          free          heap
Mem:    127          22            105           2         

        text         data          rodata        bss
Mem:    6            0             1             2

>OHOS # systeminfo

   Module    Used      Total     Enabled
--------------------------------------------
   Task      62        128       YES
   Sem       374       1024      YES
   Queue     5         1024      YES
   SwTmr     11        1024      YES
```

---

## OpenHarmony 1.1.0 修改最小OHOS_Image版本编译测试
烧写文件目录：./Min_OHOS/
配置参数见： config.json

```

>OHOS # cpup 0
>OHOS # cpup -1

SysCpuUsage in 10s: 2.10
SysCpuUsage in all time: 2.57

>OHOS # free -k

        total        used          free          heap
Mem:    130048       14392         115656        941       

        text         data          rodata        bss
Mem:    5818         48            1135          1774

>OHOS # free -m

        total        used          free          heap
Mem:    127          14            113           1         

        text         data          rodata        bss
Mem:    6            0             1             2  

>OHOS # systeminfo

   Module    Used      Total     Enabled
--------------------------------------------
   Task      24        128       YES
   Sem       326       1024      YES
   Queue     5         1024      YES
   SwTmr     11        1024      YES

```

___

## liteos_a 内核编译测试
烧写文件目录：./kernel/
配置参数见： config.json

```

>OHOS # cpup 0
>OHOS # cpup -1

SysCpuUsage in 10s: 1.16
SysCpuUsage in all time: 2.14 

>OHOS # free -k

        total        used          free          heap
Mem:    130048       6624          123424        824       

        text         data          rodata        bss
Mem:    1369         32            525           1383 

>OHOS # free -m

        total        used          free          heap
Mem:    127          6             121           1         

        text         data          rodata        bss
Mem:    1            0             1             1

>OHOS # systeminfo

   Module    Used      Total     Enabled
--------------------------------------------
   Task      23        128       YES
   Sem       37        1024      YES
   Queue     5         1024      YES
   SwTmr     4         1024      YES
```