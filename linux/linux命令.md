
## 文件描述符（file descriptor, fd）

### 查看限制数量

查看用户级限制的数量，默认值为1024，命令：`ulimit -n`。

查看系统级限制的数量，命令：`sysctl -a | grep fs.file-max`或者`cat /proc/sys/fs/file-max`

###　修改限制数量

修改用户级限制

- 临时修改，只对当前shell有效：`ulimit -SHn 2048`
- 永久修改，编辑`/etc/security/limits.conf`文件，修改其中的`hard nofile`和`soft nofile`配置的数量

修改系统级限制

- 通过`sysctl`命令修改`etc/sysctl.conf`文件：`sysctl -w fs.file-max=20480000`，完成后执行`sysctl -p`即可。