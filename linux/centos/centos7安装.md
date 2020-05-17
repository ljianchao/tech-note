
# 安装操作

## 网络配置

CentOS 7安装后，网络默认是不能访问的，需要进行手动配置

### 查看网卡配置

使用命令找到需要配置的网卡名称，我的是`ens33`

```shell
    ip addr
```

### 修改网络配置

以root账号登录，修改文件`/etc/sysconfig/network-scripts/ifcfg-ens33 `，命令如下：

```shell
    vi /etc/sysconfig/network-scripts/ifcfg-ens33
```

文件里的原内容：

```properties

```

将`ONBOOT=no`改为`ONBOOT=yes`，即可配置开启自启动。修改完成保存文件后，输入`reboot`命令重启系统即可。

### ifconfig/netstat

ifconfig 及 netstat 工具程序在 CentOS 5 及 6 的应用手册内被置标为降级已接近十年，而 Redhat 决定在 CentOS 7 不会缺省安装 net-tools 组件。其中一个转换的原因就是 ifconfig 不会显示界面卡所拥有的全部 IP 位置 —— 请改用 ip 指令。取而代之的工具是 ss 和 ip。假如你真的、真的很需要 ifconfig 和 netstat，你可执行 `yum install net-tools`。

### 参考

- [有关 CentOS-7 的问题](https://wiki.centos.org/zh/FAQ/CentOS7)
- [初装CentOS环境配置（CentOS7网络配置）](https://blog.csdn.net/loveyou388i/article/details/80797629)

## 安装rz/sz服务

命令

```
    yum -y install lrzsz
```