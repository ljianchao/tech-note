# sonarqube8.4安装使用

## 环境

- 操作系统: Linux - Centos-7-x86_64-1810
- Docker版本: Docker-CE 19.03.13
- SonarQube版本：8.4-community

## docker安装

### 1. 拉取镜像文件

拉取`postgres`和`sonarqube`的镜像文件

```shell
    docker pull postgres:latest
    docker pull sonarqube:8.4-community
```

如果是在无网的主机上安装，可以在联网的主机上拉取镜像后进行导出，然后在无网的主机上进行导入操作。

```shell
    # 联网主机上导出镜像的压缩包
    docker save -o postgres-docker.tar.gz postgres:latest
    docker save -o sonarqube-8.4-community-docker.tar.gz sonarqube:8.4-community

    # 无网主机上导入镜像的压缩包
    docker load -i postgres-docker.tar.gz
    docker load -i sonarqube-8.4-community-docker.tar.gz
```

### 2. 创建容器需要的映射目录

```shell
    mkdir -p /opt/soft/docker/postgresql && \
    mkdir /opt/soft/docker/postgresql/data && \
    mkdir /opt/soft/docker/postgresql/postgresql && \
    mkdir -p /opt/soft/docker/sonarqube && \
    mkdir /opt/soft/docker/sonarqube/data && \
    mkdir /opt/soft/docker/sonarqube/extensions && \
    mkdir /opt/soft/docker/sonarqube/logs && \
    mkdir /opt/soft/docker/sonarqube/lib
```

### 3. 启动postgres容器

启动命令如下：

```shell
    docker run -d --name postgres \
    -e POSTGRES_USER=sonar \
    -e POSTGRES_PASSWORD=sonar \
    -e POSTGRES_DB=sonar \
    -p 5432:5432 \
    -v /opt/soft/docker/postgresql/postgresql:/var/lib/postgresql \
    -v /opt/soft/docker/postgresql/data:/var/lib/postgresql/data \
    --privileged=true \
    postgres:latest
```

### 4. 启动sonarqube容器

由于多分支管理插件`sonarqube-community-branch-plugin`的`jar`包需要在启动时进行路径映射，所以需要先去下载该插件。

#### 1) 安装多分支管理插件

