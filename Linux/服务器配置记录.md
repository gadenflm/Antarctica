pULiZVQaM8rz

[TOC]


# 服务器配置_命令 (CentOS7)

### 用户

##### 添加用户 useradd 

``` Shell
useradd [-g root] [UserName] [-d //home/wkl] [-p Password] [-u uid]
```

* -g 添加用户所在组
* -d 添加用户默认目录
* -p 添加密码
* -u 分配用户id

##### 密码设置 passwd 

```Shell 
passwd [UserName]
```



### 安装

##### yum (CentOS7下主要工具)

# 服务器配置_安装 (CentOS7)

#### C 环境 安装

```Shell
 sudo yum -y install gcc gcc-c++ kernel-devel
 yum install make
 yum install compat-gcc-44    # 兼容 4.4 
 yum install compat-gcc-44-c++ # 兼容 4.4
 yum install cmake
 yum install gdb # 代码调试 GNU Debuger
 
```

#### vim 安装
```Shell
yum -y install vim
```

# 服务器配置_配置 (CentOS7)

#### sudo 用户配置

以root身份打开 `/etc/sudoers`, `root    ALL=(ALL)       ALL ` 下添加 `[UserName]    All=(ALL)   ALL`

```Shell
root    ALL=(ALL)       ALL 
wkl     ALL=(ALL)       ALL
```