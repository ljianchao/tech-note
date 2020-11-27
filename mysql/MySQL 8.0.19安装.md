# MySQL 8.0.19安装

## 环境

- 操作系统: Linux - Centos-7-x86_64-1810
- MySQL版本: 8.0.19安装

## 卸载已安装mysql

### RPM包卸载

检查是否安装了mysql

```shell
    rpm -qa | grep -i mysql
```
  
卸载前关闭mysql服务

```shell
    # 查看mysql服务状态
    service mysql status  
    # 停止mysql服务
    service mysql stop
    # 查看mysql服务的开机启动状态
    chkconfig --list | grep mysql  
```

卸载删除mysql各类rpm组件

```shell
    rpm -ev <组件名称>
```

检查和删除mysql对应的目录

```shell
    # 定位mysql相关的目录
    whereis mysql  
    find / -name mysql  
    # 删除mysql相关的目录
    rm -rf <目录>
```

删除mysql用户和用户组

```shell
    userdel mysql  
    # 运行userdel mysql之后通常会连带删除用户组
    groupdel mysql
```

### 二进制包/源码安装方式的mysql卸载

检查MySQL服务并关闭服务

```shell
    ps -ef|grep mysql  
    /etc/init.d/mysql status  
    /etc/init.d/mysql stop  
```

检查和删除mysql对应的目录

```shell
    # 定位mysql相关的目录
    whereis mysql  
    find / -name mysql  
    # 删除mysql相关的目录
    rm -rf <目录>
```

删除mysql相关配置文件

```shell
    rm -rf /etc/my.cnf  
    rm -rf /etc/init.d/mysql 
```

删除mysql用户和用户组

```shell
    userdel mysql  
    # 运行userdel mysql之后通常会连带删除用户组
    groupdel mysql
```

## 权限设置

用户组权限相关命令：

```shell
    # 查询mysql分组
    more /etc/group | grep mysql  
    groups mysql  

    # 添加mysql用户组
    groupadd mysql  

    # 删除mysql用户组
    groupdel mysql  
```

用户权限相关命令：

```shell
    # 查询用户
    more /etc/passwd | grep mysql  
    more /etc/shadow | grep mysql  
    id mysql 

    # 添加mysql用户
    useradd -r -g mysql -s /bin/false mysql

    # 删除用户
    userdel mysql
```

**Note**
> Because the user is required only for ownership purposes, not login purposes, the useradd command uses the `-r` and `-s /bin/false` options to create a user that does not have login permissions to your server host. Omit these options if your useradd does not support them.

## 安装

mysql安装在`/opt/sof/`目录下，数据统一存储在`/data/mysql/`目录下，可以在相应的目录下创建端口目录来进行区分多个mysql服务，本次安装采用默认端口`3306`，因此数据存放在`/data/mysql/3306/`目录下

### 1. 下载安装包

