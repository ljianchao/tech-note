# mysql安装

## 二进制包/源码安装方式的mysql卸载

检查MySQL服务并关闭服务
> ps -ef|grep mysql  
> /etc/init.d/mysql status  
> /etc/init.d/mysql stop  
> /etc/init.d/mysql status  

检查和删除mysql对应的文件夹
> whereis mysql  
> find / -name mysql  
> rm -rf ...  

删除一些配置文件
> rm -rf /etc/my.cnf  
> rm -rf /etc/init.d/mysql  

删除mysql用户和用户组
> userdel mysql
> groupdel mysql	(运行userdel mysql之后通常会连带删除用户组)

## 权限设置

### 用户组权限

查询mysql分组
> more /etc/group | grep mysql  
> groups mysql  

添加mysql用户组
> groupadd mysql  

删除mysql用户组
> groupdel mysql  

### 用户权限

查询用户
> more /etc/passwd | grep mysql  
> more /etc/shadow | grep mysql  
> id mysql  

删除用户
> userdel mysql

添加mysql用户
> useradd -r -g mysql -s /bin/false mysql

Note
> Because the user is required only for ownership purposes, not login purposes, the useradd command uses the -r and -s /bin/false options to create a user that does not have login permissions to your server host. Omit these options if your useradd does not support them.

## 安装mysql

mysql安装在`/opt/soft/`目录下。数据统一存储在`/data/mysql/`目录下，可以在相应的目录下创建端口号来进行区分多个mysql服务，本次安装采用默认端口号`3306`，因此数据存储在`/data/mysql/3306/`目录下

### 创建数据目录

> mkdir /data/mysql/3306/

### 将数据目录`/data/mysql/3306`给mysql用户赋权

> chown -R mysql /data/mysql/3306/  
> chgrp -R mysql /data/mysql/3306/  
> chown -R mysql:mysql /data/mysql/3306/

### 更新配置文件`/etc/my.cnf`内容

- master配置

```conf
[mysqld_safe]
# 记录mysql线程id号
pid-file=/data/mysql/3306/mysqlId.pid

[mysql]
port=3306
# 默认字符集
default-character-set=utf8

[client]
# 远程客户端连接属性
port=3306
# 临时文件，mysql启动时产生，停止时消失
socket=/data/mysql/3306/mysqld_3306.sock

[mysqld]
bind_address=0.0.0.0
port=3306
# 记录mysqld线程id号
pid_file=/data/mysql/3306/mysqld.pid
character_set_server=utf8
open_files_limit=65536
# mysql安装路径
basedir=/usr/local/mysql
# mysql的数据文件路径
datadir=/data/mysql/3306
# 临时文件路径
tmpdir=/data/mysql/3306
# 错误日志
log-error=/data/mysql/3306/alert.log
socket=/data/mysql/3306/mysqld_3306.sock
# binlog
log-bin=/data/mysql/3306/mysqld-bin
relay-log=/data/mysql/3306/relay-bin
server_id=43131
binlog_format=row
```
- slave配置

```conf
[mysqld_safe]
# 记录mysql线程id号
pid-file=/data/mysql/3306/mysqlId.pid

[mysql]
port=3306
# 默认字符集
default-character-set=utf8

[client]
# 远程客户端连接属性
port=3306
# 临时文件，mysql启动时产生，停止时消失
socket=/data/mysql/3306/mysqld_3306.sock

[mysqld]
bind_address=0.0.0.0
port=3306
# 记录mysqld线程id号
pid_file=/data/mysql/3306/mysqld.pid
character_set_server=utf8
open_files_limit=65536
# mysql安装路径
basedir=/usr/local/mysql
# mysql的数据文件路径
datadir=/data/mysql/3306
# 临时文件路径
tmpdir=/data/mysql/3306
# 错误日志
log-error=/data/mysql/3306/alert.log
socket=/data/mysql/3306/mysqld_3306.sock
# binlog
log-bin=/data/mysql/3306/mysqld-bin
relay-log=/data/mysql/3306/relay-bin
server_id=43132
binlog_format=row
## 允许备库将其重放的日志也记录到自身的二进制日志中
log_slave_updates=1
## 阻止任何没有特权权限的线程修改数据
read_only=1
```


