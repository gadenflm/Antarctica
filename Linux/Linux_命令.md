[TOC]

## 常用

#### 剪切/重命名 mv

#### 建立link ln

`ln [-d -s] [SouceFile or Dir] [TargetLink]` 

* `-d`  目录类型link
* `-s` 

#### 压缩/解压缩 tar

__常用__：
```

```

## 查询

#### 内容检索 grep
```Shell
grep [-r][-i][-c][-l] ["Sample String"] [filename]
```

* `-r` 递归（recursion），遍历子文件夹
* `[filename]` 常用 `*`
* `-i`  忽略大小写的不同!
* `-c` 计算找到 'Sample String' 的次数
* `-l` 不显示文件行号内容，只显示文件名

e.g.
```Shell
grep -r -l "[Wkl Debug]" *
```

#### 文件检索 find

#### 符号表信息列举(可用于查询.o .so .a中的函数信息) nm

## 逆向

#### 反汇编指令 objdump

```Shell
objdump [-arg] [filename]
```
仅列举个人常用：

* 针对动态库(so)：
    - `-t` 显示symbols 符号表信息
    - `-T` 显示文件的动态表符号入口（仅用于动态文件）
    - `-r` 显示文件的重定位入口
    - `-R` 显示文件的动态重定位入口（仅用于动态文件）

__查询so文件函数信息：__
```Shell
objdump -tT SampleSoName.so
```

* 针对中间代码文件(.o):
    - `-t` 显示symbols 符号表信息，__查询函数名信息__
    - `-s` 左侧显示二进制文件对应16进制全文，右侧显示解析
    - `-r` 显示文件的重定位入口

* 针对二进制执行文件(.bin):
    - `-t` 显示symbols 符号表信息，__查询函数名信息__
    - `-s` 左侧显示二进制文件对应16进制全文，右侧显示解析
    - `-h` 显示目标文件各个section的头部摘要信息。 

## 系统信息

#### 查询磁盘使用情况 df 

```Shell
df [-h] [-k] [-i] [--blocksize={SIZE}] 
```

(disk free) 命令用于显示目前在 Linux 系统上的文件系统磁盘使用情况统计。

* `-h` __(常用)__ (`--human-readable`) 使用人类可读的格式
* `-i` __(常用)__ 列出 inode 信息
* `-k` --kilobytes 就像是 --block-size=1024
* `--block-size={SIZE} ` 使用 {SIZE} 大小的 Blocks

#### 网络配置

##### ifconfig(查询 / 配置 mac地址、ip协议相关)

```Shell
ifconfig [-a]

#example:
ifconfig eth0 down #关闭网卡

ifconfig eth0 10.10.100.113 netmask 255.255.240.0 hw ether 00:18:ae:c1:b2:a3  #配置mac地址、ip地址、子网掩码

ifconfig eth0 up #启动网卡
```

##### route 查询路由信息、配置网关

```Shell 
route [-n]

#example:
route add default gw 10.10.0.1 dev eth0 #添加默认网关
```

#####  ethtool 查询和设置网卡参数

```Shell
#example:
ethtool eth0        #查询网卡参数
ethtool –S eth0     #查询eth0网口收发包统计
ethtool –s eth0 [speed 10|100|1000] [duplex half|full]  [autoneg on|off] #设置网口速率、网口半/全双工、是否自协商
```

## 进程管理

#### 进程状态 ps 
Process Status，用于显示当前进程的状态
```
ps [-options] 
```

仅列举个人常用：

* `-A` 列出所有的进程 (包括已终止的)
* `-w` 加宽显示 (信息更加详细)
* `-aux` 详细显示所有包含其他用户的进程
* `-u [UserName]` 显示特定用户的进程
* `-f`  包括详细信息

#### 作业控制系统

启动、停止、终止以及恢复作业的这些功能统称为作业控制。

1. __相关概念__  
<font color="red">
shell ：命令解释器，其实就是一个脚本语言解释器，最常用的是bash.  
__job(作业): 是相对shell 来说的，在shell中执行一条命令，实际上就是提交了一个job,只不过有的job需要运行很长时间，有的job很快就结束。__  
__进程：命令经过shell解析后，交给系统内核执行，会fork出很多进程，他们同属于登录shell的进程组。__  
</font>

作业是shell里面的一个概念，__jobs命令只能查看当前shell自己的job，不同的shell 之间相互隔离。__  我们在shell里执行一条命令的时候就开启了一个作业，一个作业对应着系统中的一个进程组，__如果作业中的某个进程又创建了子进程，则子进程不属于作业。__

2. 作业分类

* __前台作业：需要占用终端的输入、输出。__
* __后台作业：命令后面加&,后台job不占用shell的输入输出。__ 例如：`./Sample.bin&`

__一个shell在某一时刻可以同时运行一个前台作业和多个后台作业。__

3. __相关操作__

* `Ctrl+Z` 可以将一个正在前台执行的命令放到后台，并且处于暂停状态。
* `Ctrl+C` 终止Shell 当前前台作业
* `[Command]&` Shell 后台运行Command

##### 查看作业 jobs
`jobs` 显示的是当前shell环境中所起的后台正在运行或者被挂起的任务信息  
* `+` 表示是一个当前的作业
* `-` 表是一个当前作业之后的一个作业

```Shell 
jobs [-l]
```
* `-l` 将显示作业的PID. (默认仅显示作业号)

##### fg/bg 控制作业状态

```Shell
 fg %[JobNumber]
 bg %[JobNumber] 
```
jobnumber是通过jobs命令查到的后台正在执行的命令的序号

* `fg` 将后台中的命令调至前台继续运行
* `bg` 将一个在后台暂停的命令，变成继续执行

##### 不挂断地运行命令 nohup
```Shell
nohup [Command] [Arg ...] [&]
```

nohup 命令(No Hang Up)运行由 Command 参数和任何相关的 Arg 参数指定的命令，忽略所有挂断（SIGHUP）信号。

__如果你正在运行一个进程，而且在退出帐户时该进程不想结束，使用nohup命令。该命令可以在你退出帐户/关闭终端之后继续运行相应的进程。__

无论是否将 nohup 命令的输出重定向到终端，输出都将附加到当前目录的 `nohup.out` 文件中。如果当前目录的 `nohup.out` 文件不可写，输出重定向到 `$HOME/nohup.out`文件中。如果没有文件能创建或打开以用于追加，那么 Command 参数指定的命令不可调用。

__`nohup.out` 文件可以理解成守护进程的 log 的文件__

#### 终止进程 Kill

## 安装

#### Debian安装管理 dpkg (Ubuntu)

```Shell
dpkg -i [deb file]
dpkg [-L] [-l] [package name]
dpkg [-r] [-P] [package name]
```

dpkg 是Debian package的简写，为Debian开发的套件管理系统，用于软件的安装，更新和移除。
Ubuntu 基于 Debian。

* `-i` __(常用)__ 安装目标deb文件
* `-l` __(常用)__ 参数不给特定包时显示系统所有已安装package及版本信息。 给target package显示对应包的信息
* `-L` __(常用)__ 显示与 target package 的全部相关文件
* `-r` 删除目标 package, 保留配置文件
* `-P` 删除目标 package, 清除配置文件
