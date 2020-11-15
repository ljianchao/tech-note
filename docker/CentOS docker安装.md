# CentOS docker安装

## 使用官方安装脚本自动安装

安装命令如下：

```shell
    curl -fsSL https://get.docker.com | bash -s docker --mirror Aliyun
```

也可以使用国内 daocloud 一键安装命令：

```shell
    curl -sSL https://get.daocloud.io/docker | sh
```

## 手动安装

### 1. 卸载旧版本

```shell
    yum remove docker \
                  docker-client \
                  docker-client-latest \
                  docker-common \
                  docker-latest \
                  docker-latest-logrotate \
                  docker-logrotate \
                  docker-engine
```

### 2. 安装 Docker Engine-Community

#### 使用 Docker 仓库进行安装

在新主机上首次安装 Docker Engine-Community 之前，需要设置 Docker 仓库。之后，您可以从仓库安装和更新 Docker。

设置仓库：安装所需的软件包。yum-utils 提供了 yum-config-manager ，并且 device mapper 存储驱动程序需要 device-mapper-persistent-data 和 lvm2。

```shell
    yum install -y yum-utils device-mapper-persistent-data lvm2 --downloadonly --downloaddir=/tmp/docker

    yum install -y yum-utils \
    device-mapper-persistent-data \
    lvm2
```


使用以下命令来设置稳定的仓库。

使用官方源地址（比较慢）：

```shell
    yum-config-manager \
        --add-repo \
        https://download.docker.com/linux/centos/docker-ce.repo
```

阿里云地址：

```shell
    yum-config-manager \
        --add-repo \
        http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
```

清华大学地址：

```shell
    yum-config-manager \
        --add-repo \
        https://mirrors.tuna.tsinghua.edu.cn/docker-ce/linux/centos/docker-ce.repo
```

#### 安装 Docker Engine-Community

安装最新版本的 Docker Engine-Community 和 containerd：

```shell
    # 仅下载
    yum install docker-ce docker-ce-cli containerd.io --downloadonly --downloaddir=/tmp/docker

    # 安装
    yum install docker-ce docker-ce-cli containerd.io 
```

## 离线安装

### 1. 下载RPM包

在[阿里云docker镜像站](https://mirrors.aliyun.com/docker-ce/linux/centos/7/x86_64/edge/Packages/)下载如下RPM包

```
    audit-2.8.5-4.el7.x86_64.rpm
    audit-libs-2.8.5-4.el7.x86_64.rpm
    audit-libs-python-2.8.5-4.el7.x86_64.rpm
    checkpolicy-2.5-8.el7.x86_64.rpm
    containerd.io-1.3.7-3.1.el7.x86_64.rpm
    container-selinux-2.119.2-1.911c772.el7_8.noarch.rpm
    docker-ce-19.03.13-3.el7.x86_64.rpm
    docker-ce-cli-19.03.13-3.el7.x86_64.rpm
    libcgroup-0.41-21.el7.x86_64.rpm
    libsemanage-python-2.5-14.el7.x86_64.rpm
    policycoreutils-2.5-34.el7.x86_64.rpm
    policycoreutils-python-2.5-34.el7.x86_64.rpm
    python-IPy-0.75-6.el7.noarch.rpm
    setools-libs-3.3.8-4.el7.x86_64.rpm
```

### 2. 安装RPM包

可以创建安装脚本来进行RPM包的安装，如果提示有需要update的包，需要创建`updates`目录，将此类包放入`updates`目录中，安装脚本文件`install.sh`内容如下：

```shell
    #! /bin/bash

    rpm -ivh *.rpm
    rpm -Uvh updates/*.rpm
```

### 3. 启动服务

Docker 安装完默认未启动。并且已经创建好 docker 用户组，但该用户组下没有用户。

```shell
    systemctl start docker
```

### 4. 查看服务

安装完成后，查看时候已启动的docker服务：

```shell
    ps -aux | grep docker

    # 查看docker信息
    docker info
```

## Docker 镜像加速

对于使用 systemd 的系统，请在 `/etc/docker/daemon.json` 中写入如下内容（如果文件不存在请新建该文件）：

```json
    {"registry-mirrors":["https://reg-mirror.qiniu.com/"]}
```

之后重新启动服务

```shell
$ sudo systemctl daemon-reload
$ sudo systemctl restart docker
```

## 参考

- [CentOS Docker 安装](https://www.runoob.com/docker/centos-docker-install.html)
- [Docker 镜像加速](https://www.runoob.com/docker/docker-mirror-acceleration.html)
- [终极离线情况下安装docker、mysql、jdk、启动项目](https://blog.csdn.net/qq_31142237/article/details/93595560)
- [阿里云CentOS-7Packages](https://mirrors.aliyun.com/centos/7/os/x86_64/Packages/)
