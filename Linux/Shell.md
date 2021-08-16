[TOC]

#SHELL 使用笔记

### 基础

1. Shell 解释器种类：

* __Bourne Shell（/usr/bin/sh或/bin/sh）__
* __Bourne Again Shell（/bin/bash）__
* C Shell（/usr/bin/csh）
* K Shell（/usr/bin/ksh）
* Shell for Root（/sbin/sh

2. `#!`,一个约定的惯例，它告诉系统这个脚本需要什么解释器来执行，即使用哪一种 Shell。
```Shell
#!/bin/bash
echo "Hello World !"
```

3. __`./test.sh` 和 `test.sh`__

* `./test.sh` __当前目录下寻找test.sh__
* `test.sh` __系统环境变量PATH的目录下寻找test.sh__

其他可执行二进制文件同理

4. `#` 用于注释改行

### 变量



### for 循环

##### 语法

### 例子

##### __循环操作目录下的文件：__

``` Shell
for file in ./*.o
do
    echo $file
    ar x $file
done
```

##### 设置变量获取当前路径：

``` Shell 
cur_dir=$PWD
```
PWD 大写，为环境变量提供。

##### 递归函数，处理文件及子文件夹下文件中的特定后缀名删除

``` Shell
#! /bin/bash

del_count=0

function TRAVEL_ALL_FILE(){
    for file in ./*
    do
    if [ "${file##*.}x" == "ox"  -o "${file##*.}x" == "binx" -o "${file##*.}x" == "outx" -o "${file##*.}x" == "objx" ];then    #normal file
        echo ${file}
        rm -f ${file}
        ((del_count++))
    elif [ -d "${file}" ];then   #dir
        cd "${file}"
        TRAVEL_ALL_FILE
        cd ..
    fi
    done
}
 
TRAVEL_ALL_FILE
echo "There are $del_count files deleted in : ";pwd
```