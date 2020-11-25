# Quick Go

## 镜像站汇总

- [阿里云官方镜像站](https://developer.aliyun.com/mirror/)
- [语雀开源](https://www.yuque.com/fvxoda)

## linux

- [linux rpm软件库pkgs](https://pkgs.org/)
- [aliyun centos7 rpm软件库](https://mirrors.aliyun.com/centos/7/os/x86_64/Packages/)

### CentOS

- [CentOS官方下载](https://www.centos.org/download/)
- [阿里云CentOS下载](https://mirrors.aliyun.com/centos/)
- [阿里云CentOS yum加速镜像配置](https://developer.aliyun.com/mirror/centos)
    - yum加速地址：`https://mirrors.aliyun.com/repo/Centos-8.repo`

## IntelliJ IDEA

- [Plugins Marketplace](https://plugins.jetbrains.com/)

## maven

- [mvnrepository](https://mvnrepository.com/)
- [AliRepo](https://maven.aliyun.com/mvn/search)
- [search.maven](https://search.maven.org/)
- [阿里云云效 Maven](https://maven.aliyun.com/mvn/guide)
- [阿里云maven加速镜像配置](https://developer.aliyun.com/mirror/maven)
    ```shell
    <mirror>
        <id>aliyunmaven</id>
        <mirrorOf>*</mirrorOf>
        <name>阿里云公共仓库</name>
        <url>https://maven.aliyun.com/repository/public</url>
    </mirror>

    <repository>
        <id>spring</id>
        <url>https://maven.aliyun.com/repository/spring</url>
        <releases>
            <enabled>true</enabled>
        </releases>
        <snapshots>
            <enabled>true</enabled>
        </snapshots>
    </repository>
    ```

## docker

- [阿里云Docker CE镜像配置](https://developer.aliyun.com/mirror/docker-ce)
    - 加速地址：`https://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo`
- [阿里云镜像加速获取地址](https://cr.console.aliyun.com/cn-hangzhou/instances/mirrors)

## github

- [github国内代理](https://github.strcpy.cn/)
- [语雀加速](https://www.yuque.com/fvxoda)

## Magnet

- [虚名寻](https://www.xumingxun.com/bt-search)

## Tools

- [TableConvert Online](https://tableconvert.com/)
