# docker gitlab安装和配置

## 环境

- 操作系统: Linux - Centos-7-x86_64-1810
- Docker版本: Docker-CE 19.03.13
- git-lab版本：13.5.4-ce.0


## dockers安装

### 1. 拉取镜像文件

```shell
    docker pull gitlab/gitlab-ce:13.5.4-ce.0
```

### 2. 创建容器需要的映射目录

```shell
    mkdir -p /opt/soft/docker/gitlab && \
    mkdir /opt/soft/docker/gitlab/config && \
    mkdir /opt/soft/docker/gitlab/logs && \
    mkdir /opt/soft/docker/gitlab/data
```

### 3. 启动gitlab容器

启动命令如下：

```shell
    docker run -d --name gitlab \
    --env GITLAB_OMNIBUS_CONFIG="nginx['listen_port'] = 10080; external_url 'http://192.168.255.130:10080'; gitlab_rails['gitlab_ssh_host'] = '192.168.255.130'; gitlab_rails['gitlab_shell_ssh_port'] = 10022; " \
    -p 10443:443 \
    -p 10080:10080 \
    -p 10022:22 \
    -v /opt/soft/docker/gitlab/config:/etc/gitlab \
    -v /opt/soft/docker/gitlab/logs:/var/log/gitlab  \
    -v /opt/soft/docker/gitlab/data:/var/opt/gitlab \
    --restart unless-stopped \
    gitlab/gitlab-ce:13.5.4-ce.0
```
- `-d`：后台运行
- `-p`：将容器内部端口向外映射
- `--name`：命名容器名称
- `-v`：将容器内数据文件夹或者日志、配置等文件夹挂载到宿主机指定目录
- `--restart`：重启策略，使用`unless-stopped`作为重启策略，可以手工停止容器， 方便维护。
- `--env`：设置git启动的参数。使用默认的配置，生成项目的URL访问地址是按容器的hostname来生成的，也就是容器的id，因此需要增加如下参数：
    - nginx['listen_port']：设置gitlab容器内web站点的端口位10080
    - external_url：配置http协议所使用的访问地址和端口
    - gitlab_rails['gitlab_ssh_host'] 和 gitlab_rails['gitlab_shell_ssh_port'] ：配置ssh协议所使用的访问地址和端口

## UI界面操作

### 设置root密码

第一次登录页面，会提示设置`root`账号的密码，设置完成使用`root`账号和密码登录。

UI界面的地址和账号信息如下：

```
    URL: http://192.168.255.130:10080
    用户名: root
    密码：第一次登录设置
```

## 参考

- [GitLab Docker images](https://docs.gitlab.com/omnibus/docker/#gitlab-docker-images)
- [Configuration options](https://docs.gitlab.com/omnibus/settings/configuration.html#configuring-the-external-url-for-gitlab)
- [Docker安装运行Gitlab(非默认端口教程)](https://blog.csdn.net/qq_43659763/article/details/104967771)
- [Docker 方式部署 Gitlab 并使用自定义的http端口](https://www.cnblogs.com/wattwang/p/docker_gitlab.html)
- [docker安装配置gitlab详细过程](https://www.cnblogs.com/zuxing/articles/9329152.html)
- [docker下gitlab安装配置使用(完整版)](https://www.jianshu.com/p/080a962c35b6)
