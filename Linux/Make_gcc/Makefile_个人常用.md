[TOC]
#Makefile_个人常用

## 函数

函数调用语法
```Makefile
$([function] [arg1],[arg2],....)
```

### 通配符展开 wildcard

```Makefile
# 个人常用：
# 获得 DIR 全部目录的 .c 和 .cpp 文件
SRCS:=$(wildcard $(DIR)/*.cpp) \
     $((wildcard $(DIR)/*.c))
```

### 模式字符串替换 patsubst 

```Makefile
$(patsubst <pattern>,<replacement>,<text>)

# 个人常用：
# 获得 SRC对应的.c 和 .cpp 相应的 .o
OBJS:=$(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(SRCS) ))
```

查找`<text>`中的单词（单词以“空格”、“Tab”或“回车”“换行”分隔）是否符合模式`<pattern>`，如果匹配的话，则以`<replacement>`替换。

__这里，`<pattern>`可以包括通配符“%”，表示任意长度的字串。如果`<replacement>`中也包含“%”，那么，`<replacement>`中的这个“%”将是`<pattern>`中的那个“%”所代表的字串。（可以用“\”来转义，以“\%”来表示真实含义的“%”字符）__

返回：函数返回被替换过后的字符串。

## 规则

### 通用FLAGS

```Makefile

INCLUDE_PATH := include

# Compile tools:
CROSS_COMPILE := 

CC := $(CROSS_COMPILE)gcc
CXX:= $(CROSS_COMPILE)g++
STRIP :=$(CROSS_COMPILE)strip

# Flags define:

CFLAGS := -O0 -fPIC -I $(INCLUDE_PATH) -Wall -Wl,-rpath=. -std=c99
CXXFLAGS := -O0 -fPIC -I $(INCLUDE_PATH) -Wall -Wl,-rpath=. -std=c++11
SHARE := -fPIC -shared -o

```


### .o 编译设定

```Makefile

%.o : %.cpp
    ${CXX} -c ${CXXFLAGS} $< -o $@ 

%.o : %.c
    ${CC} -c ${CFLAGS} $< -o $@ 

```