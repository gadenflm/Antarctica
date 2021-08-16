#Harmony 1.1.0 编译环境构建(Docker)

## 安装Docker (略)

## Docker镜像(网上编译环境)：

下载：
`docker pull swr.cn-south-1.myhuaweicloud.com/openharmony-docker/openharmony-docker:0.0.3`

运行(需要进入OpenHarmony 源码根目录)：
`docker run -it -v $(pwd):/home/openharmony swr.cn-south-1.myhuaweicloud.com/openharmony-docker/openharmony-docker:0.0.3`

## 自己搭建的镜像（包含源码）
2021_7_1_tvt_harmony.tar

`docker load < [image]` 从本地tar文件中提取镜像

`docker images` 列出镜像表格

`docker run -it [Images:Tag]` 打开容器,运行某一个镜像,并进入交互终端

-i 交互
-t 终端
-d 后台运行

`docker ps -a`  查看已有的容器(Container)

`docker start/stop/restart [Container ID] ` 启动，停止，打开某一个容器

`docker rm -f [ContainerID]` 删除某容器

`docker cp [Container ID]:/[File Dir] [Dir]`从容器中拷贝文件出来



##进入容器后编译：
初次运行：

`python3 -m pip install --user build/lite`

`hb set .`

`>hispark_taurus`

后续：

`hb clean`
完全清除上次的编译结果（运行后，下次编译必须全编译）

`hb build -f`
全编译

`hb build [Commponent Name]` 单独编译组件（最常用）
例如，我测试多数使用 `//applications/wkl_debug` （应用子系统下自己添加的debug用组件）,此时编译使用`hb build wkl_debug`

编辑结果在 `$Harmony/out/` 下

liteos_a.bin 内核
OHOS_Image.bin 系统
rootfs_vfat.bin 文件系统
