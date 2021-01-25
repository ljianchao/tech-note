# jvm基础故障处理工具

JDK自带下列工具：

- jps（JVM Process Status Tool），显示指定系统内所有的HotSpot虚拟机进程
- jcmd，打印Java进程所涉及的基本类、线程和VM信息。
- jinfo（Configuration Info for Java），查看JVM的系统属性，可以动态设置一些系统属性。可适用于脚本。

- jstat（JVM Statistics Monitoring Tool），提供GC和类装载活动的信息。可适用于脚本。
- jhat（JVM Heap Analysis Tool），读取内存堆转储，并有助于分析。这是事后使用的工具。
- jmap（Memory Map for Java），提供堆转储和其他JVM内存使用的信息。可以适用于脚本，但堆转储必须在事后分析工具中使用。
- jstack（Stack Trace for Java），转储JVM进程的栈信息。可适用于脚本。

- jconsole，提供JVM活动的图形化视图，包括线程的使用、类的使用和GC活动。
- jvisualvm，监视JVM的GUI工具，可用来剖析运行的应用，分析JVM堆转储（事后活动，虽然jvisualvm也可以实时抓取程序的堆转储）。

## 基本的VM信息

### jcmd 工具

jcmd 命令格式：

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

jinfo 命令格式：

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
// 查看可用的 command
jcmd <pid> help

// 运行时间
jcmd <pid> VM.uptime

// 系统属性
jcmd <pid> VM.system_properties

// JVM版本
jcmd <pid> VM.version

// 显示程序所用的命令行
jcmd <pid> VM.command_line

// JVM调优标识，结果中的中间列有冒号表示标志使用的是非默认值
// 某些标志的默认值在不同平台上可能会不相同，结果的最右列会指示。
// product表示在所有平台上的默认值都是一致的；pd product表示标志的默认值是独立于平台的；
// manageable表示运行时可以动态更改标志的值；
// C2 diagnostic为编译工程师提供诊断输出，帮助理解编译器正以什么方式运作
jcmd <pid> VM.flags [-all]
java [other_options] -XX:+PrintFlagsFinal -version

// 生成堆转储
jcmd <pid> GC.heap_dump logs/heap_dump.hprof
```

### jinfo 工具

jinfo在检查单个标志时很有用，同时允许程序在执行时更改某个标志的值：

```
// 系统属性和VM信息，-sysprops 和 -flags 参数的合集
jinfo <pid>

// 获取系统属性
jinfo -sysprops <pid>

// 获取进程中所有标志的值，包括 Non-default VM flags 类型的值和 Command line 类型的值
jinfo -flags <pid>

// 检查某个标志的值
jinfo -flag PrintGCDetails <pid>

// 开启或关闭某个标志，对大多数影响GC算法行为的标志更改不会导致JVM改变它的行为，
// 一般只对那些在Printflagsfinal输出中标记为manageable的标志有效
jinfo -flag +PrintGCDetails <pid>
jinfo -flag -PrintGCDetails <pid>
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

## 控制台监视工具

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
<lines>       Number of 
samples between header lines.
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
jstat -class <pid> 1s

// 间隔1秒查询一次垃圾收集状况，共查询10次
jstat -gc <pid> 1s 10
jstat -gc <pid> 1000ms 10

// 监视Java堆各个区域使用到的最大、最小空间
jstat -gccapacity <pid> 1s

// 监视Java堆各个区域使用空间占总空间的百分比
jstat -gcutil <pid> 1s

// 监视Java堆各个区域使用空间占总空间的百分比，同时输出导致上一次GC产生的原因
jstat -gccause <pid> 1s

// 监视新生代GC状况
jstat -gcnew <pid> 1s

// 监视新生代的最大、最小空间
jstat -gcnewcapacity <pid> 1s

// 监视老年代GC状况
jstat -gcold <pid> 1s

// 监视老年代的最大、最小空间
jstat -gcoldcapacity <pid> 1s

// 输出JIT编译器编译过的方法、耗时等信息
jstat -compiler <pid> 1s

// 输出已经被JIT编译器编译过的方法
jstat -printcompilation <pid> 1s
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
jmap -dump:format=b,file=dump.bin <pid>

// 当虚拟机进程对-dump选项没有响应时，可使用`-F`选项强制生成dump快照
jmap -F -dump:format=b,file=dump.bin <pid>

// 显示java堆详细信息，包括回收器、参数设置、分代状况等
jmap -heap <pid>

// 显示堆中对象统计信息，包括类、实例数量、合计容量
jmap -histo <pid>

// 统计ClassLoader信息
jmap -clstats <pid>

// 显示在F-Queue中等待Finalizer线程执行finalize方法的对象
jmap -finalizerinfo <pid>
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
jstack -l <pid>

// 如果调用到本地方法的话，可以显示C/C++的堆栈
jstack -m <pid>

// 当正常输出的请求不被响应时，强制输出线程堆栈
jstack -F <pid>
```

## 可视化监视工具

### jconsole

### jvisualvm 工具

## 参考

- [jcmd的使用](https://www.cnblogs.com/caibh/p/13787158.html)