github网站下载多分支管理插件[sonarqube-community-branch-plugin](https://github.com/mc1arke/sonarqube-community-branch-plugin)，选择合适的版本[下载](https://github.com/xuhuisheng/sonar-l10n-zh/releases)，我们选择适用于sonarqube8.4的版本`sonarqube-community-branch-plugin-1.5.0.jar`。

下载完成的jar包放到sonarqube容器映射的本地目录`/opt/soft/docker/sonarqube/lib/`下。

使用方式：在扫描时，增加如下参数：

```shell
    -Dsonar.branch.name=${GIT_BRANCH}
```

#### 2) 启动sonarqube容器

启动命令如下：

```shell
    docker run -d --name sonarqube \
    --link postgres \
    -e SONARQUBE_JDBC_USERNAME=sonar \
    -e SONARQUBE_JDBC_PASSWORD=sonar \
    -e SONARQUBE_JDBC_URL=jdbc:postgresql://postgres:5432/sonar \
    -p 9000:9000 \
    -v /opt/soft/docker/sonarqube/data:/opt/sonarqube/data \
    -v /opt/soft/docker/sonarqube/extensions:/opt/sonarqube/extensions \
    -v /opt/soft/docker/sonarqube/logs:/opt/sonarqube/logs \
    -v /opt/soft/docker/sonarqube/lib/sonarqube-community-branch-plugin-1.5.0.jar:/opt/sonarqube/lib/common/sonarqube-community-branch-plugin.jar \
    -v /opt/soft/docker/sonarqube/lib/sonarqube-community-branch-plugin-1.5.0.jar:/opt/sonarqube/extensions/plugins/sonarqube-community-branch-plugin.jar \
    sonarqube:8.4-community
```

## sonarqube设置

### 插件安装

#### 1) 安装汉化插件

github网站下载汉化插件[sonar-l10n-zh](https://github.com/xuhuisheng/sonar-l10n-zh)，选择合适的版本[下载](https://github.com/xuhuisheng/sonar-l10n-zh/releases)，我们选择适用于sonarqube8.4的版本`sonar-l10n-zh-plugin-8.4.jar`。

下载完成的jar包放到sonarqube容器映射的本地目录`/opt/soft/docker/sonarqube/extensions/downloads`下，重启容器生效。

#### 2) 配置缺省的质量配置（Quality Profiles）

SonarQube8.0到8.4缺省的**质量配置（Quality Profiles）**没有配置任何语言，需要使用者自行安装，否则会提示出错。我们是java语言开发，所以安装`sonar-java`插件。

github网站下载阿里p3c规范插件[sonar-java](https://github.com/SonarSource/sonar-java)，选择合适的版本[下载](https://github.com/SonarSource/sonar-java/releases)，我们选择适用于sonarqube8.4的版本`6.8.0.23379`。

本次下载的是源码包，需要使用maven命令打包生成需要的插件的jar包，，然后放到sonarqube容器映射的本地目录`/opt/soft/docker/sonarqube/extensions/downloads`下，重启容器生效。

#### 3) 集成阿里p3c规范

> 阿里p3c插件暂时不兼容该版本，本节内容废弃

~~github网站下载阿里p3c规范插件[sonar-pmd](https://github.com/jensgerdes/sonar-pmd)，选择合适的版本[下载](https://github.com/mc1arke/sonarqube-community-branch-plugin/releases)，我们选择`sonar-pmd-plugin-3.2.1.jar`。~~

~~下载完成的jar包放到sonarqube容器映射的本地目录`/opt/soft/docker/sonarqube/extensions/downloads`下，重启容器生效。~~

## UI界面操作

UI界面的地址和账号信息如下：

```
    URL: http://localhost:9000
    用户名/密码: admin/admin
```

### 1. 阿里p3c规则激活

> 阿里p3c插件暂时不兼容该版本，本节内容废弃

~~进入管理页面后，选择`质量配置`选项卡，在`搜索配置`下拉列表中选择`java`，可以看到`ali-p3c`条目，点击设置下列列表中的`激活更多规则`选项，跳转到`代码规则`页面，点击`批量修改`下拉列表中的`激活ali-p3c`按钮，完成阿里p3c规则激活。~~

### 2. 使用

- 点击`项目`选项卡，选择`创建新项目`
- 设置`项目标识`和`显示名`
- 创建令牌，如生成的令牌为`f6e0b747d91c8845b376cb97e11d3a8a0f58ae89`

### 3. 检查提交

检查提交有两种方式，一种是使用maven进行提交，一种是使用sonar-scanner进行提交。

#### 1）maven提交

使用maven分析你的项目

```shell
  mvn sonar:sonar \
  -Dsonar.projectKey=demo \
  -Dsonar.host.url=http://127.0.0.1:9000 \
  -Dsonar.login=f6e0b747d91c8845b376cb97e11d3a8a0f58ae89 \
  -Dsonar.branch.name=master
```

- 参数`-Dsonar.projectKey`指定项目名称
- 参数`-Dsonar.host.url`指定管理端地址
- 参数`-Dsonar.login`指定管理界面的令牌
- 参数`-Dsonar.branch.name`指定分支名称

#### 2）sonar-scanner提交

```shell
    sonar-scanner -Dsonar.host.url=http://localhost:9000 \
    -Dsonar.projectKey=sonarqube_test  \
    -Dsonar.login=09904533c04eee38bc6c6d5b4acfc4fb73865626 \
    -Dsonar.java.binaries=. \
    -Dsonar.java.source=11 \
    -X
```

### 4. 检查结果查看

在管理端页面项目选项卡，选择已分析的项目查看结果。

## Idea代码检查插件 - SonarLint 安装使用

SonarLint 是 SonarQube的单机版，可以作为Idea的插件进行代码规范检查，也可以连接到SonarQube中生成统计报告。

### 插件安装

#### 通过Jetbrains官方仓库安装

- `IntelliJ IDEA` 打开 `Settings >> Plugins >> Browse repositories...`
- 在搜索框输入`SonarLint`，点击Install进行安装，然后重启IDE生效(因为插件zip包托管在Jetbrains官方CDN上，所以是从国外的服务器进行下载，可能会出现超时的情况)

#### 通过下载安装包进行安装

- 打开[插件](https://plugins.jetbrains.com/plugin/7973-sonarlint/versions)页面，选择合适的版本下载
- `IntelliJ IDEA` 打开`Settings >> Plugins >> Install plugin from disk...`，选择刚刚下载的zip包安装，然后重启IDE

### 插件使用

右键单击项目(或文件)，找到`SonarLint`并点击`Analyze with SonarLint`进行分析，查看分析结果。

## Idea代码检查插件 - ali-p3c 安装使用

### 插件安装

#### 通过Jetbrains官方仓库安装

- `IntelliJ IDEA` 打开 `Settings >> Plugins >> Browse repositories...`
- 在搜索框输入`alibaba`即可看到`Alibaba Java Code Guidelines`插件，点击Install进行安装，然后重启IDE生效(因为插件zip包托管在Jetbrains官方CDN上，所以是从国外的服务器进行下载，可能会出现超时的情况)

#### 通过下载安装包进行安装

- 打开[插件](https://plugins.jetbrains.com/plugin/10046-alibaba-java-coding-guidelines/versions)页面，选择合适的版本下载
- `IntelliJ IDEA` 打开`Settings >> Plugins >> Install plugin from disk...`，选择刚刚下载的zip包安装，然后重启IDE

### 插件使用

右键单击项目(或文件)，点击`编码规约扫描`进行分析，查看分析结果。

## 参考

- [Install the Server](https://docs.sonarqube.org/latest/setup/install-server/)
- [docker部署sonarqube](https://zhuanlan.zhihu.com/p/139175875)
- [docker 安装 sonarQube](https://www.cnblogs.com/shenh/p/13428029.html)
- [Windows环境从零搭建SonarQube 7.4(稳定版)](https://www.jianshu.com/p/dcd71a58f926)
- [Windows环境从零搭建SonarQube 8.5(最新版)](https://www.jianshu.com/p/8c4cfc482aa6)
- [docker-compose方式部署sonarqube+postgresql](https://www.nps.ink/290681.html)
- [SonarQube 8.0的初始化操作](https://blog.csdn.net/liumiaocn/article/details/103208062)
- [sonarqube配置全指南，集成阿里巴巴p3c规范](https://www.cnblogs.com/dalianmaodada/archive/2020/03/17/12468985.html)
- [sonarqube配置全指南，集成阿里巴巴p3c规范](https://my.oschina.net/u/4327212/blog/3209285)
- [SonarQube整合alibaba编码规范](https://www.jianshu.com/p/c0f42dba73b8)
- [一步步编写SonarQube Plugin](https://www.jianshu.com/p/4757cf99e661)
- [Idea代码检查插件 - SonarLint 安装使用](https://www.jianshu.com/p/94462ea2301c)
- [ali-p3c IDEA插件使用文档](https://github.com/alibaba/p3c/wiki/IDEA%E6%8F%92%E4%BB%B6%E4%BD%BF%E7%94%A8%E6%96%87%E6%A1%A3)
- [Jenkins+maven+pmd对java代码进行静态代码分析](https://blog.csdn.net/hwhua1986/article/details/48342745)
