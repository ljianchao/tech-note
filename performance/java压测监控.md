# java压测监控

## 监控linux系统CPU数据

`vmstat`（Virtual Memory Statistics，虚拟内存统计）可对操作系统的虚拟内存、进程、CPU活动进行监控。

`vmstat`常用命令格式如下：

```
vmstat [options] [delay [count]]
vmstat [-a] [-n] [-S unit] [delay [ count]]
vmstat [-s] [-n] [-S unit]
vmstat [-m] [-n] [delay [ count]]
vmstat [-d] [-n] [delay [ count]]
vmstat [-p disk partition] [-n] [delay [ count]]
vmstat [-f]
vmstat [-V]
```

命令选项说明如下：

```
delay：采样的时间间隔数，默认单位是秒。如果不指定，只显示一条结果。
count：采用次数。如果不指定采用次数，但指定了采样的时间间隔数，这时采用次数为无穷。
-a, --active           active/inactive memory, 显示活跃和非活跃内存
-f, --forks            number of forks since boot, 显示从系统启动至今的fork数量
-m, --slabs            slabinfo, 显示slabinfo
-n, --one-header       do not redisplay header, 只在开始时显示一次各字段名称
-s, --stats            event counter statistics, 显示内存相关统计信息及多种系统活动数量
-d, --disk             disk statistics, 显示磁盘相关统计信息
-D, --disk-sum         summarize disk statistics
-p, --partition <dev>  partition specific statistics, 显示指定磁盘分区统计信息
-S, --unit <char>      define display unit, 使用指定单位显示。参数有 k 、K 、m 、M，分别代表1000、1024、1000000、1048576字节（byte）。默认单位为K（1024 bytes）
-w, --wide             wide output
-t, --timestamp        show timestamp

-h, --help     display this help and exit
-V, --version  output version information and exit, 显示vmstat版本信息
```

实例：

```
// 每间隔1秒打印一次系统维度的CPU情况
vmstat -n 1
// 每间隔1秒采样1次，共采样10次
vmstat -n 1 10
```

`vmstat`命令输出分成六个部分：

```
(1)进程procs： 
    r：在运行队列中等待的进程数
    b：在等待io的进程数
 
 
(2)Linux 内存监控内存memoy： 
    swpd：现时可用的交换内存（单位KB）
    free：空闲的内存（单位KB）
    buff: 缓冲区中的内存数（单位：KB）。
    cache：被用来做为高速缓存的内存数（单位：KB）
 
 
(3) Linux 内存监控swap交换页面 
    si: 从磁盘交换到内存的交换页数量，单位：KB/秒。
    so: 从内存交换到磁盘的交换页数量，单位：KB/秒。
 
 
(4)Linux 内存监控io块设备:
    bi: 发送到块设备的块数，单位：块/秒。
    bo: 从块设备接收到的块数，单位：块/秒。
 
 
(5)Linux 内存监控system系统： 
    in: 每秒的中断数，包括时钟中断。
    cs: 每秒的环境（上下文）转换次数。
 
 
(6)Linux 内存监控cpu中央处理器：
    cs：用户进程使用的时间，以百分比表示
    sy：系统进程使用的时间，以百分比表示
    id：中央处理器的空闲时间，以百分比表示
 
常见诊断：
1.假如 r 经常大于4 ，且 id 经常小于40，表示中央处理器的负荷很重
2.假如 bi，bo 长期不等于0，表示物理内存容量太小
```

参数详解：

