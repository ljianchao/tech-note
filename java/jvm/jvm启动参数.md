# jvm启动参数

## 开启jmx

### 不启用认证

命令：

```
java \
 -Dcom.sun.management.jmxremote.authenticate=false \
 -Dcom.sun.management.jmxremote.port=5555 \
 -Dcom.sun.management.jmxremote.ssl=false \
 -Djava.rmi.server.hostname=test.domain.dounine.com \
 -jar test-jar-1.0.jar
```

###　启用用户认证

命令

```
java \
 -Dcom.sun.management.jmxremote.authenticate=true \
 -Dcom.sun.management.jmxremote.port=5555 \
 -Dcom.sun.management.jmxremote.access.file=./jmxremote.access \
 -Dcom.sun.management.jmxremote.password.file=./jmxremote.password \
 -Dcom.sun.management.jmxremote.ssl=false \
 -Djava.rmi.server.hostname=test.domain.dounine.com \
 -jar test-jar-1.0.jar
```

编辑`./jmxremote.access`权限文件，内容：

```
dounine readwrite
```

编辑`./jmxremote.password`密码文件，内容：

```
dounine 你的密码
```

增加访问权限

```
chmod 400 ./jmxremote.access
chmod 400 ./jmxremote.password
```



## 开始远程调试

jdk1.7之前的命令：

```
    java -agentlib:jdwp=transport=dt_socket,address=8000,server=y,suspend=y -jar xxx.jar
```

jdk1.7之后的命令：

```
    java -Xdebug -Xrunjdwp:transport=dt_socket,server=y,suspend=n,address=8000 -jar test.jar
```

## 内存设置

### 设置堆内存

- `-Xms` 参数设置初始化堆内存 `InitialHeapSize` 的值，`-Xmx` 参数设置最大堆内存 `MaxHeapSize` 的值。默认使用 `-XX:+UseParallelGC` 参数设置垃圾回收器的组合为 `Parallel Scavenge + Parallel Old`。
- `-Xmn` 参数用于设置新生代的大小。设置一个较大的新生代会减少老年代的大小，这个参数对系统性能以及 GC 行为有很大的影响。新生代的大小一般设置为整个堆空间的 1/4 到 1/3 左右。设置 `-Xmn` 的效果等同于设置了相同的 `-XX:NewSize` （新生代的初始大小）和 `-XX:MaxNewSize` （新生代的最大值）。
- `-XX:MinHeapFreeRatio`：设置堆空间的最小空闲比例。当堆空间的空闲内存小于这个数值时，JVM 便会扩展堆空间。
- `-XX:MaxHeapFreeRatio`：设置堆空间的最大空闲比例。当堆空间的空闲内存大于这个数值时，便会压缩堆空间，得到一个较小的堆。
- `-XX:NewSize`：设置新生代的大小。
- `-XX:NewRatio`：设置老年代与新生代的比例，它等于老年代大小除以新生代大小。
- `-XX:SurvivorRatio`：新生代中 eden 区与 survior 区的比例。
- `-XX:TargetSurvivorRatio`：设置 survior 区的可使用率。当 survior 区的空间使用率达到这个数值时，会将对象送入老年代。
- `-XX:PretenureSizeThreshold`：指定大于该设置值的对象直接在老年代分配（默认值为 0），避免在 Eden 区及两个 Survior 区之间来回复制，产生大量的内存复制操作。该参数只对 `Serial` 和 `ParNew` 两款新生代收集器有效。
- `-XX:MaxTenuringThreshold`：指定对象晋升老年代的年龄阈值（默认值为 15）。

启动命令：

```shell
   java -Xms1024m -Xmx1024m -jar spring-boot-demo-0.0.1-SNAPSHOT.jar
```

使用命令 `jcmd <pid> VM.flags` 查看设置的 flags 情况：

```
217430:
-XX:CICompilerCount=4 -XX:InitialHeapSize=1073741824 -XX:MaxHeapSize=1073741824 -XX:MaxNewSize=357564416 -XX:MinHeapDeltaBytes=524288 -XX:NewSize=357564416 -XX:OldSize=716177408 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:+UseParallelGC
```

使用命令 `jmap -heap <pid>` 查看堆内存情况：

