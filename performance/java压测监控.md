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
- 运行`jstack + pid` 命令导出JAVA线程信息到`tmp.txt`文件，命令行为`jstack 2068 > tmp.txt`
- 运行`top -H -p 2068` 查看线程的CPU占用排名情况，并记录PID，假如是`2079`；
- 上一步记录的PID为十进制，转换为16进制的数字，`printf '%x\n' 2079`，得到结果为`81f`
- 在文件`tmp.txt`中查找`nid=0x81f`线程对应的堆栈信息

## 监控jvm数据

JDK自带下列工具：

- jps（JVM Process Status Tool），显示指定系统内所有的HotSpot虚拟机进程
- jcmd，打印Java进程所涉及的基本类、线程和VM信息。
- jconsole，提供JVM活动的图形化视图，包括线程的使用、类的使用和GC活动。
- jhat（JVM Heap Dump Browser），读取内存堆转储，并有助于分析。这是事后使用的工具。
- jmap（Memory Map for Java），提供堆转储和其他JVM内存使用的信息。可以适用于脚本，但堆转储必须在事后分析工具中使用。
- jinfo（Configuration Info for Java），查看JVM的系统属性，可以动态设置一些系统属性。可适用于脚本。
- jstack（Stack Trace for Java），转储JVM进程的栈信息。可适用于脚本。
- jstat（JVM Statistics Monitoring Tool），提供GC和类装载活动的信息。可适用于脚本。
- jvisualvm，监视JVM的GUI工具，可用来剖析运行的应用，分析JVM堆转储（事后活动，虽然jvisualvm也可以实时抓取程序的堆转储）。

### 基本的VM信息

命令格式：

```
Usage: jcmd <pid | main class> <command ...|PerfCounter.print|-f file>
or: jcmd -l                                                    
or: jcmd -h                                                    
                                                                
command must be a valid jcmd command for the selected jvm.      
Use the command "help" to see which commands are available.   
If the pid is 0, commands will be sent to all Java processes.   
The main class argument will be used to match (either partially 
or fully) the class used to start Java.                         
If no options are given, lists Java processes (same as -p).     
                                                                
PerfCounter.print display the counters exposed by this process  
-f  read and execute commands from the file                     
-l  list JVM processes on the local machine                     
-h  this help 
```

```
Usage:
    jinfo [option] <pid>
        (to connect to running process)
    jinfo [option] <executable <core>
        (to connect to a core file)
    jinfo [option] [server_id@]<remote server IP or hostname>
        (to connect to remote debug server)

where <option> is one of:
    -flag <name>         to print the value of the named VM flag
    -flag [+|-]<name>    to enable or disable the named VM flag
    -flag <name>=<value> to set the named VM flag to the given value
    -flags               to print VM flags
    -sysprops            to print Java system properties
    <no option>          to print both of the above
    -h | -help           to print this help message
```

常用命令：

```
// 运行时间
jcmd process_id VM.uptime

// 系统属性
jcmd process_id VM.system_properties
jinfo -sysprops process_id

// JVM版本
jcmd process_id VM.version

// 显示程序所用的命令行
jcmd process_id VM.command_line

// JVM调优标识，结果中的中间列有冒号表示标志使用的是非默认值
// 某些标志的默认值在不同平台上可能会不相同，结果的最右列会指示。
// product表示在所有平台上的默认值都是一致的；pd product表示标志的默认值是独立于平台的；
// manageable表示运行时可以动态更改标志的值；
// C2 diagnostic为编译工程师提供诊断输出，帮助理解编译器正以什么方式运作
jcmd process_id VM.flags [-all]
java [other_options] -XX:+PrintFlagsFinal -version
```

jinfo在检查单个标志时很有用，同时允许程序在执行时更改某个标志的值：

```
// 获取进程中所有标志的值
jinfo -flags process_id

// 检查某个标志的值
jinfo -flag PrintGCDetails process_id

// 开启或关闭某个标志，对大多数影响GC算法行为的标志更改不会导致JVM改变它的行为，
// 一般只对那些在Printflagsfinal输出中标记为manageable的标志有效
jinfo -flag +PrintGCDetails process_id
jinfo -flag -PrintGCDetails process_id
```

### jps：虚拟机进程状况工具

jps可以列出正在运行的虚拟机进程，并显示虚拟机执行主类（Main Class，main()函数所在的类）名称以及 这些进程的本地虚拟机唯一ID（Local Virual Machine Identifier，LVMID）。

命令格式：

```
usage: jps [-help]
    jps [-q] [-mlvV] [<hostid>]

Definitions:
    <hostid>:      <hostname>[:<port>]
```

常用命令：

```
// 输出主类的全名
jps -l

// 输出虚拟机进程启动时JVM参数
jps -v

// 输出主类的全名和传递给主类main()函数的参数
jps -lm
```

### jstat：虚拟机统计信息监视工具

jstat（JVM Statistics Monitoring Tool）是用于监视虚拟机各种运行状态信息的命令行工具。它可以显示本地或远程虚拟机进程中的类加载、内存、垃圾收集、JIT编译等运行数据。

命令格式：