### 创建软链

> cd /usr/local/  
> ln -s /opt/soft/mysql-5.7.29-linux-glibc2.12-x86_64 mysql

### 初始化数据库，配置参数
> bin/mysqld --initialize --user=mysql 

Note：可能出现如下错误
> bin/mysqld: error while loading shared libraries: libaio.so.1: cannot open shared object file: No such file or directory  

修复上面的错误，需安装libaio
> yum install -y libaio

去查看日志文件`/data/mysql/3306/alert.log`，最后一句会提示我们密码，需要记住：
> 2020-04-16T17:42:37.888951Z 1 [Note] A temporary password is generated for root@localhost: W3g1v9nuky;a

### 生成mysql密钥对

> bin/mysql_ssl_rsa_setup

### 安全启动
> bin/mysqld_safe --user=mysql &

查看进程
> ps -ef|grep mysql

### 临时密码登录mysql

> bin/mysql -h localhost -u root -p

登录过程中如报错：
> bin/mysql: error while loading shared libraries: libncurses.so.5: cannot open shared object file: No such file or directory

可以从网站：`pkgs.org`中搜索`libncurses.so.5`的rpm包

还可能遇到以下错误：

```shell
[root@localhost work]# rpm -ivh ncurses-6.1-12.20190803.fc31.src.rpm 
Updating / installing...
   1:ncurses-6.1-12.20190803.fc31     warning: user mockbuild does not exist - using root
warning: group mock does not exist - using root
warning: user mockbuild does not exist - using root
warning: group mock does not exist - using root
```

此时需要创建mock组和mockbuild用户

```shell
[root@localhost work]# groupadd mock
[root@localhost work]# useradd mockbuild -g mock
```

### 修改密码，创建远程连接
> set password=password('123456');  
> grant all privileges on *.* to 'root'@'%' identified by '123456';  -- 远程登录密码  
> flush privileges;  
> quit;

## 编辑`/etc/profile`文件，添加环境变量，使用mysql命令
> export PATH=$PATH:/usr/local/mysql/bin

## 设置开机服务启动

拷贝support-files/mysql.server 拷贝为/etc/init.d/mysql
> cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysql

查看mysql服务是否在服务配置中
> chkconfig --list mysql

注册开机启动服务
> chkconfig --add mysql

启动或停止mysql服务
> service mysql start
> service mysql stop

## 搭建一主多从异步复制集群

### 主库配置

#### 主库上设置复制使用的账号

在主库上，设置一个复制使用的账号，并授予REPLICATION SLAVE权限

```sql
CREATE USER 'repl'@'192.168.43.%' IDENTIFIED BY '123456';
GRANT REPLICATION SLAVE ON *.* TO 'repl'@'192.168.43.%';
flush privileges;
```

#### 设置读锁定有效

在主库上，设置读锁定有效，确保没有数据库操作，以便获得一个一致性的快照：

```sql
flush tables with read lock;
```

#### 获取主库上的二进制日志和偏移量

获取主库上的二进制日志和偏移量，目的是为了从数据库启动以后，从这个点开始进行数据的恢复。

```
mysql> show master status;
+-------------------+----------+--------------+------------------+-------------------+
| File              | Position | Binlog_Do_DB | Binlog_Ignore_DB | Executed_Gtid_Set |
+-------------------+----------+--------------+------------------+-------------------+
| mysqld-bin.000003 | 48913314 |              |                  |                   |
+-------------------+----------+--------------+------------------+-------------------+
```

#### 生成主数据库的备份

使用`mysqldump`工具生成主数据库的备份，`--master-data=2`必须，让备份出来的文件中记录备份这一时刻的binlog文件与position号，为搭建主从环境准备

```shell
 mysqldump --single-transaction -uroot -p123456 --master-data=2 -A > /root/all.sql
```

#### 恢复数据库的写操作

```sql
unlock tables;
```

#### 数据库备份文件传送到从服务器

```shell
tar czvf all.tar.gz all.sql
scp all.tar.gz root@192.168.43.132:/opt/soft/
scp all.tar.gz root@192.168.43.132:/opt/soft/
```

### 从库配置