```
Attaching to process ID 217430, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.131-b11

using thread-local object allocation.
Parallel GC with 8 thread(s)

Heap Configuration:
   MinHeapFreeRatio         = 0
   MaxHeapFreeRatio         = 100
   MaxHeapSize              = 1073741824 (1024.0MB)
   NewSize                  = 357564416 (341.0MB)
   MaxNewSize               = 357564416 (341.0MB)
   OldSize                  = 716177408 (683.0MB)
   NewRatio                 = 2
   SurvivorRatio            = 8
   MetaspaceSize            = 21807104 (20.796875MB)
   CompressedClassSpaceSize = 1073741824 (1024.0MB)
   MaxMetaspaceSize         = 17592186044415 MB
   G1HeapRegionSize         = 0 (0.0MB)

Heap Usage:
PS Young Generation
Eden Space:
   capacity = 268435456 (256.0MB)
   used     = 165211456 (157.55792236328125MB)
   free     = 103224000 (98.44207763671875MB)
   61.54606342315674% used
From Space:
   capacity = 44564480 (42.5MB)
   used     = 0 (0.0MB)
   free     = 44564480 (42.5MB)
   0.0% used
To Space:
   capacity = 44564480 (42.5MB)
   used     = 0 (0.0MB)
   free     = 44564480 (42.5MB)
   0.0% used
PS Old Generation
   capacity = 716177408 (683.0MB)
   used     = 5798864 (5.5302276611328125MB)
   free     = 710378544 (677.4697723388672MB)
   0.809696582889138% used

10932 interned Strings occupying 928984 bytes.
```

### 设置栈大小

参数 `-Xss` 设置栈容量 `-XX:ThreadStackSize` 的值，JDK 8 默认值为 1024（K）。

启动命令：

```shell
   java -Xms20M -Xmx20M -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=92 -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -Xss512k -jar spring-boot-demo-0.0.1-SNAPSHOT.jar
```

使用命令 `jcmd <pid> VM.flags` 查看设置的 flags 情况：

```
55824:
-XX:CICompilerCount=4 -XX:CMSInitiatingOccupancyFraction=92 -XX:+HeapDumpOnOutOfMemoryError -XX:InitialHeapSize=20971520 -XX:MaxHeapSize=20971520 -XX:MaxNewSize=6946816 -XX:MaxTenuringThreshold=6 -XX:MinHeapDeltaBytes=196608 -XX:NewSize=6946816 -XX:OldPLABSize=16 -XX:OldSize=14024704 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:ThreadStackSize=512 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseConcMarkSweepGC -XX:+UseFastUnorderedTimeStamps -XX:+UseParNewGC 
```

### 元空间设置

- `-XX:MaxMetaspaceSize`：设置元空间最大值，默认值是 -1，即不限制，或者说只受限于本地内存大小。
- `-XX:MetaspaceSize`：指定元空间的初始大小，以**字节**为单位，达到该值就会触发垃圾收集进行类型卸载，同时收集器会对该值进行调整：如果释放了大量的空间，就适当降低该值；如果释放了很少的空间，那么在不超过 `-XX:MaxMetaspaceSize`（如果设置了的话）的情况下，适当提高该值。
- `-XX:MinMetaspaceFreeRatio`：作用是在垃圾收集之后控制最小的元空间剩余容量的比例，可减少因为元空间不足导致的垃圾收集频率。类似的还有 `-XX:MaxMetaspaceFreeRatio`，用于控制最大的元空间剩余容量的百分比。

### 本机直接内存设置

`-XX:MaxDirectMemorySize` 参数设置直接内存（Direct Memory）的容量大小，如果不去指定，则默认与 Java 堆的最大值一致。

## 垃圾收集器设置

### 常用垃圾回收器组合参数设定

- `-XX:+UseSerialGC`: Serial New(DefNew) + SerialOld。
- `-XX:+UseParNewGC`：ParNew + Serial Old（该参数在 JDK 9 后不再支持）
- `-XX:+UseConcMarkSweepGC`: ParNew + CMS + Serial Old（后备预案）
- `-XX:+UseParallelGC`（默认）: Parallel Scavenge + Parallel Old(1.7 & 1.8默认)
- `-XX:+UseParallelOldGC`: Parallel Scavenge + Pallel Old（该参数在 JDK 5 之后已无用）
- `-XX:+UseG1GC`: G1

