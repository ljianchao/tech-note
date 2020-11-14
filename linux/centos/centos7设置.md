# centos7设置

## 网络配置

CentOS 7安装后，网络默认是不能访问的，需要进行手动配置

### 1. 查看网卡配置

使用命令找到需要配置的网卡名称，我的是`ens33`

```shell
    ip addr
```

### 2. 修改网络配置

以root账号登录，修改文件`/etc/sysconfig/network-scripts/ifcfg-ens33 `，命令如下：

```shell
    vi /etc/sysconfig/network-scripts/ifcfg-ens33
```

1）修改为动态IP

文件里的原内容：

```properties
    TYPE=Ethernet
    PROXY_METHOD=none
    BROWSER_ONLY=no
    BOOTPROTO=dhcp
    DEFROUTE=yes
    IPV4_FAILURE_FATAL=no
    IPV6INIT=yes
    IPV6_AUTOCONF=yes
    IPV6_DEFROUTE=yes
    IPV6_FAILURE_FATAL=no
    IPV6_ADDR_GEN_MODE=stable-privacy
    NAME=ens33
    UUID=60f3b61b-a0e9-4fca-9feb-a88b82625ad3
    DEVICE=ens33
    ONBOOT=no
```

将`ONBOOT=no`改为`ONBOOT=yes`，即可配置开机自启动。修改完成保存文件后，输入`systemctl restart network`命令重启网络服务即可。

2）修改为静态IP

修改为静态ip的配置

```properties
    TYPE=Ethernet
    PROXY_METHOD=none
    BROWSER_ONLY=no
    BOOTPROTO=static
    DEFROUTE=yes
    IPV4_FAILURE_FATAL=no
    IPV6INIT=yes
    IPV6_AUTOCONF=yes
    IPV6_DEFROUTE=yes
    IPV6_FAILURE_FATAL=no
    IPV6_ADDR_GEN_MODE=stable-privacy
    NAME=ens33
    UUID=60f3b61b-a0e9-4fca-9feb-a88b82625ad3
    DEVICE=ens33
    ONBOOT=yes
    IPADDR=192.168.1.22
    NETMASK=255.255.255.0
    GATEWAY=192.168.1.1
    DNS1=223.5.5.5
    DNS2=101.226.4.6
```

## 设置yum源

### 1. 备份

```shell
    mv /etc/yum.repos.d/CentOS-Base.repo /etc/yum.repos.d/CentOS-Base.repo.backup
```

### 2. 下载新的 CentOS-Base.repo 到 /etc/yum.repos.d/

CentOS7

```shell
    wget -O /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-7.repo
```

或者

```shell
    curl -o /etc/yum.repos.d/CentOS-Base.repo https://mirrors.aliyun.com/repo/Centos-7.repo
```

### 3. 生成缓存

```shell
    yum clean all  # 清除系统所有的yum缓存
    yum makecache  # 生成yum缓存
```

## 设置yum不删除RPM包

使用yum命令不删除RPM包有两种方法：插件和修改配置文件。

### 插件方式

安装yum-downloadonly插件仅下载RPM包，而不安装，安装后，yum命令就多了两个命令参数：`--downloadonly`和`--downloaddir=/path/to/dir`
> 目前yum已集成该插件，不要单独进行安装操作

```shell
    yum -y install yum-downloadonly
```

安装后，假如使用yum命令安装`lrzsz`软件，命令如下：

```shell
    yum install lrzsz --downloadonly --downloaddir=/path/to/dir
```

如果系统中已经安装了`lrzsz`软件，那么就不会下载成功了，可以先使用`yum remove`卸载。

### 修改配置文件

修改配置文件`/etc/yum.conf`，将`keepcache`设置为`1`

```properties
    [main]
    keepcache=1
```

## 设置Epel 镜像

EPEL (Extra Packages for Enterprise Linux), 是由 Fedora Special Interest Group 维护的 Enterprise Linux（RHEL、CentOS）中经常用到的包。

### 1. 备份(如有配置其他epel源)

```shell
    mv /etc/yum.repos.d/epel.repo /etc/yum.repos.d/epel.repo.backup
    mv /etc/yum.repos.d/epel-testing.repo /etc/yum.repos.d/epel-testing.repo.backup
```

### 2. 下载新repo 到/etc/yum.repos.d/

#### epel(RHEL 7)

1）查看可用的epel源

```shell
    yum list | grep epel-release
```

2）安装 epel 配置包

```shell
    yum install -y epel-release
```

3）将 repo 配置中的地址替换为阿里云镜像站地址

```shell
    wget -O /etc/yum.repos.d/epel.repo http://mirrors.aliyun.com/repo/epel-7.repo
```
4）清除缓存

```shell
    yum clean all  # 清除系统所有的yum缓存
    yum makecache  # 生成yum缓存
```


## 必备软件安装

### wget

```shell
    yum -y install wget
```

### ifconfig/netstat

ifconfig 及 netstat 工具程序在 CentOS 5 及 6 的应用手册内被置标为降级已接近十年，而 Redhat 决定在 CentOS 7 不会缺省安装 net-tools 组件。其中一个转换的原因就是 ifconfig 不会显示界面卡所拥有的全部 IP 位置 —— 请改用 ip 指令。取而代之的工具是 ss 和 ip。假如你真的、真的很需要 ifconfig 和 netstat，你可执行以下命令安装：

```shell
    yum install net-tools
```

### 安装上传/下载服务

上传/下载服务`rz/sz`需执行以下命令安装：

```shell
    yum -y install lrzsz
```

### 安装jdk

添加全局环境变量，编辑`/etc/profile`文件

```
export JAVA_HOME=/opt/soft/jdk1.8.0_131
export CLASSPATH=.:${JAVA_HOME}/jre/lib/rt.jar:${JAVA_HOME}/lib/dt.jar:${JAVA_HOME}/lib/tools.jar
export PATH=$PATH:${JAVA_HOME}/bin
```

刷新配置文件

```
source /etc/profile
```

## 参考

- [有关 CentOS-7 的问题](https://wiki.centos.org/zh/FAQ/CentOS7)
- [初装CentOS环境配置（CentOS7网络配置）](https://blog.csdn.net/loveyou388i/article/details/80797629)
- [centos7配置国内yum源](https://xiaojin21cen.blog.csdn.net/article/details/84726193)
- [Linux系统yum命令安装软件时保留(下载)rpm包](https://zhangge.net/4222.html)
- [阿里云CentOS-7Packages](https://mirrors.aliyun.com/centos/7/os/x86_64/Packages/)
- [阿里云官方镜像站](https://developer.aliyun.com/mirror/)
- [阿里云CentOS镜像](https://developer.aliyun.com/mirror/centos?spm=a2c6h.13651102.0.0.3e221b11QdYsDh)
- [阿里云Epel 镜像](https://developer.aliyun.com/mirror/epel?spm=a2c6h.13651102.0.0.1cb31b11kba1ZE)
