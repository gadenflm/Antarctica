##[GitHub tutorial](https://git-scm.com/docs/)

####同Linux:

```
$ clear 
$ cd 
$ ls 
```


#git:

`$ git status`
查看当前的 repository,查看仓库当前的状态，显示有变更的文件

`$ git add [filename] or .`
上传指定文件 or 当前目录下所有 写入暂存区

`$ git rm  [filename]`
删除指定文件 暂存区中移除

`$ git commit <-a> <-m> [message]`
将暂存区内容添加到本地仓库中

* [message]:"标识"  (个人习惯：目前时间)

* -a 参数设置修改文件后不需要执行 git add 命令，直接来提交
   ;该参数只能更新文件，不能上传新的文件


`$ git diff `
比较文件的不同，即暂存区和工作区的差异。

`$ git pull `
下载云端代码并合并

`$ git push `
上传云端代码并合并

`$ git log <--oneline>`
查看历史提交记录。

* --oneline 选项来查看历史记录的简洁的版本。