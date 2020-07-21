
## 复制功能

### 主库增加复制配置

To configure the binary log and server ID options, shut down the MySQL server and edit the `my.cnf` or
my.ini file. Within the [mysqld] section of the configuration file, add the log-bin and server-id
options.

在`my.cnf`中配置二进制日志和server ID选项

``` 
[mysqld]
## 通常使用服务器IP地址的末8位
server_id=1
## 二进制日志文件名前缀
log_bin=mysql-bin
```

### 备库增加复制配置

在`my.cnf`中配置以下内容

``` 
[mysqld]
## 通常使用服务器IP地址的末8位
server_id=2
## 二进制日志文件名前缀
log_bin=/data/mysql/3306/mysql-bin
## 指定中继日志的位置和命名
relay_log=/data/mysql/3306/mysql-relay-bin
## 允许备库将其重放的日志也记录到自身的二进制日志中
log_slave_updates=1
## 阻止任何没有特权权限的线程修改数据
read_only=1
```