### 查看 JVM 默认垃圾回收器方法

使用以下命令查看：

```
    jcmd <pid> VM.flags
```

输出内容如下：

```
<pid>:
-XX:CICompilerCount=4 -XX:InitialHeapSize=1073741824 -XX:MaxHeapSize=1073741824 -XX:MaxNewSize=357564416 -XX:MinHeapDeltaBytes=524288 -XX:NewSize=357564416 -XX:OldSize=716177408 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:+UseParallelGC
```

### 查看某个 flag 的值

使用如下命令查看：

```
    jcmd <pid> VM.flags -all | grep <flag>
```

### 设置 GC 日志

使用如下命令设置：

```
java -Xms1024m -Xmx1024m -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=92 -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -jar spring-boot-demo-0.0.1-SNAPSHOT.jar
```

- `-XX:+HeapDumpOnOutOfMemoryError`：让虚拟机在出现内存溢出异常的时候 Dump 出当前的内存堆转储快照以便进行事后分析。
- `-XX:HeapDumpPath=logs/heapDump.hprof`：指定 dump 文件的路径，目录路径必须存在。默认值为 `./java_pid%p.hprof`，建议不设置，手动设置 `%p` 变量不生效。
- `-Xloggc:logs/gc-%t.log`：指定 GC 日志的输入路径。
- `-XX:+PrintGCDateStamps`：输出 GC 的触发时间。
- `-XX:+PrintGC` 和 `-verbose:gc`：打印 GC 基本信息，会显示容量的变化。
- `-XX:+PrintGCDetails`：打印 GC 详细信息。虚拟机在发生垃圾收集行为时打印内存回收日志，并且在进程退出的时候输出当前的内存各区域分配情况。GC 日志格式：`[GC 名称][GC前内存占用] -> [GC后内存占用](该区内存总大小)`。
- `-XX:+PrintHeapAtGC`：打印 GC 前后的堆、方法区可用容量的变化。
- `-XX:+PrintGCApplicationStoppedTime`：查看 GC 过程中用户线程并发时间以及停顿时间。

### SericalGC

使用参数 `-XX:+UseSerialGC` 开启 Serial 收集器 和 Serial Old 收集器组合。

启动命令：

```
   java -Xms1024m -Xmx1024m -XX:+UseSerialGC -jar spring-boot-demo-0.0.1-SNAPSHOT.jar
```

使用命令 `jcmd <pid> VM.flags` 查看设置的 flags 情况：

```
216506:
-XX:CICompilerCount=4 -XX:InitialHeapSize=1073741824 -XX:MaxHeapSize=1073741824 -XX:MaxNewSize=357892096 -XX:MinHeapDeltaBytes=196608 -XX:NewSize=357892096 -XX:OldSize=715849728 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:+UseSerialGC
```

使用命令 `jmap -heap <pid>` 查看堆内存情况：

```
Attaching to process ID 216506, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.131-b11

using thread-local object allocation.
Mark Sweep Compact GC

Heap Configuration:
   MinHeapFreeRatio         = 40
   MaxHeapFreeRatio         = 70
   MaxHeapSize              = 1073741824 (1024.0MB)
   NewSize                  = 357892096 (341.3125MB)
   MaxNewSize               = 357892096 (341.3125MB)
   OldSize                  = 715849728 (682.6875MB)
   NewRatio                 = 2
   SurvivorRatio            = 8
   MetaspaceSize            = 21807104 (20.796875MB)
   CompressedClassSpaceSize = 1073741824 (1024.0MB)
   MaxMetaspaceSize         = 17592186044415 MB
   G1HeapRegionSize         = 0 (0.0MB)

Heap Usage:
New Generation (Eden + 1 Survivor Space):
   capacity = 322109440 (307.1875MB)
   used     = 166651480 (158.93123626708984MB)
   free     = 155457960 (148.25626373291016MB)
   51.73753367799466% used
Eden Space:
   capacity = 286326784 (273.0625MB)
   used     = 166651480 (158.93123626708984MB)
   free     = 119675304 (114.13126373291016MB)
   58.203245142445354% used
From Space:
   capacity = 35782656 (34.125MB)
   used     = 0 (0.0MB)
   free     = 35782656 (34.125MB)
   0.0% used
To Space:
   capacity = 35782656 (34.125MB)
   used     = 0 (0.0MB)
   free     = 35782656 (34.125MB)
   0.0% used
tenured generation:
   capacity = 715849728 (682.6875MB)
   used     = 5467960 (5.214653015136719MB)
   free     = 710381768 (677.4728469848633MB)
   0.7638418771600064% used

10961 interned Strings occupying 930880 bytes.
```

