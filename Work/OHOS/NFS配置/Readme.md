#Harmony 1.1.0 NFS配置

https://device.harmonyos.com/cn/docs/develop/kernel/oem_kernal_filesys_nfs-0000001050145051

##服务器端（Ubuntu 18）

安装服务
`sudo apt-get install nfs-kernel-server`

配置服务
在文件/etc/exports 结尾添加：
`/home/book/nfs_roofs *(rw,no_root_squash,async)`
文件权限一般设置为777

(重)启动服务
`sudo /etc/init.d/nfs-kernel-server start`
`sudo /etc/init.d/nfs-kernel-server restart`

##客户端 （Liteos_a）

`ifconfig` 查看当前ip地址，并配置，用法见：
https://gitee.com/openharmony/docs/blob/master/zh-cn/device-dev/kernel/ifconfig.md

`ping [host_ip]` 用ping指令检测与服务器间网络通讯

`su 0 0` 用于切换到管理员用户

mount 指令绑定：
```
OHOS # mkdir /nfs
OHOS # mount 10.10.7.177:/home/book/nfs /nfs nfs 0 0
```