```
Usage: jstat -help|-options
    jstat -<option> [-t] [-h<lines>] <vmid> [<interval> [<count>]]

Definitions:
<option>      An option reported by the -options option
<vmid>        Virtual Machine Identifier. A vmid takes the following form:
                    <lvmid>[@<hostname>[:<port>]]
                Where <lvmid> is the local vm identifier for the target
                Java virtual machine, typically a process id; <hostname> is
                the name of the host running the target Java virtual machine;
                and <port> is the port number for the rmiregistry on the
                target host. See the jvmstat documentation for a more complete
                description of the Virtual Machine Identifier.
<lines>       Number of samples between header lines.
<interval>    Sampling interval. The following forms are allowed:
                    <n>["ms"|"s"]
                Where <n> is an integer and the suffix specifies the units as 
                milliseconds("ms") or seconds("s"). The default units are "ms".
<count>       Number of samples to take before terminating.
-J<flag>      Pass <flag> directly to the runtime system.
```

常用命令：

```
// 监视类装载、卸载数量、总空间以及类装载所耗费的时间
jstat -class process_id 1s

// 间隔1秒查询一次垃圾收集状况，共查询10次
jstat -gc process_id 1s 10
jstat -gc process_id 1000ms 10

// 监视Java堆各个区域使用到的最大、最小空间
jstat -gccapacity process_id 1s

// 监视Java堆各个区域使用空间占总空间的百分比
jstat -gcutil process_id 1s

// 监视Java堆各个区域使用空间占总空间的百分比，同时输出导致上一次GC产生的原因
jstat -gccause process_id 1s

// 监视新生代GC状况
jstat -gcnew process_id 1s

// 监视新生代的最大、最小空间
jstat -gcnewcapacity process_id 1s

// 监视老年代GC状况
jstat -gcold process_id 1s

// 监视老年代的最大、最小空间
jstat -gcoldcapacity process_id 1s

// 输出JIT编译器编译过的方法、耗时等信息
jstat -compiler process_id 1s

// 输出已经被JIT编译器编译过的方法
jstat -printcompilation process_id 1s
```

### jmap：Java内存映像工具

jmap命令用于生成堆转储快照，还可以查询finalize执行队列、Java堆和永久代的详细信息，如空间使用率、当前用的是哪种收集器等。

命令格式：

```
Usage:
jmap [option] <pid>
    (to connect to running process)
jmap [option] <executable <core>
    (to connect to a core file)
jmap [option] [server_id@]<remote server IP or hostname>
    (to connect to remote debug server)

where <option> is one of:
    <none>               to print same info as Solaris pmap
    -heap                to print java heap summary
    -histo[:live]        to print histogram of java object heap; if the "live"
                        suboption is specified, only count live objects
    -clstats             to print class loader statistics
    -finalizerinfo       to print information on objects awaiting finalization
    -dump:<dump-options> to dump java heap in hprof binary format
                        dump-options:
                        live         dump only live objects; if not specified,
                                        all objects in the heap are dumped.
                        format=b     binary format
                        file=<file>  dump heap to <file>
                        Example: jmap -dump:live,format=b,file=heap.bin <pid>
    -F                   force. Use with -dump:<dump-options> <pid> or -histo
                        to force a heap dump or histogram when <pid> does not
                        respond. The "live" suboption is not supported
                        in this mode.
    -h | -help           to print this help message
    -J<flag>             to pass <flag> directly to the runtime system
```

常用命令

```
// 生成java堆转储快照，格式为：-dump:[live,]format=b,file=<filename>，
// 其中live子参数说明是否只dump出存活的对象
jmap -dump:format=b,file=dump.bin process_id

// 当虚拟机进程对-dump选项没有响应时，可使用`-F`选项强制生成dump快照
jmap -F -dump:format=b,file=dump.bin process_id

// 显示java堆详细信息，包括回收器、参数设置、分代状况等
jmap -heap process_id

// 显示堆中对象统计信息，包括类、实例数量、合计容量
jmap -histo process_id

// 统计ClassLoader信息
jmap -clstats process_id

// 显示在F-Queue中等待Finalizer线程执行finalize方法的对象
jmap -finalizerinfo process_id
```

### jstack：Java堆栈跟踪工具

jstack命令用于生成虚拟当前时刻的线程快照。

命令格式：


```
Usage:
jstack [-l] <pid>
    (to connect to running process)
jstack -F [-m] [-l] <pid>
    (to connect to a hung process)
jstack [-m] [-l] <executable> <core>
    (to connect to a core file)
jstack [-m] [-l] [server_id@]<remote server IP or hostname>
    (to connect to a remote debug server)

Options:
    -F  to force a thread dump. Use when jstack <pid> does not respond (process is hung)
    -m  to print both java and native frames (mixed mode)
    -l  long listing. Prints additional information about locks
    -h or -help to print this help message
```

常用命令

```
// 显示堆栈和锁的信息
jstack -l process_id

// 如果调用到本地方法的话，可以显示C/C++的堆栈
jstack -m process_id

// 当正常输出的请求不被响应时，强制输出线程堆栈
jstack -F process_id
```

## 参考

- java性能权威指南
- 深入理解Java虚拟机-JVM高级特性与最佳实践
- [Linux vmstat命令详解](https://www.jellythink.com/archives/419)
- [Linux性能监测：CPU篇](https://www.jellythink.com/archives/432)
- [Linux top命令详解](https://www.jellythink.com/archives/421)
- [Linux系统内存监控、性能诊断工具vmstat命令详解](http://zhangge.net/4936.html)
- [抓取占用CPU高的JAVA线程，进而找出有问题的WEB页面](https://zhangge.net/131.html)
- [linux CPU占用飙升的排查方法（一）](https://www.jianshu.com/p/778ffa2ab881)