```
r 表示运行队列(就是说多少个进程真的分配到CPU)，我测试的服务器目前CPU比较空闲，没什么程序在跑，当这个值超过了CPU数目，就会出现CPU瓶颈了。这个也和top的负载有关系，一般负载超过了3就比较高，超过了5就高，超过了10就不正常了，服务器的状态很危险。top的负载类似每秒的运行队列。如果运行队列过大，表示你的CPU很繁忙，一般会造成CPU使用率很高。

b 表示阻塞的进程,这个不多说，进程阻塞，大家懂的。

swpd 虚拟内存已使用的大小，如果大于0，表示你的机器物理内存不足了，如果不是程序内存泄露的原因，那么你该升级内存了或者把耗内存的任务迁移到其他机器。

free   空闲的物理内存的大小，我的机器内存总共8G，剩余3415M。

buff   Linux/Unix系统是用来存储，目录里面有什么内容，权限等的缓存，我本机大概占用300多M

cache cache直接用来记忆我们打开的文件,给文件做缓冲，我本机大概占用300多M(这里是Linux/Unix的聪明之处，把空闲的物理内存的一部分拿来做文件和目录的缓存，是为了提高 程序执行的性能，当程序使用内存时，buffer/cached会很快地被使用。)

si  每秒从磁盘读入虚拟内存的大小，如果这个值大于0，表示物理内存不够用或者内存泄露了，要查找耗内存进程解决掉。我的机器内存充裕，一切正常。

so  每秒虚拟内存写入磁盘的大小，如果这个值大于0，同上。

bi  块设备每秒接收的块数量，这里的块设备是指系统上所有的磁盘和其他块设备，默认块大小是1024byte，我本机上没什么IO操作，所以一直是0，但是我曾在处理拷贝大量数据(2-3T)的机器上看过可以达到140000/s，磁盘写入速度差不多140M每秒

bo 块设备每秒发送的块数量，例如我们读取文件，bo就要大于0。bi和bo一般都要接近0，不然就是IO过于频繁，需要调整。

in 每秒CPU的中断次数，包括时间中断

cs 每秒上下文切换次数，例如我们调用系统函数，就要进行上下文切换，线程的切换，也要进程上下文切换，这个值要越小越好，太大了，要考虑调低线程或者进程的数目,例如在apache和nginx这种web服务器中，我们一般做性能测试时会进行几千并发甚至几万并发的测试，选择web服务器的进程可以由进程或者线程的峰值一直下调，压测，直到cs到一个比较小的值，这个进程和线程数就是比较合适的值了。系统调用也是，每次调用系统函数，我们的代码就会进入内核空间，导致上下文切换，这个是很耗资源，也要尽量避免频繁调用系统函数。上下文切换次数过多表示你的CPU大部分浪费在上下文切换，导致CPU干正经事的时间少了，CPU没有充分利用，是不可取的。

us 用户CPU时间，我曾经在一个做加密解密很频繁的服务器上，可以看到us接近100,r运行队列达到80(机器在做压力测试，性能表现不佳)。

sy 系统CPU时间，如果太高，表示系统调用时间长，例如是IO操作频繁。

id  空闲 CPU时间，一般来说，id + us + sy = 100,一般我认为id是空闲CPU使用率，us是用户CPU使用率，sy是系统CPU使用率。

wt 等待IO CPU时间。
```

## 监控CPU使用率较高的java进程

抓取高占用CPU的线程的简单方法：

- 运行`top`命令取得JAVA进程号(PID)，假如是`2068`；
- 运行`jstack <pid>` 命令导出JAVA线程信息到`tmp.txt`文件，命令行为`jstack 2068 > tmp.txt`
- 运行`top -H -p 2068` 查看线程的CPU占用排名情况，并记录PID，假如是`2079`；
- 上一步记录的PID为十进制，转换为16进制的数字，`printf '%x\n' 2079`，得到结果为`81f`
- 在文件`tmp.txt`中查找`nid=0x81f`线程对应的堆栈信息

## 参考

- java性能权威指南
- 深入理解Java虚拟机-JVM高级特性与最佳实践
- [Linux vmstat命令详解](https://www.jellythink.com/archives/419)
- [Linux性能监测：CPU篇](https://www.jellythink.com/archives/432)
- [Linux top命令详解](https://www.jellythink.com/archives/421)
- [Linux系统内存监控、性能诊断工具vmstat命令详解](http://zhangge.net/4936.html)
- [抓取占用CPU高的JAVA线程，进而找出有问题的WEB页面](https://zhangge.net/131.html)
- [linux CPU占用飙升的排查方法（一）](https://www.jianshu.com/p/778ffa2ab881)
