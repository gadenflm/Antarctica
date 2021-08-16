[TOC]

# Linux 命令

### 权限

##### 修改拥有者 chown

```Shell
chown [-R] [UserName] [FileName or Path]
chown [-R] [UserName:GroupName] [FileName or Path]
```

change owner,修改文件所属用户及用户组

* -R 递归调用

##### 修改文件权限 chmod

```Shell
chmod [-R] xyz [FileName or Path]
```

* -R 递归调用
* __xyz__
    - xyz，为3个数字分别对应拥有用户(owner)、组(group)、其他(others) ，取值0-7，即`0B000~0B111`，三位分别对应rwx，即可读(read),可写(write),可执行(execute)三种权限
    - x Owner权限
    - y 同组用户权限
    - z 其他人权限

PS: 文件本身权限查看可以使用 `ls -l`



