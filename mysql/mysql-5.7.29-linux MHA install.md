# MySQL安装

## 卸载已安装mysql

### RPM包卸载

- 检查是否安装了mysql

```shell
    rpm -qa | grep -i mysql
```
  
- 卸载前关闭mysql服务

```shell
    service mysql status  
    service mysql stop  
    service mysql status  
    chkconfig --list | grep mysql  
```

- 收集mysql文件夹信息

```shell
    whereis mysql  
    find / -name mysql
```

- 卸载删除mysql各类组件

```shell
    rpm -ev 组件名称
```

- 检查和删除mysql对应的文件夹

```shell
    whereis mysql  
    find / -name mysql  
    rm -rf ...
```

- 删除mysql用户和用户组

```shell
    userdel mysql  
    groupdel mysql	(运行userdel mysql之后通常会连带删除用户组)
```

### 二进制包/源码安装方式的mysql卸载

- 检查MySQL服务并关闭服务

```shell
    ps -ef|grep mysql  
    /etc/init.d/mysql status  
    /etc/init.d/mysql stop  
    /etc/init.d/mysql status 
```

- 检查和删除mysql对应的文件夹

```shell
    whereis mysql  
    find / -name mysql  
    rm -rf ...  
```

- 删除一些配置文件

```shell
    rm -rf /etc/my.cnf  
    rm -rf /etc/init.d/mysql 
```

- 删除mysql用户和用户组

```shell
    userdel mysql
    groupdel mysql	(运行userdel mysql之后通常会连带删除用户组)
```

## 权限设置

### 用户组权限

- 查询mysql分组

```shell
    more /etc/group | grep mysql  
    groups mysql  
```


- 添加mysql用户组

```shell
    groupadd mysql  
```


- 删除mysql用户组

```shell
    groupdel mysql  
```


### 用户权限

- 查询用户

```shell
    more /etc/passwd | grep mysql  
    more /etc/shadow | grep mysql  
    id mysql 
```
 

- 删除用户

```shell
    userdel mysql
```


- 添加mysql用户

```shell
    useradd -r -g mysql -s /bin/false mysql
```


**Note**
> Because the user is required only for ownership purposes, not login purposes, the useradd command uses the -r and -s /bin/false options to create a user that does not have login permissions to your server host. Omit these options if your useradd does not support them.

## 安装

mysql安装在`/opt/sof/`目录下，数据统一存储在`/data/mysql/`目录下，可以在相应的目录下创建端口目录来进行区分多个mysql服务，本次安装采用默认端口`3306`，因此数据存放在`/data/mysql/3306/`目录下

### 下载地址

```shell
    https://downloads.mysql.com/archives/community/ 
```

### 解压tar包

```shell
    tar xzvf mysql-5.7.29-linux-glibc2.12-x86_64.tar.gz
```

###　创建数据目录

```shell
    mkdir -p /data/mysql/3306
```

### 将`/data/mysql/3306`目录给mysql用户赋权

```shell
    chown -R mysql /data/mysql/3306/
    chgrp -R mysql /data/mysql/3306/  
    chown -R mysql:mysql /data/mysql/3306/
```


### 更新配置文件`/etc/my.cnf`内容

- master配置

```conf
[mysqld_safe]
# 记录mysql线程id号
pid_file=/data/mysql/3306/mysqlId.pid

[mysql]
port=3306
# 默认字符集
default_character_set=utf8

[client]
# 远程客户端连接属性
port=3306
# 临时文件，mysql启动时产生，停止时消失
socket=/data/mysql/3306/mysqld_3306.sock

[mysqld]
bind_address=0.0.0.0
port=3306
# 记录mysql线程id号
pid_file=/data/mysql/3306/mysqlId.pid
character_set_server=utf8
open_files_limit=65536
# mysql安装路径
basedir=/opt/soft/mysql
# mysql的数据文件路径
datadir=/data/mysql/3306
# 临时文件路径
tmpdir=/data/mysql/3306
# 错误日志
log_error=/data/mysql/3306/alert.log
socket=/data/mysql/3306/mysqld_3306.sock
## binlog
log-bin=/data/mysql/3306/mysql-bin
relay-log=/data/mysql/3306/relay-bin
server_id=43128
```

- slave配置

```conf
[mysqld_safe]
# 记录mysql线程id号
pid_file=/data/mysql/3306/mysqlId.pid

[mysql]
port=3306
# 默认字符集
default_character_set=utf8

[client]
# 远程客户端连接属性
port=3306
# 临时文件，mysql启动时产生，停止时消失
socket=/data/mysql/3306/mysqld_3306.sock

[mysqld]
bind_address=0.0.0.0
port=3306
# 记录mysql线程id号
pid_file=/data/mysql/3306/mysqlId.pid
character_set_server=utf8
open_files_limit=65536
# mysql安装路径
basedir=/opt/soft/mysql
# mysql的数据文件路径
datadir=/data/mysql/3306
# 临时文件路径
tmpdir=/data/mysql/3306
# 错误日志
log_error=/data/mysql/3306/alert.log
socket=/data/mysql/3306/mysqld_3306.sock
## binlog
log-bin=/data/mysql/3306/mysql-bin
relay-log=/data/mysql/3306/relay-bin
server_id=43129
# 允许备库将其重复的日志也记录到自身的二进制日志中
log_slave_updates=1
# 阻止任何没有特权权限的线程修改数据
read_only=1 
```

### 创建软链

```shell
    cd /usr/local/  
    ln -s /opt/soft/mysql-5.7.29-linux-glibc2.12-x86_64
```

### 初始化数据库，配置参数

```shell
    bin/mysqld --initialize --user=mysql
```

**Note**：可能出现如下错误

```shell
    bin/mysqld: error while loading shared libraries: libaio.so.1: cannot open shared object file: No such file or directory 
```
 
修复上面的错误，需安装libaio

```shell
    yum install -y libaio
```

在文件`/data/mysql/3306/alert.log`中最后一句会提示我们密码，需要记住：

```shell
    2020-04-16T16:11:48.365789Z 1 [Note] A temporary password is generated for root@localhost: HeqF4ym76l+:
```

### 生成mysql密钥对

```shell
    bin/mysql_ssl_rsa_setup
```
 
###　安全启动

```shell
    bin/mysqld_safe --user=mysql &
```

查看进程

```shell
    ps -ef|grep mysql
```

### 临时密码登录mysql

```shell
    bin/mysql -h localhost -u root -p
```

此时可能会报错

```shell
    [root@localhost mysql]# bin/mysql -h localhost -u root -p
    bin/mysql: error while loading shared libraries: libncurses.so.5: cannot open shared object file: No such file or directory
```

解决方法:

```shell
    yum install libncurses*
```

### 修改密码，创建远程连接

```mysql
    set password=password('123456');  
    grant all privileges on *.* to 'root'@'%' identified by '123456';  -- 远程登录密码  
    flush privileges;  
    quit;
```


## 设置开机服务启动

拷贝support-files/mysql.server 拷贝为/etc/init.d/mysql
cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysql

查看mysql服务是否在服务配置中
chkconfig --list mysql

注册开机启动服务
chkconfig --add mysql

启动或停止mysql服务
service mysql start
service mysql stop

## 添加环境变量，使用mysql命令
export PATH=$PATH:/usr/local/mysql/bin






