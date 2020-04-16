## linux安装vmvare-tools
    sudo ./vmware-install.pl

## 创建java文件夹    
    sudo mkdir java

## 修改root默认密码
    sudo passwd  root

## SSH服务
    3.1 查看是否已安装ssh服务
        rpm -qa | grep openssh-server
        openssh-server-5.3p1-19.fc12.i686 （这行表示已安装）
        若未安装ssh服务，可输入：
        yum install openssh-server 
        进行安装
    3.2 查看ssh服务状态
        service sshd status
    3.3 启动ssh服务
        service sshd start
    3.4 设置系统启动时开启服务
        systemctl enable sshd.service


## 防火墙
    3.5 开启防火墙22端口
       iptables -A INPUT -p tcp --dport 22 -j ACCEPT
       也可以将上述参数加入防火墙配置中：
       vi /etc/sysconfig/iptables
       加入：-A INPUT -m state --state NEW -m tcp -p tcp --dport 22 -j ACCEPT
       保存后重启iptables即可
       详情可以查阅 iptables的用法

    3.6 防火墙
CentOS 7.0默认使用的是firewall作为防火墙，使用iptables必须重新设置一下
1、直接关闭防火墙
systemctl stop firewalld.service #停止firewall
systemctl disable firewalld.service #禁止firewall开机启动
2、设置 iptables service
yum -y install iptables-services
如果要修改防火墙配置，如增加防火墙端口3306
vi /etc/sysconfig/iptables 
增加规则
-A INPUT -m state --state NEW -m tcp -p tcp --dport 3306 -j ACCEPT
保存退出后
systemctl restart iptables.service #重启防火墙使配置生效
systemctl enable iptables.service #设置防火墙开机启动
最后重启系统使设置生效即可。

1. 防火墙
    4.1 查看防火墙的状态
    service iptables status
    4.2 关闭防火墙
    service iptables stop

2. 安装gcc
   6.1 查看是否安装gcc
        rpm -qa | grep gcc
   6.2 安装gcc
       yum install gcc

3. SELinux
    7.1 查看状态
        getenforce
        sestatus [-vb]

4. 安装vim
    8.1 安装vim
        yum install vim
    发生如下错误：Transaction check error:
    file /usr/share/man/man1/vim.1.gz from install of vim-common-2:8.0.206-1.fc25.x86_64 conflicts with file from package vim-minimal-2:7.4.1989-2.fc25.x86_64

    解决方法：
先更新了下vi，再装vim就好了
就是yum update vi
然后yum install vim


9. 清空安装缓存
    8.1 清空安装缓存
        dnf clean packages 



阵子在虚拟机上装好了centos5.3，并配好了nginx+php+mysql，但是本机就是无法访问。一直就没去折腾了。
具体情况如下
1。本机能ping通虚拟机
2。虚拟机也能ping通本机
3。虚拟机能访问自己的web
4。本机无法访问虚拟己的web
后来发现是防火墙将80端口屏蔽了的缘故。
检查是不是服务器的80端口被防火墙堵了，可以通过命令：telnet server_ip 80 来测试。
解决方法如下：
/sbin/iptables -I INPUT -p tcp --dport 80 -j ACCEPT
然后保存：
/etc/rc.d/init.d/iptables save
重启防火墙
/etc/init.d/iptables restart
CentOS防火墙的关闭，关闭其服务即可：
查看CentOS防火墙信息：/etc/init.d/iptables status
关闭CentOS防火墙服务：/etc/init.d/iptables stop
永久关闭防火墙：
chkconfig –level 35 iptables off