#### 恢复从主库传递过来的数据

```shell
tar xzvf all.tar.gz 
mysql -uroot -p123456 < all.sql 
```

#### 配置主从服务

```sql
-- 设置主从
change master to
master_host='192.168.43.131',
master_port=3306,
master_user='repl',
master_password='123456',
master_log_file='mysqld-bin.000003',
master_log_pos=48913314;

-- 启动主从复制
start slave;

-- 查看主从复制状态，`Slave_IO_Running: Yes`和`Slave_SQL_Running: Yes`即主从复制正常
show slave status\G
show processlist \G 

-- 查看主库binlog文件和位置及开启gtid模式下记录的gtid
show master status \G

- 关闭主从同步
stop slave;

- 清空从库的所有配置信息
reset slave all;
```

### 验证主从同步

创建新表或者向新表中插入数据

## MHA高可用架构设置

master 192.168.43.131 主节点
slave  192.168.43.132 候选主节点，MHA Manager节点
slave  192.168.43.133 从节点
VIP 192.168.10.100/32

### 开启mysql半同步设置

半同步复制在异步复制的基础上进行构建。

#### master服务器设置

##### 安装半同步复制插件

```sql
install plugin rpl_semi_sync_master soname 'semisync_master.so';
```

##### 配置全局参数，开启半同步复制

```sql
    set global rpl_semi_sync_master_enabled=1;
    show variables like '%semi%';
    -- 如果Binlog在**rpl_semi_sync_master_timeout**时间内都无法成功推送到从库上，则MySQL自动调整复制为异步模式，默认10s。该值可以设置很大，禁止向异步复制切换保证数据复制的安全性
    set global rpl_semi_sync_master_timeout=3600000;  -- 1小时
```

为了开机自启半同步复制功能，配置文件`my/cnf`增加以下内容：

```conf
[mysqld]
# 半同步复制
rpl_semi_sync_master_enabled=1
rpl_semi_sync_master_timeout=3600000
```

#### slave服务器设置

##### 安装半同步复制插件

```sql
install plugin rpl_semi_sync_slave soname 'semisync_slave.so';
```

##### 配置全局参数，开启半同步复制

```sql
   set global rpl_semi_sync_slave_enabled=1;
   show variables like '%semi%';
   -- 重启一下从库上的I/O线程
   STOP SLAVE IO_THREAD; 
   START SLAVE IO_THREAD;
```

为了开机自启半同步复制功能，配置文件`my/cnf`增加以下内容：

```conf
[mysqld]
# 半同步复制
rpl_semi_sync_slave_enabled=1
```

#### 查看半同步复制信息

在主库上查看当前半同步复制的一些状态值:

```sql
-- Rpl_semi_sync_master_status ON ，表示从库开启了半同步复制
show global status like "%semi%";
```

### 配置SSH登录免密验证

主节点`192.168.43.131`配置其他所有从节点的免密登录

```shell
ssh-keygen -t rsa
ssh-copy-id -i /root/.ssh/id_rsa.pub root@192.168.43.132
ssh-copy-id -i /root/.ssh/id_rsa.pub root@192.168.43.133
```

从节点+MHA Manager节点`192.168.43.132`配置其他所有节点的免密登录

```shell
ssh-keygen -t rsa
ssh-copy-id -i /root/.ssh/id_rsa.pub root@192.168.43.131
ssh-copy-id -i /root/.ssh/id_rsa.pub root@192.168.43.133
```

从节点`192.168.43.133`配置其他所有节点的免密登录

```shell
ssh-keygen -t rsa
ssh-copy-id -i /root/.ssh/id_rsa.pub root@192.168.43.131
ssh-copy-id -i /root/.ssh/id_rsa.pub root@192.168.43.132
```

### 安装MHA

#### 安装MHA Node

在三台主机上都需要安装MHA-node软件包

##### 安装MySQL依赖的perl环境

```shell
# 安装perl-DBD-MySQL
yum install -y perl-DBD-MySQL
```

##### 在所有节点上安装MHA Node

```shell
rpm -ivh mha4mysql-node-0.58-0.el7.centos.noarch.rpm
```

#### 安装MHA Manager

在master节点上安装MHA Manager

```shell

```



