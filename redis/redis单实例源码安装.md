# CentOS安装单节点redis

本安装过程采用源码进行安装，采用的系统版本为CentOS `7.2.1511`，redis版本是 `3.2.5`。

## 源码安装单实例redis

首先找到你下载的redis压缩文件，进行解压。

```shell
    tar xzvf redis-3.2.5.tar.gz
```

针对该版本的redis目录创建软链接redis，方便进行操作。

```shell
    ln -s redis-3.2.5 redis
```

进入目录，并进行编译安装，编译的日志输出到同级目录的"install_manifest.log"中，我们将redis安装在目录`/usr/local/redis`（确保该目录已创建）下。

```shell
    cd redis
    make PREFIX=/usr/local/redis install >& install_manifest.log &
```

Note：如果上面的make过程失败，一般是由于系统中未安装`gcc`导致的，可以通过`yum`命令安装

```shell
    yum install gcc
```

安装完成后，你将在目录`/usr/local/redis`中看到一个`bin`目录，里面包含了一下几个文件

```shell
    redis-benchmark  redis-check-aof  redis-check-rdb  redis-cli  redis-sentinel  redis-server
```

## 设置环境变量

为了快捷使用redis相关命令，将redis的命令所在目录添加到系统参数`PATH`中，在`~/.bash_profile`文件末尾添加如下内容：

```bash
    #export REDIS_HOME=/usr/local/redis
    #export PATH=$PATH:$REDIS_HOME/bin
    export PATH=$PATH:/usr/local/redis/bin
```

刷新环境变量，使设置生效

```shell
    source ~/.bash_profile
```

## 启动redis服务器

### 命令式启动服务器

命令启动服务器的方式适用于开发和测试环境。

默认启动服务器的端口为`6379`，命令为

```shell
    redis-server
```

修改启动端口号

```shell
    redis-server --port 6380
```

指定配置文件

```shell
    redis-server /work/components/redis/redis.conf
```


### 脚本式启动服务器

Redis源码中含有初始化的启动脚本，位于`utils/redis_init_script`，拷贝该脚本到`/etc/init.d`中（`/etc/init.d`是一个符号链接文件，链接指向`/etc/rc.d/init.d`），文件名修改为`redis_端口号`

```shell
    cp utils/redis_init_script /etc/init.d/redis_6379
```

修改`redis_6379`脚本中的主要内容如下：

```bash
    REDISPORT=6379
    EXEC=/usr/local/redis/bin/redis-server
    CLIEXEC=/usr/local/redis/bin/redis-cli

    PIDFILE=/var/run/redis_${REDISPORT}.pid
    CONF="/etc/redis/${REDISPORT}.conf"
```

创建必要的文件目录

- `/etc/redis`目录存放Redis的配置文件
- `/var/redis/端口号`目录存放Redis的持久化文件

```shell
    mkdir /etc/redis
    mkdir /var/redis
    mkdir /var/redis/6379
```

将源码中的配置文件模板复制到`/etc/redis`目录中，以端口号重新命名

```shell
    cp /work/components/redis/redis.conf /etc/redis/6379.conf
```

修改该配置文件的部分内容

- `daemonize`改为yes，使redis以守护进程的模式运行
- `pidfile`改为`/var/run/redis_端口号.pid`，设置Redis的PID文件位置
- `bind`绑定的ip地址，默认值为`127.0.0.1`，该值服务器外无法访问
- `port`监听的端口号
- `dir`改为`/var/redis/端口号`，设置持久化文件存放位置
- `requirepass`设置连接服务器的密码


启动服务

```shell
    /etc/init.d/redis_6379 start
```


### 服务式启动Redis

`chkconfig`命令需要在redis启动脚本中增加配置，加在`#!/bin/sh`下一行

```shell
    # chkconfig: 2345 80 90
```

输入注册服务命令

```shell
    chkconfig --add redis_6379
```

启动服务

```shell
    service redis_6379 start
```


