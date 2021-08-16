[TOC]

---

###安装python3.8后,Ubuntu无法打开终端gnome-terminal

`/usr/bin/gnome-terminal`

上述文件第一行

`#!/usr/bin/python3` -> `#!/usr/bin/python3.6`

###linux终端在已经显示的输出中查找

在终端下，
<font color="red">  Ctrl + Shift + F </font>

### 空间不够时，清理回收站内容

回收站位置
`~/.local/share/Trash/`

* file 存放删除的文件
* info 存放文件信息

可以直接 
```Shell
rm -rf *
```

---

### 环境变量PATH添加方法

#### (可选之一)修改开机自启动 shell 文件

`~/.bashrc`

结尾添加

```Shell
export PATH=[added path]:$PATH
```

`PATH` 和 `$PATH` 之间为内容,`:`为不同路径间的分隔符
立马生效需要执行

```Shell
source ~/.bashrc
```

可用`echo $PATH`查看`PATH`

---

### <font color="red">`|` 的测试使用,目的：删除docker下所有Container</font>

`|` 管道符，上一条命令的输出，作为下一条命令参数

``` Shell
sudo docker ps -a | awk -F ' ' '{print $1}' | sudo xargs docker rm 
```

`sudo docker ps -a` 列出所有容器信息

`awk -F ' ' '{print $1}'` 使用 awk 处理 `docker ps -a` 的输出文本， `-F` 指定行内分隔符为 ` ` （空格）, `{print $1}` 输出分隔后的第一个参数，对应于`docker ps -a` 的第一栏输出(如下)：

```
CONTAINER
ef7d85d18dd9
190bde5a98de
8758ac640e3e
7b126ebb5a51
d188364f0a10
74e03274fc75
abe71732d055
0a6ae979ff0d
3f66ac452985
4e64212e28be
06d2eb6b7fe0
9bab8b13f44e
4104e0a65327
dfbb338cfc5c
0c7d368d84da
```

接着使用 `xargs` 获取上条指令的输出作文本指令的输入 

`docker rm` 完成删除， 提示` Error:no such container:CONTAINER `。

