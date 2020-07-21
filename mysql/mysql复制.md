
# 复制

## 复制概述

MySQL支持两种复制方式

- 基于语句的复制（也称逻辑复制），早在MySQL3.23版本中就已存在
- 基于行的复制，MySQL 5.1版本开始支持

这两种方式都是通过在主库上记录二进制日志、在备库重放日志的方式来实现异步的数据复制。这意味着，在同一时间点备库上的数据可能与主库存在不一致，并且无法保证主备之间的延迟。

### 复制开销

复制通常不会增加主库的开销，主要是启用二进制日志带来的开销（开启一个二进制转储线程），但出于备份或及时从崩溃中恢复的目的，这点开销是必要的。除此之外，每个备库也会对主库增加一些负载（例如网络I/O开销）。

## 配置复制

配置复制，分以下几步：

- 在每台服务器上创建复制账号
- 配置主库和备库
- 通知备库连接到主库并从主库复制数据

### 创建复制账号

```sql
mysql> CREATE USER 'repl'@'%.example.com' IDENTIFIED BY 'password';
mysql> GRANT REPLICATION SLAVE ON *.* TO 'repl'@'%.example.com';
```

### 配置主库和备库

### 启动复制

### 复制的管理和维护


### QA

#### 转移ip地址

查看章节[11.3 负载均衡]

通过ARP（地址解析协议）命令强制使IP地址的更改快速而且原子性地通知到网络上。

相关技术：Pacemaker, LVS和Wackamole

#### 故障转移和故障恢复

在故障转移（failover）的过程中，高可用是建立在冗余的基础上。

一个完全的故障转移方案至少能够监控并自动替换组件。它对应用应该是完全透明的。

- 提升备库或切换角色

    提升一台备库为主库，或者在一个主-主复制结构中调换主动和被动角色

- 虚拟IP地址或IP接管

    需要提供特定服务的MySQL实例指定一个逻辑IP地址。当MySQL实例失效时，可以将IP地址转移到另一台MySQL服务器上。

# 集群方案

## 高可用方案

目前比较常用的高可用方案有三种

- MHA：Master High Availability
- MGR: MySQL Group Replication
- PXC: Percona XtraDB Cluster

### MHA高可用方案

MHA（Master High Availability）目前在MySQL高可用方面是一个相对成熟的解决方案。在MySQL故障切换过程中，MHA能做到0~30秒之内自动完成数据库的故障切换操作，并且在进行故障切换过程中，MHA能在最大程度上保证数据的一致性，以达到真正意义上的高可用。

MHA架构由两部分组成：MHA Manager（管理节点）和MHA Node（数据节点）。MHA Manager可以单独部署在一台独立的机器上管理多个master-slave集群，也可以部署在一台slave上。

使用MySQL 5.5的半同步复制，可以大大降低数据丢失的风险。MHA可以与半同步复制结合起来。如果一个slave已经收到了最新的二进制日志，MHA可以将最新的二进制日志应用于其他所有的slave服务器上，因此它们彼此保持一致性。

目前MHA主要支持一主多从的架构，必须最少有三台数据库服务器，一主二从，即一台充当master，一台充当备用master，另一台充当从库，出于机器成本考虑，淘宝也在此基础上进行了改造，目前淘宝TMHA已经支持一主一从。

**TODO**，备选master和slave如何提供相关的读服务？

#### 半同步复制

半同步复制保证了事务成功提交后，至少有两份日志记录，一份在主库的Binlog日志上，另一份在至少一个从库的中继日志Relay Log上，从而更进一步保证了数据的完整性。

半同步复制很大程度上取决于主从库之间的网络情况，往返时延RTT越小决定了从库的实时性越好。

##### 安装插件

半同步复制需要安装插件，查看已安装插件`select * from mysql.plugin;`，

主库上安装插件`install plugin rpl_semi_sync_master SONAME 'semisync_master.so';`

从库上安装插件`install plugin rpl_semi_sync_slave SONAME 'semisync_slave.so';`

重启MySQL插件才会生效。

##### 打开半同步

主库上配置全局参数：

```sql
    set global rpl_semi_sync_master_enable=1;

    -- 如果Binlog在**rpl_semi_sync_master_timeout**时间内都无法成功推送到从库上，则MySQL自动调整复制为异步模式
    set global rpl_semi_sync_master_timeout=30000;  -- 30s
```

从库上配置参数

```sql
    set global rpl_semi_sync_slave_enable=1;

    -- 重启一下从库上的I/O线程
    STOP SLAVE IO_THREAD; START SLAVE IO_THREAD;
```

主库上查看当前半同步复制的状态

```sql
    -- 关注3个状态值
    -- Rpl_semi_sync_master_status：值为ON，表示半同步复制目前处于打开状态
    -- Rpl_semi_sync_master_yes_tx：值为0，表示主库当前尚未有任何一个事务时通过半同步复制到从库
    -- Rpl_semi_sync_master_no_tx：值为3，表示当前有3个事务不是半同步模式下从库及时响应的
    show status like '%semi_sync%';
```

检查从库的I/O线程的状态

```sql
    show slave status\G;
```





#### 安装部署MHA

##### 安装MHA node（在所有的MySQL服务上安装）

##### 安装MHA Manager

##### 配置SSH登录无密码验证

##### 搭建主从配置

##### 配置MHA

##### 检查SSH的配置

##### 检查整个复制环境状况

##### 检查MHA Manager的状态

##### 开启MHA Manager监控

##### 查看启动日志

##### 关闭MHA Manager监控

##### VIP配置