### ParallelGC

新生代 `NewSize` 和 老年代 `OldSize` 的默认大小比例为 `1:2`。

使用参数 `-XX:+UseParallelGC` 开启 Parallel Scavenge 收集器和 Parallel Old 收集器组合，JDK 1.7 （包括）之后默认采用Parallel Scavenge 收集器作为新生代收集器。不能和 CMS 收集器配合使用。

Parallel Scavenge 收集器提供了两个参数用于精确控制吞吐量：

- `-XX:MaxGCPauseMillis`：控制最大收集停顿时间。参数允许的值是一个大于 0 的毫秒数，收集器将尽力保证内存回收花费的时间不超过用户设定值。不过大家不要异想天开地认为如果把这个参数的值设置得更小一点就能使得系统的垃圾收集速度变得更快，**垃圾收集停顿时间缩短是以牺牲吞吐量和新生代空间为代价换取的**：系统把新生代调得小一些，收集 300MB 新生代肯定比收集 500M B快，但这也直接导致垃圾收集发生得**更频繁**，原来 10 秒收集一次、每次停顿 100 毫秒，现在变成 5 秒收集一次、每次停顿 70 毫秒。停顿时间的确在下降，但吞吐量也降下来了。
- `-XX:GCTimeRatio`：直接设置吞吐量大小。参数的值则应当是一个大于 0 小于 100 的整数，也就是垃圾收集时间占总时间的比率，相当于**吞吐量的倒数**。譬如把此参数设置为 19 ，那允许的最大垃圾收集时间就占总时间的 5%（即1/(1+19)），默认值为 99，即允许最大 1%（即1/(1+99)）的垃圾收集时间。

使用参数 `-XX:+UseAdaptiveSizePolicy` 开启 Parallel Scavenge 收集器的自适应调节策略（默认是开启的）。

启动命令：

```shell
    java -Xms1024m -Xmx1024m -XX:+UseParallelGC -jar spring-boot-demo-0.0.1-SNAPSHOT.jar
```

使用命令 `jcmd <pid> VM.flags` 查看设置的 flags 情况：

```
217430:
-XX:CICompilerCount=4 -XX:InitialHeapSize=1073741824 -XX:MaxHeapSize=1073741824 -XX:MaxNewSize=357564416 -XX:MinHeapDeltaBytes=524288 -XX:NewSize=357564416 -XX:OldSize=716177408 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:+UseParallelGC
```

使用命令 `jmap -heap <pid>` 查看堆内存情况：

```
Attaching to process ID 217430, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.131-b11

using thread-local object allocation.
Parallel GC with 8 thread(s)

Heap Configuration:
   MinHeapFreeRatio         = 0
   MaxHeapFreeRatio         = 100
   MaxHeapSize              = 1073741824 (1024.0MB)
   NewSize                  = 357564416 (341.0MB)
   MaxNewSize               = 357564416 (341.0MB)
   OldSize                  = 716177408 (683.0MB)
   NewRatio                 = 2
   SurvivorRatio            = 8
   MetaspaceSize            = 21807104 (20.796875MB)
   CompressedClassSpaceSize = 1073741824 (1024.0MB)
   MaxMetaspaceSize         = 17592186044415 MB
   G1HeapRegionSize         = 0 (0.0MB)

Heap Usage:
PS Young Generation
Eden Space:
   capacity = 268435456 (256.0MB)
   used     = 165211456 (157.55792236328125MB)
   free     = 103224000 (98.44207763671875MB)
   61.54606342315674% used
From Space:
   capacity = 44564480 (42.5MB)
   used     = 0 (0.0MB)
   free     = 44564480 (42.5MB)
   0.0% used
To Space:
   capacity = 44564480 (42.5MB)
   used     = 0 (0.0MB)
   free     = 44564480 (42.5MB)
   0.0% used
PS Old Generation
   capacity = 716177408 (683.0MB)
   used     = 5798864 (5.5302276611328125MB)
   free     = 710378544 (677.4697723388672MB)
   0.809696582889138% used

10932 interned Strings occupying 928984 bytes.
```

