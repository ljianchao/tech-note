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

### 解压tar包

```
   tar xvf mysql-8.0.19-linux-glibc2.12-x86_64.tar.xz
```

### 创建软链

```
   cd /usr/local/  
   ln -s /opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64 mysql
```

### mysql目录赋权

```
   chown -R mysql:mysql /opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64
   chmod 750 /opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64
```

### 创建数据目录

```
   mkdir -p /data/mysql/3306
```

### 将数据目录`/data/mysql/3306`给mysql用户赋权

```
   chown -R mysql:mysql /data/mysql/3306/
```

### 更新配置文件`/etc/my.cnf`内容

- master配置

```conf
[mysqld_safe]
# 记录mysql线程id号
pid-file=/data/mysql/3306/mysqlId.pid

[mysql]
port=3306
# 默认字符集
default-character-set=utf8mb4

[client]
# 远程客户端连接属性
port=3306
# 临时文件，mysql启动时产生，停止时消失
socket=/data/mysql/3306/mysqld_3306.sock
default-character-set=utf8mb4

[mysqld]
bind_address=0.0.0.0
port=3306
# 记录mysqld线程id号
pid_file=/data/mysql/3306/mysqld.pid
init_connect='SET NAMES utf8mb4'
character_set_server=utf8mb4
collation-server=utf8mb4_bin
open_files_limit=65536
# mysql安装路径
basedir=/opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64
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
server_id=255128
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
default-character-set=utf8mb4

[client]
# 远程客户端连接属性
port=3306
# 临时文件，mysql启动时产生，停止时消失
socket=/data/mysql/3306/mysqld_3306.sock
default-character-set=utf8mb4

[mysqld]
bind_address=0.0.0.0
port=3306
# 记录mysqld线程id号
pid_file=/data/mysql/3306/mysqld.pid
init_connect='SET NAMES utf8mb4'
character_set_server=utf8mb4
collation-server=utf8mb4_bin
open_files_limit=65536
# mysql安装路径
basedir=/opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64
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
server_id=255129
binlog_format=row
## 允许备库将其重放的日志也记录到自身的二进制日志中
log_slave_updates=1
## 阻止任何没有特权权限的线程修改数据
read_only=1
```

- 无注释版本

```conf
[mysqld_safe]
pid-file=/data/mysql/3309/mysqlId.pid

[mysql]
port=3309
default-character-set=utf8mb4

[client]
port=3309
socket=/data/mysql/3309/mysqld_3309.sock
default-character-set=utf8mb4

[mysqld]
bind_address=0.0.0.0
port=3309
pid_file=/data/mysql/3309/mysqld.pid
init_connect='SET NAMES utf8mb4'
character_set_server=utf8mb4
collation-server=utf8mb4_bin
open_files_limit=65536
basedir=/opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64
datadir=/data/mysql/3309
tmpdir=/data/mysql/3309
log-error=/data/mysql/3309/alert.log
socket=/data/mysql/3309/mysqld_3309.sock
log-bin=/data/mysql/3309/mysqld-bin
relay-log=/data/mysql/3309/relay-bin
server_id=255128
binlog_format=row
sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION'
log_bin_trust_function_creators=1
max_connections=2000
```



### 初始化数据库，配置参数
> bin/mysqld --initialize --user=mysql --basedir=/opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64 --datadir=/data/mysql/3309  

Note：可能出现如下错误
> bin/mysqld: error while loading shared libraries: libaio.so.1: cannot open shared object file: No such file or directory  

修复上面的错误，需安装libaio
> yum install -y libaio

去查看日志文件`/data/mysql/3306/alert.log`，最后一句会提示我们密码，需要记住：
> [Note] [MY-010454] [Server] A temporary password is generated for root@localhost: ta+ysuJT_7fK

### 生成mysql密钥对

> bin/mysql_ssl_rsa_setup --datadir=/data/mysql/3309

### 安全启动
> bin/mysqld_safe --defaults-file=/data/mysql/3309/my.cnf --user=mysql &

查看进程
> ps -ef|grep mysql

### 临时密码登录mysql

> bin/mysql -h 127.0.0.1 -P 3309 -uroot -p

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

```mysql
   # 设置root账户本地登录的密码
   ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '123456';

   # 设置root账户远程登陆的密码和权限
   CREATE USER 'root'@'%' IDENTIFIED BY '123456';
   # 必须用以下语句修改密码，否则远程客户端连接会报错误：错误号码2058，Plugin caching_sha2_password not be load: xxxxx
   ALTER USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY '123456';
   GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;;
   flush privileges;
```

NOTE:
> CentOS 7 防火墙默认没有开放3306端口，需要如下命令添加端口：

```
   # 查看端口是否开启
   firewall-cmd --query-port=3306/tcp

   # 添加端口
   firewall-cmd --zone=public --add-port=3306/tcp --permanent

   # 重启防火墙，使新增的端口生效
   firewall-cmd --reload
```

### 修改sql的模式

执行以下语句修改sql的模式
```sql
## 查询当前模式
SELECT @@GLOBAL.sql_mode;
## 修改模式
set global sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
```

修改配置文件`my.cnf`，防止重启失效

```conf
[mysqld]
sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION'
```

### 解决开启bin-log函数同步的问题

```sql
## 查询配置
SHOW GLOBAL VARIABLES LIKE 'log_bin_trust_function_creators';

## 更改配置
SET GLOBAL log_bin_trust_function_creators=1;
```

修改配置文件`my.cnf`，防止重启失效

```conf
[mysqld]
log_bin_trust_function_creators=1
```

### 设置最大连接数

```sql
## 查询最大连接数，默认151
SHOW VARIABLES LIKE 'max_connections';

## 更改配置
SET GLOBAL max_connections=2000;
```

修改配置文件`my.cnf`，防止重启失效

```conf
[mysqld]
max_connections=2000
```

## 添加环境变量

编辑`/etc/profile`文件，添加环境变量，使用mysql命令

```
   export PATH=$PATH:/usr/local/mysql/bin
```

刷新环境变量

```
   source /etc/profile
```

## 设置开机服务启动

拷贝support-files/mysql.server 拷贝为/etc/init.d/mysql_3309
> cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysql_3309

修改`/etc/init.d/mysql_3309`里的内容，在`case "$mode" in`之前增加以下内容：

```
basedir="/opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64"
bindir="$basedir/bin"
datadir="/data/mysql/3309"
mysqld_pid_file_path="$datadir/mysqld.pid"
confdir="$datadir/my.cnf"
```

查看mysql服务是否在服务配置中
> chkconfig --list mysql_3309

注册开机启动服务
> chkconfig --add mysql_3309

启动或停止mysql服务
> service mysql_3309 start
> service mysql_3309 stop

## mysql 5.7 升级mysql 8.x

### 数据备份

```
mysqldump -uroot -p'123456' -B ghost_db > /tmp/mysql_bak/ghost_db.sql
```

### 数据恢复

mysqldump导入时必须使用`-B`参数包含创建表的语句

```
mysql -uroot -p'123456' < /tmp/mysql_bak/ghost_db.sql
```

错误信息

```
[root@localhost mysql_bak]# mysql -uroot -p'123456' youlin_app < /tmp/mysql_bak/youlin_app.sql
mysql: [Warning] Using a password on the command line interface can be insecure.
ERROR 1300 (HY000) at line 2483: Invalid utf8 character string: 'B6CCD0'
```




