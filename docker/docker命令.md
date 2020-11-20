# docker命令

## 基础命令

```shell
## 启动docker服务
systemctl start docker

## 查看容器
docker ps

## 查看所有容器
docker ps -a

## 获取镜像
docker pull ubuntu

## 启动某个容器
docker start <容器 ID>

## 后台运行容器
## /bin/bash：放在镜像名后的是命令，这里我们希望有个交互式 Shell，因此用的是 /bin/bash
## -d：后台运行，-i: 交互式操作，-t: 终端
## -p：设置端口映射，-P：将容器内部使用的网络端口随机映射到我们使用的主机上
docker run -itd --name ubuntu-test ubuntu /bin/bash

docker run -d -P training/webapp python app.py
docker run -d -p 5000:5000 training/webapp python app.py

## 查看容器的配置
docker inspect <容器 ID>

## 查看容器网络端口的快捷方式
docker port <容器 ID>
docker port <容器 NAME>

## 停止容器
docker stop <容器 ID>

## 停止的容器可以通过 docker restart 重启
docker restart <容器 ID>

## 进入容器，在使用 -d 参数时，容器启动后会进入后台。此时想要进入容器，可以通过以下指令进入
docker attach <容器 ID>
docker exec -it <容器 ID> /bin/bash # 推荐大家使用 docker exec 命令，因为此退出容器终端，不会导致容器的停止。

## 退出容器
输入exit或者快捷键`Ctrl+D`

## 导出容器
docker export 1e560fca3906 > ubuntu.tar

## 导入容器
cat docker/ubuntu.tar | docker import - test/ubuntu:v1
docker import http://example.com/exampleimage.tgz example/imagerepo

## 删除容器
docker rm -f <容器 ID>

## 清理掉所有处于终止状态的容器
docker container prune

## 查看容器内部的标准输出，-f: 让 docker logs 像使用 tail -f 一样来输出容器内部的标准输出
docker logs -f [ID或者名字]

## 查看容器内部运行的进程
docker top [ID或者名字]
```

## 镜像相关

```shell
## 列出本地主机上的镜像
docker images

## 启动镜像
docker run -t -i ubuntu:14.04 /bin/bash

## 获取一个新的镜像
docker pull ubuntu:13.10

## 查找镜像，可以从 Docker Hub 网站来搜索镜像，Docker Hub 网址为： https://hub.docker.com/
docker search httpd

## 删除镜像
docker rmi hello-world
```

## 修改启动参数

- 停止docker服务
- 找到容器id
- 进入宿主机容器的配置文件目录：`/var/lib/docker/containers/{容器id}`
- 修改配置文件`config.v2.json`
- 启动docker 服务

## 参考

- [Docker 命令大全](https://www.runoob.com/docker/docker-command-manual.html)
- [Docker 资源汇总](https://www.runoob.com/docker/docker-resources.html)
- [修改通过docker run启动容器参数](https://www.cnblogs.com/blackmood/p/12182905.html)