### ConcurrentMarkSweepGC

使用参数 `-XX:+UseConcMarkSweepGC` （默认会增加 `-XX:+UseParNewGC` 参数）开启 ParNew 收集器和 CMS 收集器组合。

参数 `-XX:CMSInitiatingOccupancyFraction` 的值控制在老年代达到指定的比例后触发CMS 收集器进行垃圾回收（JDK 8 默认值为 -1，是否占比 92% ?）。

开关参数 `-XX:+UseCMSCompactAtFullCollection` （默认是开启的，此参数从 JDK 9 开始废弃）用于在 CMS 收集器不得不进行 `Full GC` 时开启内存碎片的合并整理过程。

参数 `-XX:CMSFullGCsBeforeCompaction` （此参数从 JDK 9 开始废弃）的作用是要求 CMS 收集器在执行过若干次（数值由参数决定）不整理空间的 `Full GC` 之后，下一次进入 `Full GC` 前会先进行碎片整理（默认值为 0， 表示每次进入 Full GC 时都进行碎片整理）。

启动命令：

```shell
    java -Xms1024m -Xmx1024m -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=80 -jar spring-boot-demo-0.0.1-SNAPSHOT.jar
```

使用命令 `jcmd <pid> VM.flags` 查看设置的 flags 情况：

```
217687:
-XX:CICompilerCount=4 -XX:InitialHeapSize=1073741824 -XX:MaxHeapSize=1073741824 -XX:MaxNewSize=357892096 -XX:MaxTenuringThreshold=6 -XX:MinHeapDeltaBytes=196608 -XX:NewSize=357892096 -XX:OldPLABSize=16 -XX:OldSize=715849728 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseConcMarkSweepGC -XX:+UseFastUnorderedTimeStamps -XX:+UseParNewGC
```

使用命令 `jmap -heap <pid>` 查看堆内存情况：

```
Attaching to process ID 217687, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.131-b11

using parallel threads in the new generation.
using thread-local object allocation.
Concurrent Mark-Sweep GC

Heap Configuration:
   MinHeapFreeRatio         = 40
   MaxHeapFreeRatio         = 70
   MaxHeapSize              = 1073741824 (1024.0MB)
   NewSize                  = 357892096 (341.3125MB)
   MaxNewSize               = 357892096 (341.3125MB)
   OldSize                  = 715849728 (682.6875MB)
   NewRatio                 = 2
   SurvivorRatio            = 8
   MetaspaceSize            = 21807104 (20.796875MB)
   CompressedClassSpaceSize = 1073741824 (1024.0MB)
   MaxMetaspaceSize         = 17592186044415 MB
   G1HeapRegionSize         = 0 (0.0MB)

Heap Usage:
New Generation (Eden + 1 Survivor Space):
   capacity = 322109440 (307.1875MB)
   used     = 189306176 (180.53643798828125MB)
   free     = 132803264 (126.65106201171875MB)
   58.770763129450664% used
Eden Space:
   capacity = 286326784 (273.0625MB)
   used     = 180949672 (172.56705474853516MB)
   free     = 105377112 (100.49544525146484MB)
   63.196907209351394% used
From Space:
   capacity = 35782656 (34.125MB)
   used     = 8356504 (7.969383239746094MB)
   free     = 27426152 (26.155616760253906MB)
   23.353503999255953% used
To Space:
   capacity = 35782656 (34.125MB)
   used     = 0 (0.0MB)
   free     = 35782656 (34.125MB)
   0.0% used
concurrent mark-sweep generation:
   capacity = 715849728 (682.6875MB)
   used     = 0 (0.0MB)
   free     = 715849728 (682.6875MB)
   0.0% used

10988 interned Strings occupying 932928 bytes.
```

