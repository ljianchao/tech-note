# linux命令

## 输出环境变量

```shell
    echo $JAVA_HOME
```

## 常用命令用法

查看命令详情

```shell
    man ls # 查看ls命令手册
    info ls # ls命令信息
    ls --help # ls命令用法
```

ls命令

```shell
    ls　# 以默认方式显示当前目录文件列表
    ls -a　# 显示所有文件包括隐藏文件
    ls -l　# 显示文件属性，包括大小，日期，符号连接，是否可读写及是否可执行
    ls -F  # 在列出的文件 目录 名称后加一符号 例如可执行文件加 "*", 目录则加 "/"
```

rm删除命令

```shell
    rm file　# 删除某一个文件
    rm -rf dir　# 删除当前目录下叫dir的整个目录（包括下面的文件或目录）
    rm -rf dir/* # 仅删除目录dir里的内容，目录dir保留
```



cp复制命令

```shell
    cp source target　# 将文件source复制为target
    cp /root/source .　# 将/root下的文件source复制到当前目录
    cp -r source target # 参数 r 是指连同源文件中的子目录一同拷贝
```


移动命令

```shell
    mv source target　# 将文件source更名为target
    mv /tmp/xx.tar /root
    mv aaa.tar bbb.tar  # 重命名
```


## 进程及任务管理命令

ps进程命令：

```shell
    ps　#查看运行的进程
    ps  -e  # 显示所有进程
    ps    -f   # 全格式
    ps    -l   # 长格式
    ps -ef    # 全格式显示所有进程
    ps -aux
```

top动态显示系统运行状态

```shell
    <空格> # 立即刷新显示
    h 或者? # 显示帮助画面给出一些简短的命令总结说明
    m # 切换显示内存信息
    t # 切换显示进程和 CPU状态信息
    c # 切换显示命令名称和完整命令行
    M # 根据驻留内存大小进行排序
    P # 根据CPU使用百分比大小进行排序
    q # 退出
```

kill命令

```shell
    kill # 终止后台进程
    kill -l # 显示信号列表
    kill -9 1234 # 终止1234进程
```

## 软件安装命令

rpm命令

```shell
    rpm 
    rpm -i tomcat4.rpm # 安装tomcat软件并进行相关系统配置
    rpm -U tomcat4.rpm # 升级tomcat软件并进行相关系统配置
    rpm -qa # 列出已安装的全部软件包
    rmp -qa | grep 'tomcat' # 列出包名中包含tomcat的软件包
    rpm -qf filename # 查询软件某个文件到底属于哪个软件包
    rpm -qi packagename # 查找某个已经安装的软件包的功能
    rpm -e  packagename # 删除某个软件包
    rpm -ivh program.rpm # 安装程序program并显示安装进度
    rpm -Uvh  软件包名 # 升级一个软件包
```

yum命令

```shell
    yum search name # 从仓库查询包含name的软件
    yum install name # 从仓库下载并安装name软件
    yum update # 更新系统软件

    # 使用yum命令仅下载RPM包，而不安装，有两种方法
    # 1. 安装yum-downloadonly插件，安装后，yum命令就多了两个命令参数：`--downloadonly`和`--downloaddir=/path/to/dir`
    yum install yum-downloadonly
    yum install 软件名称 --downloadonly --downloaddir=/path/to/dir

    # 2. 不使用插件，修改配置文件`/etc/yum.conf`，将keepcache设置为1
    `
    [main]
    cachedir=/var/cache/yum
    keepcache=1
    `
```



## 磁盘操作命令

du命令

```shell
    du # 查看目录大小
    du -hs # 查询某个目录的大小
    du -h | grep G # 查看以G为单位的文件
```

df命令

```shell
    df # 查看磁盘分区情况
```

## 网络情况    

netstat命令

```shell
    netstat -a # 列出所有端口 (包括监听和未监听的)
    netstat -at # 列出所有 tcp 端口
    netstat -au # 列出所有 udp 端口
    netstat -l # 只显示监听端口
    netstat -lt # 只列出所有监听 tcp 端口
    netstat -lu # 只列出所有监听 udp 端口
    netstat -p # 查看磁盘使用情况 在 netstat 输出中显示 PID 和进程名称 netstat -p
    netstat -na | grep 9200 查看9200端口
```


##  用户、用户组和权限操作

chmod命令

```shell
    chmod -R 777 elasticsearch-2.2.0-node2 # 修改目录权限 
```

chown命令

```shell
    chown # 将指定文件的拥有者改为指定的用户或组
    chown mail:mail log2012.log # 改变拥有者和群组
    chown root: log2012.log # 改变文件拥有者和群组
    chown :mail log2012.log # 改变文件群组
    chown -R -v root:mail test6 # 改变指定目录以及其子目录下的所有文件的拥有者和群组 
```



## 文件传输

scp命令

```shell
    scp -r node1/discovery-multicast/ root@192.168.87.77:/opt/node1/plugins/ 传输目录
```


## 程序运行

nohub命令

```shell
    在运行命令后加 & 号，表示将工作丢到后台执行，如 tar xzvf abc.tar.gz &
    nohub # 可以让你在脱机或注销后，还能够让工作继续运行
    nohub [指令与参数] # 表示在终端机前景中工作
    nohub [指令与参数] & # 表示在终端机背景中工作
```

systemctl命令

```shell
    systemctl enable sshd.service # 开机启动sshd服务
    systemctl disable sshd.service # 关闭开启sshd服务
```


## 内容搜索

grep命令

```shell
    grep –e “正则表达式” 文件名 # 从文件中查找与正则匹配的行
    grep –i "被查找的字符串" 文件名 # 查找不区分大小写 
    grep -c "被查找的字符串" 文件名 # 查找匹配的行数
    grep –v "被查找的字符串" 文件名 # 查找不匹配的行
```

find命令

```shell
    find / -name 标识符 # 查找包含标识符的文件或者目录
```


## 编译

```shell
    make test # 测试是否编译正确
    make # 编译程序
    make install # 将这些可执行程序复制到 /usr/local/bin目录中，以便以后执行程序时可以不用输入完整的路径
```



## 系统信息

```shell
    uname -a # Linux查看版本当前操作系统内核信息
    cat /proc/version # Linux查看当前操作系统版本信息
    cat /etc/issue 或 cat /etc/redhat-release # Linux查看版本当前操作系统发行版信息
```

## 文件描述符（file descriptor, fd）

```shell
    # 查看限制
    ulimit -n # 查看用户级限制的数量，默认值为1024
    `sysctl -a | grep fs.file-max`或者`cat /proc/sys/fs/file-max` # 查看系统级限制的数量

    # 修改用户级限制
    `ulimit -SHn 2048` # 临时修改，只对当前shell有效
    永久修改，编辑`/etc/security/limits.conf`文件，修改其中的`hard nofile`和`soft nofile`配置的数量

    # 修改系统级限制
    通过`sysctl`命令修改`etc/sysctl.conf`文件：`sysctl -w fs.file-max=20480000`，完成后执行`sysctl -p`即可。
```


