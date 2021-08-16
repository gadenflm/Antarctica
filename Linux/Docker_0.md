[TOC]

#Docker 记录

***巫昆仑 2021年7月1日***

### 语法

`docker pull` 获取（下载）镜像

`docker images` 列出镜像表格

`docker run -it [Images:Tag]` 打开容器,运行某一个镜像,并进入交互终端

-i 交互
-t 终端
-d 后台运行

`docker save -o name.tar [image]` 保存一个镜像到本地tar文件

`docker load < [image]` 从本地tar文件中提取镜像

---

`docker ps -a`  查看已有的容器(Container)

`docker start/stop/restart [Container ID] ` 启动，停止，打开某一个容器

`docker attach [Container ID]` 进入一个后台运行的容器，从这个容器中退出，会导致容器停止

`docker exec [Container ID]`
进入一个后台运行的容器，从这个容器中推出，容器继续运行！！

`docker export [Container ID] > name.tar` 导出某容器，存储为本地tar文件

`docker rm -f [ContainerID]` 删除某容器

`docker cp [Container ID]:/[File Dir] [Dir]`从容器中拷贝文件出来

---

`docker build -t [name] [Path]` 编译一个本地镜像,Path 为Dockerfile路径和操作路径。

Dockerfile：
```
From [Images:Tag]  
<!-- 从某个已有镜像中为基础编译 -->

COPY [SourceFile/Dir] [TargetFile/Dir]
<!-- 从本地复制文件到镜像 -->
```

---

### 用例

##### docker container 清除

见`Linux_问题(杂).md`

``` Shell
sudo docker ps -a | awf -F ' ' '{print $1}' | sudo xargs docker rm 
```

##### __docker build 大小问题__

存放Dockerfile文件的文件夹最好是只有Dockerfile这一个文件，Docker默认会把Dockerfile文件同级目录下的东西全部发送到Docker daemon。

解决方案：

在Dockerfile同级目录下添加一个 .dockerignore 文件, 在 .dockerignore 中添加需要忽略的文件或者文件夹即可。