到[mysql官方网站](https://downloads.mysql.com/archives/community/)选择合适的源码包进行下载，我们选择`mysql-8.0.19-linux-glibc2.12-x86_64.tar.xz`这个版本。

### 2. 解压tar包

```shell
    tar xvf mysql-8.0.19-linux-glibc2.12-x86_64.tar.xz
```

### 3. 创建软链

```shell
    cd /usr/local/  
    ln -s /opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64 mysql
```

### 4. mysql目录赋权

```shell
    chown -R mysql:mysql /opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64
    chmod 750 /opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64
```


### 5. 创建数据目录并赋权

```shell
    mkdir -p /data/mysql/3306
    chown -R mysql:mysql /data/mysql/3306/
```

### 6. 创建配置文件

拷贝配置文件`/etc/my.cnf`到`/etc/my_3306.cnf`

```shell
    cp /etc/my.cnf /etc/my_3306.cnf
```

修改`/etc/my_3306.cnf`文件内容，下面一个为带注释的版本，一个为实际使用的无注释的版本。

注释版本：

```conf
[mysqld_safe]
# 记录mysql线程id号
pid-file=/data/mysql/3306/mysqld.pid

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
sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION'
log_bin_trust_function_creators=1
max_connections=2000
```

无注释版本：

```conf
[mysqld_safe]
pid-file=/data/mysql/3306/mysqld.pid

[mysql]
port=3306
default-character-set=utf8mb4

[client]
port=3306
socket=/data/mysql/3306/mysqld_3306.sock
default-character-set=utf8mb4

[mysqld]
bind_address=0.0.0.0
port=3306
pid_file=/data/mysql/3306/mysqld.pid
init_connect='SET NAMES utf8mb4'
character_set_server=utf8mb4
collation-server=utf8mb4_bin
open_files_limit=65536
basedir=/opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64
datadir=/data/mysql/3306
tmpdir=/data/mysql/3306
log-error=/data/mysql/3306/alert.log
socket=/data/mysql/3306/mysqld_3306.sock
log-bin=/data/mysql/3306/mysqld-bin
relay-log=/data/mysql/3306/relay-bin
server_id=255128
binlog_format=row
sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION'
log_bin_trust_function_creators=1
max_connections=2000
```

#### 6.1 修改sql的模式

执行以下语句修改sql的模式

```sql
    ## 查询当前模式
    SELECT @@GLOBAL.sql_mode;
    ## 修改模式
    set global sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION';
```

修改配置文件`my_3306.cnf`，防止重启失效

```conf
    [mysqld]
    sql_mode='STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_ENGINE_SUBSTITUTION'
```

#### 6.2 解决开启bin-log函数同步的问题

```sql
    ## 查询配置
    SHOW GLOBAL VARIABLES LIKE 'log_bin_trust_function_creators';

    ## 更改配置
    SET GLOBAL log_bin_trust_function_creators=1;
```

修改配置文件`my_3306.cnf`，防止重启失效

```conf
    [mysqld]
    log_bin_trust_function_creators=1
```

#### 6.3 设置最大连接数

```sql
    ## 查询最大连接数，默认151
    SHOW VARIABLES LIKE 'max_connections';

    ## 更改配置
    SET GLOBAL max_connections=2000;
```

修改配置文件`my_3306.cnf`，防止重启失效

```conf
    [mysqld]
    max_connections=2000
```

### 7. 初始化数据库，配置参数

#### 7.1 检查依赖

安装过程可能出现如下错误

```shell
    bin/mysqld: error while loading shared libraries: libaio.so.1: cannot open shared object file: No such file or directory 
```

**Note**:
> MySQL has a dependency on the `libaio` library. Data directory initialization and subsequent server startup steps will fail if this library is not installed locally.

修复上面的错误，需安装`libaio` library

```shell 
    # 查询是否已安装libaio
    yum list installed | grep libaio
    # yum 安装
    yum install -y libaio
```

#### 7.2 初始化数据库

```shell
    # 可以指定数据目录
    bin/mysqld --defaults-file=/etc/my_3306.cnf --initialize --user=mysql
```

**Note**：去查看日志文件`/data/mysql/3306/alert.log`，最后一句会提示我们密码，需要记住：

```shell
    2020-11-27T06:27:32.387591Z 5 [Note] [MY-010454] [Server] A temporary password is generated for root@localhost: deNeyfW8rQ&I
```

### 8. 生成mysql密钥对

```shell
    # 指定数据目录
    bin/mysql_ssl_rsa_setup --datadir=/data/mysql/3306
```
 
### 9. 安全启动

```shell
    # 指定目录方式启动
    bin/mysqld_safe --defaults-file=/etc/my_3306.cnf --user=mysql &
    # 查看进程
    ps -ef|grep mysql
```

### 10. 临时密码登录mysql

```shell
    bin/mysql -h 127.0.0.1 -P 3306 -u root -p
```

此时可能会报错：

```shell
    [root@localhost mysql]# bin/mysql -h 127.0.0.1 -P 3306 -u root -p
    bin/mysql: error while loading shared libraries: libncurses.so.5: cannot open shared object file: No such file or directory
```

yum方式安装：

```shell
    yum install libncurses*
```

也可以从网站：`pkgs.org`中搜索`libncurses.so.5`的rpm包进行安装。

rpm包安装还可能遇到以下错误：

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

### 11. 修改密码，创建远程连接

```shell
    # set password=password('123456');  mysql 5.7.9以后废弃了password字段和password()函数，
    # authentication_string字段表示用户密码
    # 设置本地登录的密码
    ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '123456';    

    # 设置root远程登陆的账号和权限
    CREATE USER 'root'@'%' IDENTIFIED BY '123456';
    # 必须用此语句修改密码，否则远程客户端连接会报密码的错误
    ALTER USER 'root'@'%' IDENTIFIED WITH mysql_native_password BY '123456';  
    GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' WITH GRANT OPTION;
    flush privileges;  
    USE mysql;
    SELECT user,host FROM user;
    quit;
```

用户权限内容：

```
    mysql> SELECT user,host FROM user;
    +------------------+-----------+
    | user             | host      |
    +------------------+-----------+
    | root             | %         |
    | mysql.infoschema | localhost |
    | mysql.session    | localhost |
    | mysql.sys        | localhost |
    | root             | localhost |
    +------------------+-----------+
```

**NOTE**: CentOS 7防火墙默认没有开放`3306`端口，需要以下命令添加端口：

```shell
    # 添加端口
    firewall-cmd --zone=public --add-port=3306/tcp --permanent
    # 重启防火墙
    firewall-cmd --reload 
```

## 设置mysql服务开机启动

默认端口号为`3306`，同一服务器中可能部署多个mysql实例，因此启动服务我们添加端口后缀`3306`，先拷贝文件：

```shell
    # 将`support-files/mysql.server`文件拷贝为`/etc/init.d/mysql_3306`
    cp /usr/local/mysql/support-files/mysql.server /etc/init.d/mysql_3306
```

然后修改`/etc/init.d/mysql_3306`里的内容，在`case "$mode" in`之前增加以下内容：

```
    basedir="/opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64"
    bindir="$basedir/bin"
    datadir="/data/mysql/3306"
    mysqld_pid_file_path="$datadir/mysqld.pid"
    confdir="/etc/my_3306.cnf"
```

修改`start`服务，添加`--defaults-file`参数：

```shell
    $bindir/mysqld_safe --defaults-file="$confdir" --datadir="$datadir" --pid-file="$mysqld_pid_file_path" $other_args >/dev/null &
```

完整的`/etc/init.d/mysql_3306`内容看文末。

最后注册服务

```shell
    # 查看mysql服务是否在服务配置中
    chkconfig --list mysql_3306

    # 注册开机启动服务
    chkconfig --add mysql_3306
    # 删除开启启动服务
    chkconfig --del mysql_3306

    # 启动或停止mysql服务
    service mysql_3306 start
    service mysql_3306 stop
```

## 设置环境变量

编辑`/etc/profile`文件，在最后添加mysql目录的环境变量

```
    export PATH=$PATH:/usr/local/mysql/bin
```

刷新环境变量

```
    source /etc/profile
```

## 完整的`/etc/init.d/mysql_3306`

内容如下：

```shell
#!/bin/sh
# Copyright Abandoned 1996 TCX DataKonsult AB & Monty Program KB & Detron HB
# This file is public domain and comes with NO WARRANTY of any kind

# MySQL daemon start/stop script.

# Usually this is put in /etc/init.d (at least on machines SYSV R4 based
# systems) and linked to /etc/rc3.d/S99mysql and /etc/rc0.d/K01mysql.
# When this is done the mysql server will be started when the machine is
# started and shut down when the systems goes down.

# Comments to support chkconfig on RedHat Linux
# chkconfig: 2345 64 36
# description: A very fast and reliable SQL database engine.

# Comments to support LSB init script conventions
### BEGIN INIT INFO
# Provides: mysql
# Required-Start: $local_fs $network $remote_fs
# Should-Start: ypbind nscd ldap ntpd xntpd
# Required-Stop: $local_fs $network $remote_fs
# Default-Start:  2 3 4 5
# Default-Stop: 0 1 6
# Short-Description: start and stop MySQL
# Description: MySQL is a very fast and reliable SQL database engine.
### END INIT INFO
 
# If you install MySQL on some other places than /usr/local/mysql, then you
# have to do one of the following things for this script to work:
#
# - Run this script from within the MySQL installation directory
# - Create a /etc/my.cnf file with the following information:
#   [mysqld]
#   basedir=<path-to-mysql-installation-directory>
# - Add the above to any other configuration file (for example ~/.my.ini)
#   and copy my_print_defaults to /usr/bin
# - Add the path to the mysql-installation-directory to the basedir variable
#   below.
#
# If you want to affect other MySQL variables, you should make your changes
# in the /etc/my.cnf, ~/.my.cnf or other MySQL configuration files.

# If you change base dir, you must also change datadir. These may get
# overwritten by settings in the MySQL configuration files.

basedir=
datadir=

# Default value, in seconds, afterwhich the script should timeout waiting
# for server start. 
# Value here is overriden by value in my.cnf. 
# 0 means don't wait at all
# Negative numbers mean to wait indefinitely
service_startup_timeout=900

# Lock directory for RedHat / SuSE.
lockdir='/var/lock/subsys'
lock_file_path="$lockdir/mysql"

# The following variables are only set for letting mysql.server find things.

# Set some defaults
mysqld_pid_file_path=
if test -z "$basedir"
then
  basedir=/usr/local/mysql
  bindir=/usr/local/mysql/bin
  if test -z "$datadir"
  then
    datadir=/usr/local/mysql/data
  fi
  sbindir=/usr/local/mysql/bin
  libexecdir=/usr/local/mysql/bin
else
  bindir="$basedir/bin"
  if test -z "$datadir"
  then
    datadir="$basedir/data"
  fi
  sbindir="$basedir/sbin"
  libexecdir="$basedir/libexec"
fi

# datadir_set is used to determine if datadir was set (and so should be
# *not* set inside of the --basedir= handler.)
datadir_set=

#
# Use LSB init script functions for printing messages, if possible
#
lsb_functions="/lib/lsb/init-functions"
if test -f $lsb_functions ; then
  . $lsb_functions
else
  log_success_msg()
  {
    echo " SUCCESS! $@"
  }
  log_failure_msg()
  {
    echo " ERROR! $@"
  }
fi

PATH="/sbin:/usr/sbin:/bin:/usr/bin:$basedir/bin"
export PATH

mode=$1    # start or stop

[ $# -ge 1 ] && shift


other_args="$*"   # uncommon, but needed when called from an RPM upgrade action
           # Expected: "--skip-networking --skip-grant-tables"
           # They are not checked here, intentionally, as it is the resposibility
           # of the "spec" file author to give correct arguments only.

case `echo "testing\c"`,`echo -n testing` in
    *c*,-n*) echo_n=   echo_c=     ;;
    *c*,*)   echo_n=-n echo_c=     ;;
    *)       echo_n=   echo_c='\c' ;;
esac

parse_server_arguments() {
  for arg do
    case "$arg" in
      --basedir=*)  basedir=`echo "$arg" | sed -e 's/^[^=]*=//'`
                    bindir="$basedir/bin"
		    if test -z "$datadir_set"; then
		      datadir="$basedir/data"
		    fi
		    sbindir="$basedir/sbin"
		    libexecdir="$basedir/libexec"
        ;;
      --datadir=*)  datadir=`echo "$arg" | sed -e 's/^[^=]*=//'`
		    datadir_set=1
	;;
      --pid-file=*) mysqld_pid_file_path=`echo "$arg" | sed -e 's/^[^=]*=//'` ;;
      --service-startup-timeout=*) service_startup_timeout=`echo "$arg" | sed -e 's/^[^=]*=//'` ;;
    esac
  done
}

wait_for_pid () {
  verb="$1"           # created | removed
  pid="$2"            # process ID of the program operating on the pid-file
  pid_file_path="$3" # path to the PID file.

  i=0
  avoid_race_condition="by checking again"

  while test $i -ne $service_startup_timeout ; do

    case "$verb" in
      'created')
        # wait for a PID-file to pop into existence.
        test -s "$pid_file_path" && i='' && break
        ;;
      'removed')
        # wait for this PID-file to disappear
        test ! -s "$pid_file_path" && i='' && break
        ;;
      *)
        echo "wait_for_pid () usage: wait_for_pid created|removed pid pid_file_path"
        exit 1
        ;;
    esac

    # if server isn't running, then pid-file will never be updated
    if test -n "$pid"; then
      if kill -0 "$pid" 2>/dev/null; then
        :  # the server still runs
      else
        # The server may have exited between the last pid-file check and now.  
        if test -n "$avoid_race_condition"; then
          avoid_race_condition=""
          continue  # Check again.
        fi

        # there's nothing that will affect the file.
        log_failure_msg "The server quit without updating PID file ($pid_file_path)."
        return 1  # not waiting any more.
      fi
    fi

    echo $echo_n ".$echo_c"
    i=`expr $i + 1`
    sleep 1

  done

  if test -z "$i" ; then
    log_success_msg
    return 0
  else
    log_failure_msg
    return 1
  fi
}

# Get arguments from the my.cnf file,
# the only group, which is read from now on is [mysqld]
if test -x "$bindir/my_print_defaults";  then
  print_defaults="$bindir/my_print_defaults"
else
  # Try to find basedir in /etc/my.cnf
  conf=/etc/my.cnf
  print_defaults=
  if test -r $conf
  then
    subpat='^[^=]*basedir[^=]*=\(.*\)$'
    dirs=`sed -e "/$subpat/!d" -e 's//\1/' $conf`
    for d in $dirs
    do
      d=`echo $d | sed -e 's/[ 	]//g'`
      if test -x "$d/bin/my_print_defaults"
      then
        print_defaults="$d/bin/my_print_defaults"
        break
      fi
    done
  fi

  # Hope it's in the PATH ... but I doubt it
  test -z "$print_defaults" && print_defaults="my_print_defaults"
fi

#
# Read defaults file from 'basedir'.   If there is no defaults file there
# check if it's in the old (depricated) place (datadir) and read it from there
#

extra_args=""
if test -r "$basedir/my.cnf"
then
  extra_args="-e $basedir/my.cnf"
fi

parse_server_arguments `$print_defaults $extra_args mysqld server mysql_server mysql.server`

#
# Set pid file if not given
#
if test -z "$mysqld_pid_file_path"
then
  mysqld_pid_file_path=$datadir/`hostname`.pid
else
  case "$mysqld_pid_file_path" in
    /* ) ;;
    * )  mysqld_pid_file_path="$datadir/$mysqld_pid_file_path" ;;
  esac
fi

basedir="/opt/soft/mysql-8.0.19-linux-glibc2.12-x86_64"
bindir="$basedir/bin"
datadir="/data/mysql/3306"
mysqld_pid_file_path="$datadir/mysqld.pid"
confdir="/etc/my_3306.cnf"

case "$mode" in
  'start')
    # Start daemon

    # Safeguard (relative paths, core dumps..)
    cd $basedir

    echo $echo_n "Starting MySQL"
    if test -x $bindir/mysqld_safe
    then
      # Give extra arguments to mysqld with the my.cnf file. This script
      # may be overwritten at next upgrade.
      # $bindir/mysqld_safe --datadir="$datadir" --pid-file="$mysqld_pid_file_path" $other_args >/dev/null &
      $bindir/mysqld_safe --defaults-file="$confdir" --datadir="$datadir" --pid-file="$mysqld_pid_file_path" $other_args >/dev/null &
      wait_for_pid created "$!" "$mysqld_pid_file_path"; return_value=$?

      # Make lock for RedHat / SuSE
      if test -w "$lockdir"
      then
        touch "$lock_file_path"
      fi

      exit $return_value
    else
      log_failure_msg "Couldn't find MySQL server ($bindir/mysqld_safe)"
    fi
    ;;

  'stop')
    # Stop daemon. We use a signal here to avoid having to know the
    # root password.

    if test -s "$mysqld_pid_file_path"
    then
      # signal mysqld_safe that it needs to stop
      touch "$mysqld_pid_file_path.shutdown"

      mysqld_pid=`cat "$mysqld_pid_file_path"`

      if (kill -0 $mysqld_pid 2>/dev/null)
      then
        echo $echo_n "Shutting down MySQL"
        kill $mysqld_pid
        # mysqld should remove the pid file when it exits, so wait for it.
        wait_for_pid removed "$mysqld_pid" "$mysqld_pid_file_path"; return_value=$?
      else
        log_failure_msg "MySQL server process #$mysqld_pid is not running!"
        rm "$mysqld_pid_file_path"
      fi

      # Delete lock for RedHat / SuSE
      if test -f "$lock_file_path"
      then
        rm -f "$lock_file_path"
      fi
      exit $return_value
    else
      log_failure_msg "MySQL server PID file could not be found!"
    fi
    ;;

  'restart')
    # Stop the service and regardless of whether it was
    # running or not, start it again.
    if $0 stop  $other_args; then
      $0 start $other_args
    else
      log_failure_msg "Failed to stop running server, so refusing to try to start."
      exit 1
    fi
    ;;

  'reload'|'force-reload')
    if test -s "$mysqld_pid_file_path" ; then
      read mysqld_pid <  "$mysqld_pid_file_path"
      kill -HUP $mysqld_pid && log_success_msg "Reloading service MySQL"
      touch "$mysqld_pid_file_path"
    else
      log_failure_msg "MySQL PID file could not be found!"
      exit 1
    fi
    ;;
  'status')
    # First, check to see if pid file exists
    if test -s "$mysqld_pid_file_path" ; then 
      read mysqld_pid < "$mysqld_pid_file_path"
      if kill -0 $mysqld_pid 2>/dev/null ; then 
        log_success_msg "MySQL running ($mysqld_pid)"
        exit 0
      else
        log_failure_msg "MySQL is not running, but PID file exists"
        exit 1
      fi
    else
      # Try to find appropriate mysqld process
      mysqld_pid=`pidof $libexecdir/mysqld`

      # test if multiple pids exist
      pid_count=`echo $mysqld_pid | wc -w`
      if test $pid_count -gt 1 ; then
        log_failure_msg "Multiple MySQL running but PID file could not be found ($mysqld_pid)"
        exit 5
      elif test -z $mysqld_pid ; then 
        if test -f "$lock_file_path" ; then 
          log_failure_msg "MySQL is not running, but lock file ($lock_file_path) exists"
          exit 2
        fi 
        log_failure_msg "MySQL is not running"
        exit 3
      else
        log_failure_msg "MySQL is running but PID file could not be found"
        exit 4
      fi
    fi
    ;;
    *)
      # usage
      basename=`basename "$0"`
      echo "Usage: $basename  {start|stop|restart|reload|force-reload|status}  [ MySQL server options ]"
      exit 1
    ;;
esac

exit 0

```

## 参考

- [安装mysql8.0.11及修改root密码、连接navicat for mysql的思路详解](https://www.jb51.net/article/142025.htm)
- [sqlyog连接mysql8.0](https://www.cnblogs.com/diaoye/p/9665916.html)
- [Centos7 firewall开放3306端口](https://www.cnblogs.com/huizhipeng/p/10127333.html)
- [解决Can not connect to local MySQL server through socket /tmp/mysql.sock (2)](https://blog.csdn.net/liu911025/article/details/106636790)
- [ERROR 2002 (HY000): Can't connect to local MySQL server through socket '/var/lib/mysql/mysql.sock' (2)](https://www.cnblogs.com/kerrycode/p/4690948.html)
- [XtraBackup出现 Can't connect to local MySQL server through socket '/tmp/mysql.sock'](https://www.cnblogs.com/kerrycode/p/5309083.html)
