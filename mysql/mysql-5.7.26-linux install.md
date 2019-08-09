# 卸载已安装mysql

## RPM包卸载

检查是否安装了mysql
> rpm -qa | grep -i mysql
  
  
卸载前关闭mysql服务
> service mysql status  
> service mysql stop  
> service mysql status  
> chkconfig --list | grep mysql  

收集mysql文件夹信息
> whereis mysql  
> find / -name mysql

卸载删除mysql各类组件
> rpm -ev 组件名称

检查和删除mysql对应的文件夹
> whereis mysql  
> find / -name mysql  
> rm -rf ...

删除mysql用户和用户组
> userdel mysql  
> groupdel mysql	(运行userdel mysql之后通常会连带删除用户组)  

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

# 权限设置

## 用户组权限

查询mysql分组
> more /etc/group | grep mysql  
> groups mysql  

添加mysql用户组
> groupadd mysql  

删除mysql用户组
> groupdel mysql  

## 用户权限

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

# 安装

解压tar包
> tar xzvf mysql-5.7.26-linux-glibc2.12-x86_64.tar.gz

创建数据目录
> mkdir data

给mysql用户赋权
> chown -R mysql .  
> chgrp -R mysql .  
> chown -R mysql:mysql data

删除默认配置文件
> rm -rf /etc/my.cnf

创建软链
> cd /usr/local/  
> ln -s /work/components/mysql-5.7.26-linux-glibc2.12-x86_64 mysql  

初始化数据库，配置参数
> bin/mysqld --initialize --user=mysql --basedir=/usr/local/mysql --datadir=/usr/local/mysql/data  

Note：可能出现如下错误
> bin/mysqld: error while loading shared libraries: libaio.so.1: cannot open shared object file: No such file or directory  

修复上面的错误，需安装libaio
> yum install -y libaio

安装最后一句会提示我们密码，需要记住：
> 2019-05-13T09:26:55.683236Z 1 [Note] A temporary password is generated for root@localhost: (LxR,qhyE9Ej

生成mysql密钥对
> bin/mysql_ssl_rsa_setup --datadir=/usr/local/mysql/data/mysql  

安全启动
> bin/mysqld_safe --user=mysql &

查看进程
> ps -ef|grep mysql

临时密码登录mysql
> bin/mysql -h localhost -u root -p

修改密码，创建远程连接
> set password=password('123456');  
> grant all privileges on *.* to 'root'@'%' identified by '123456';  -- 远程登录密码  
> flush privileges;  
> quit;

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

## 添加环境变量，使用mysql命令
> export PATH=$PATH:/usr/local/mysql/bin


## 使用外部配置文件
拷贝配置文件
> cp /usr/local/mysql/support-files/my-default.cnf /etc/my.cnf

编辑/etc/my.cnf文件

```properties

    basedir = /usr/local/mysql
    datadir = /usr/local/mysql/data
    character_set_server=uft8
    
    [client]
    
    default-character-set=uft8
    #sql_mode=NO_ENGINE_SUBSTITUTION,STRICT_TRANS_TABLES

```





