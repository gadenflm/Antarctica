### 网络配置
```Shell
ifconfig eth0 down
ifconfig eth0 10.10.100.113 netmask 255.255.240.0 hw ether 00:18:ae:c1:b2:a3
ifconfig eth0 up

#optional:
route add default gw 10.10.0.1 dev eth0
```

### CIFS linux mount
```Shell
mount -t cifs -o domain=ambarella,sec=ntlmssp,username=dell,password=flm,uid=0,gid=0,file_mode=0755,dir_mode=0755 //10.10.100.111/share /mnt/nfs
```

```
export LD_LIBRARY_PATH=/mnt/nfs/sdk
```


```
ifconfig eth0 down
ifconfig eth0 10.10.100.113 netmask 255.255.240.0 hw ether 00:18:ae:c1:b2:a3
ifconfig eth0 up

mount -t cifs -o domain=ambarella,sec=ntlmssp,username=dell,password=flm,uid=0,gid=0,file_mode=0755,dir_mode=0755 //10.10.100.114/share /mnt/nfs
cd /mnt/nfs/sdk

mount -t cifs -o domain=ambarella,sec=ntlmssp,username=dell,password=flm,uid=0,gid=0,file_mode=0755,dir_mode=0755 //10.10.100.114/share /mnt/nfs 
mv -f /tmp_run/yuv* /mnt/nfs
```