### G1GC

- `-XX:G1HeapRegionSize`：设置每个 Region 的大小，默认是 1MB，取值范围为 1MB~32MB，且应为 2 的 N 次幂。
- `-XX:MaxGCPauseMillis`：设定运行的收集停顿时间，默认值为 200 毫秒。

启动命令：

```shell
    java -Xms1024m -Xmx1024m -XX:+UseG1GC -jar spring-boot-demo-0.0.1-SNAPSHOT.jar
```

使用命令 `jcmd <pid> VM.flags` 查看设置的 flags 情况：

```
217942:
-XX:CICompilerCount=4 -XX:ConcGCThreads=2 -XX:G1HeapRegionSize=1048576 -XX:InitialHeapSize=1073741824 -XX:MarkStackSize=4194304 -XX:MaxHeapSize=1073741824 -XX:MaxNewSize=643825664 -XX:MinHeapDeltaBytes=1048576 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseFastUnorderedTimeStamps -XX:+UseG1GC 
```

使用命令 `jmap -heap <pid>` 查看堆内存情况：

```
Attaching to process ID 217942, please wait...
Debugger attached successfully.
Server compiler detected.
JVM version is 25.131-b11

using thread-local object allocation.
Garbage-First (G1) GC with 8 thread(s)

Heap Configuration:
   MinHeapFreeRatio         = 40
   MaxHeapFreeRatio         = 70
   MaxHeapSize              = 1073741824 (1024.0MB)
   NewSize                  = 1363144 (1.2999954223632812MB)
   MaxNewSize               = 643825664 (614.0MB)
   OldSize                  = 5452592 (5.1999969482421875MB)
   NewRatio                 = 2
   SurvivorRatio            = 8
   MetaspaceSize            = 21807104 (20.796875MB)
   CompressedClassSpaceSize = 1073741824 (1024.0MB)
   MaxMetaspaceSize         = 17592186044415 MB
   G1HeapRegionSize         = 1048576 (1.0MB)

Heap Usage:
G1 Heap:
   regions  = 1024
   capacity = 1073741824 (1024.0MB)
   used     = 159383536 (151.99998474121094MB)
   free     = 914358288 (872.0000152587891MB)
   14.84374850988388% used
G1 Young Generation:
Eden Space:
   regions  = 140
   capacity = 506462208 (483.0MB)
   used     = 146800640 (140.0MB)
   free     = 359661568 (343.0MB)
   28.985507246376812% used
Survivor Space:
   regions  = 12
   capacity = 12582912 (12.0MB)
   used     = 12582912 (12.0MB)
   free     = 0 (0.0MB)
   100.0% used
G1 Old Generation:
   regions  = 0
   capacity = 554696704 (529.0MB)
   used     = 0 (0.0MB)
   free     = 554696704 (529.0MB)
   0.0% used

10921 interned Strings occupying 925744 bytes.
```

## 参考

- [Java HotSpot VM Options](https://www.oracle.com/java/technologies/javase/vmoptions-jsp.html)
- [Java Platform, Standard Edition Tools Reference](https://docs.oracle.com/javase/8/docs/technotes/tools/windows/java.html)
- [JVM参数使用手册](https://segmentfault.com/a/1190000010603813)
- [Java - jmx远程调优](https://www.jianshu.com/p/923580d3a5a2)
- [JVM的server模式和client模式](https://blog.csdn.net/qq_26545305/article/details/70241939)
- [JVM调优实战一（Parallel + ParallelOld）](https://blog.csdn.net/bch1991/article/details/109458333)
- [为什么不推荐启用UseGCLogFileRotation来记录GC日志？](https://blog.csdn.net/goldenfish1919/article/details/93997132)
- [Java如何生成Heap Dump及OOM问题排查](https://www.jianshu.com/p/91ee6476848c)
- [Heap堆分析（堆转储、堆分析）](https://www.cnblogs.com/duanxz/p/8510623.html)
- [GC调优-XX:PrintGCDetails深度解析](https://www.cnblogs.com/A-Dun/p/13790719.html)
