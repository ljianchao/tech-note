# 相关命令

- 显示连接信息：`SHOW PROCESSLIST;`
- 查询连接超时：`show global variables like 'wait_timeout';`
- 设置连接超时：`set global wait_timeout=3600;`
- 查询缓存配置：`show global variables like 'query_cache_type';`

- 查询数据库事务隔离级别：`show variables like 'transaction_isolation';`
- 查找持续时间超过60s的事务：`select * from information_schema.innodb_trx where TIME_TO_SEC(timediff(now(),trx_started))>60;`
- 查询每次事务的`redo log`是否直接持久化到磁盘的信息，1表示直接持久化到磁盘：`show variables like 'innodb_flush_log_at_trx_commit';`
- 查询每次事务的`binlog`是否直接持久化到磁盘的信息，1表示直接持久化到磁盘：`show variables like 'sync_binlog';`

- 查询死锁超时时间：`show variables like 'innodb_lock_wait_timeout';`
- 查询死锁检测设置：`show variables like 'innodb_deadlock_detect';`

- 查询`change buffer`占`buffer pool`的比例：`show variables like 'innodb_change_buffer_max_size';`

- 查看慢查询的参数设置：`show variables like 'long_query_time';`
- 设置慢查询的参数：`set long_query_time=0;`
- 查看慢查询日志：`select * from mysql.slow_log;`

- 显示表T的索引信息（包含基数-cardinality，索引类型-Index_type）：`show index from T;`

- select加读锁（S锁、共享锁）
```
    select k from t where id=1 lock in share mode;
```
- select加写锁（X锁，排他锁）
```    
    select k from t where id=1 for update;
```


# 删除数据库中的表
SELECT CONCAT('DROP TABLE IF EXISTS ', table_name, ';')
FROM information_schema.`TABLES`
WHERE table_schema = 'youlin-health-school';

# 查询版本号

```mysql
   SELECT VERSION();
```           

# 账号操作

## 创建账号

```sql
   # 设置root账户远程登陆的密码和权限
   CREATE USER 'root'@'%' IDENTIFIED BY '123456';
   # 必须用以下语句修改密码，否则远程客户端连接会报错误：错误号码2058，Plugin caching_sha2_password not be load: xxxxx
   ALTER USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY '123456';
   GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;
   flush privileges;
```

## 授权

```sql
   -- 查询某用户的权限
   SHOW GRANTS FOR yltapi;

   -- 授予某用户的表权限
   GRANT ALL PRIVILEGES ON `youlin-scheduler-job`.* TO 'yltapi'@'%' WITH GRANT OPTION;

   -- 授权CURD权限
   GRANT SELECT, INSERT, UPDATE, DELETE ON `youlin_app`.* TO 'yltapi'@'%' WITH GRANT OPTION;
```

# 管理操作

## 查看进程

```sql

SHOW PROCESSLIST;

SELECT pr.`ID`, pr.`USER`,pr.`HOST`,pr.`DB`,pr.`COMMAND`,pr.`TIME`,pr.`STATE`,pr.`TIME_MS`,pr.`INFO`,pr.`MEMORY_USED` FROM information_schema.`PROCESSLIST` pr
WHERE `Host` LIKE '192.168.1.146%'
ORDER BY ID;

```

## 查询表结构

```sql
-- 查询表结构
SELECT col.`COLUMN_NAME`,col.`COLUMN_TYPE`,col.`COLUMN_COMMENT`,col.`COLUMN_KEY`,col.`EXTRA`,col.`IS_NULLABLE` 
FROM information_schema.`COLUMNS` col
WHERE 
col.`TABLE_SCHEMA`='youlin-health-school'
AND col.`TABLE_NAME`='yl_user_property';

-- 查询表索引
SHOW INDEX FROM yl_platform_users;

```

## 增加索引

```sql
-- 创建单列索引
ALTER TABLE `yl_user_property` ADD INDEX idx_user_id(`user_id`);

-- 创建组合索引
ALTER TABLE `yl_users_school_check` ADD INDEX idx_schnum_uname_appid(`school_number`, `user_name`, `app_id`);

```

## 慢sql监控

### 慢查询

```sql
-- 查询慢sql变量
SHOW VARIABLES LIKE '%slow_query%';
/**
Variable_name	Value
slow_query_log	OFF
slow_query_log_file	/data/mysql/3309/youlin-slow.log
**/

SHOW VARIABLES LIKE '%long_query_time%';
/**
Variable_name	Value
long_query_time	10.000000
**/

SHOW VARIABLES LIKE '%log_output%';
/**
Variable_name	Value
log_output	FILE
**/

-- 开启慢sql监控
SET GLOBAL log_output='TABLE';    -- 开启慢sql日志，记录到mysql.slow_log表
SET GLOBAL slow_query_log='ON';    -- 打开慢sql监控	
SET GLOBAL long_query_time=0.1;    -- 设置超过0.1秒的查询为慢sql

-- 查询慢sql
SELECT * FROM mysql.slow_log;

-- 关闭慢sql监控
SET GLOBAL slow_query_log='OFF';    -- 关闭慢sql监控
```

## 内存

SHOW VARIABLES LIKE 'innodb_buffer_pool_size';

SHOW GLOBAL VARIABLES LIKE '%sort_buffer_size%';

SHOW GLOBAL VARIABLES LIKE 'key_buffer_size';

SHOW GLOBAL VARIABLES LIKE 'innodb_io_capacity';

# 基本操作

## 字符串索引

### 查询字符串前缀索引的区分度

```
SELECT COUNT(DISTINCT ypu.openid) AS L FROM yl_platform_users ypu;

SELECT COUNT(DISTINCT LEFT(ypu.openid, 4)) / COUNT(DISTINCT ypu.openid) AS L4 FROM yl_platform_users ypu;

SELECT COUNT(DISTINCT LEFT(ypu.openid, 5)) / COUNT(DISTINCT ypu.openid) AS L5 FROM yl_platform_users ypu;

SELECT COUNT(DISTINCT LEFT(ypu.openid, 6)) / COUNT(DISTINCT ypu.openid) AS L6 FROM yl_platform_users ypu;

SELECT COUNT(DISTINCT LEFT(ypu.openid, 7)) / COUNT(DISTINCT ypu.openid) AS L7 FROM yl_platform_users ypu;

SELECT COUNT(DISTINCT LEFT(ypu.openid, 8)) / COUNT(DISTINCT ypu.openid) AS L8 FROM yl_platform_users ypu;
```

