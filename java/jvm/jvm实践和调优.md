# jvm实践

## OutOfMemoryError异常

在《Java虚拟机规范》的规定里，除了程序计数器为，虚拟机内存的其他几个运行时区域都有发生 `OutOfMemoryError` （简称OOM）异常的可能。

### Java 堆溢出

Java 堆用于存储对象实例，我们只要不断地创建对象，并且保证 GC Roots 到对象之间有可达路径来避免垃圾回收机制清除这些对象，那么随着对象数量的增加，总容量触及最大堆的容量限制后就会产生内存溢出异常。

实验 java 代码如下：

```java
package com.example.jvm.demo.service;

import java.io.IOException;
import java.util.Vector;

/**
 * jvm 最大堆内存需要小于10M，将堆的最小值 `-Xms` 参数与最大值 `-Xmx` 参数设置为一样避免堆自动扩展；
 * `-XX:+HeapDumpOnOutOfMemoryError` 参数让虚拟机在出现内存溢出异常的时候 Dump 出当前的内存转储
 * 快照以便进行时候分析，默认的堆转储文件为 `./java_pid%p.hprof`
 *
 * 启动命令：
 *     java -Xms5M -Xmx5M -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=92 -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.service.HeapOOM
 *
 * 异常内容：
 * java.lang.OutOfMemoryError: Java heap space
 * Dumping heap to java_pid61446.hprof ...
 * Heap dump file created [6156134 bytes in 0.015 secs]
 * Exception in thread "Thread-0" java.lang.OutOfMemoryError: Java heap space
 * 	at com.example.jvm.demo.service.HeapOOM.testOOM(HeapOOM.java:28)
 * 	at com.example.jvm.demo.service.HeapOOM.run(HeapOOM.java:36)
 * 	at java.lang.Thread.run(Thread.java:748)
 */
public class HeapOOM implements Runnable {

    public void testOOM() {
        Vector vector = new Vector();
        for (int i = 0; i < 10; i++) {
            byte[] bytes = new byte[1024 * 1024];   // 每个循环分配 1MB 内存
            vector.add(bytes);  // 强引用，使 GC 时不能释放空间
            System.out.println(i + "MB is allocated.");
        }
    }

    @Override
    public void run() {
        testOOM();
    }

    public static void main(String[] args) {

        // 测试栈容量溢出
        new Thread(new HeapOOM()).start();

        System.out.println("HeapOOM is up.");
        try {
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

```

出现 Java 堆内存溢出时，异常堆栈信息“java.lang.OutOfMemoryError”会跟随进一步提示“Java heap space”。

堆转储快照分析：

- 首先确认内存中导致 OOM 的对象是否是必要的，也就是要先分清楚到底是出现了**内存泄漏（Memory Leak）** 还是**内存溢出（Memory Overflow）**。
- 如果是内存泄漏，通过工具查找泄漏对象到 GC Roots 的引用链，找到泄漏对象是通过怎样的引用路径、与哪些 GC Roots 相关联，才导致垃圾收集器无法回收它们，根据泄漏对象的类型信息以及它到 GC Roots 引用链的信息，一般可用比较准确地定位到这些对象创建的位置，进而找出产生内存泄漏的代码的具体位置。
- 如果不是内存泄漏，换句话说就是内存中的对象确实都是必须存活的，那就应当检查 Java 虚拟机的堆参数（-Xms 和 -Xmx）设置，与机器内存对比，看看是否还有向上调整的空间。再从代码上检查是否存在某些对象生命周期过程，持有状态时间过长、存储结构设计不合理等情况，尽量减少程序运行期的内存消耗。

### 虚拟机栈和本地方法栈溢出

由于 HotSpot 虚拟机中不区分虚拟机栈和本地方法栈，因此对于 HotSpot 来说，`-Xoss` 参数（设置本地方法栈大小）虽然存在，但实际上是没有任何效果的，栈容量只能由 `-Xss` 参数（对应 flag `-XX:ThreadStackSize`）来设定。

关于虚拟机栈和本地方法栈，在《Java虚拟机规范》中描述了两种异常：

- 如果线程请求的栈深度大于虚拟机运行的最大深度，将抛出 `StackOverflowError` 异常。
- 如果虚拟机的栈内存允许动态扩展，当扩展栈容量无法申请到足够的内存时，将抛出 `OutOfMemoryError` 异常。

HotSpot 虚拟机是不支持栈内存的动态扩展，所以除非在创建线程申请内存时就因无法获得足够内存而出现 `OutOfMemoryError` 异常，否则在线程运行时是不会因为扩展而导致内存溢出的，之后因为栈容量无法容纳新的栈帧而导致 `StackOverflowError` 异常。

使用 `-Xss` 参数减小栈内存容量，实验代码如下：

```java
package com.example.jvm.demo.service;

import java.io.IOException;

/**
 * 使用 -Xss 参数减小栈内存容量，JDK 8 默认值为 1024（K）
 *
 * 启动命令：
 *     java -Xms100M -Xmx100M -Xss228K -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=92 -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.service.JavaVMStackSOF
 *
 * 异常内容：
 * Exception in thread "Thread-0" java.lang.StackOverflowError
 * 	at com.example.jvm.demo.service.JavaVMStackSOF.stackLeak(JavaVMStackSOF.java:25)
 * 	at com.example.jvm.demo.service.JavaVMStackSOF.stackLeak(JavaVMStackSOF.java:25)
 * 	at com.example.jvm.demo.service.JavaVMStackSOF.stackLeak(JavaVMStackSOF.java:25)
 * 	at com.example.jvm.demo.service.JavaVMStackSOF.stackLeak(JavaVMStackSOF.java:25)
 * 	at com.example.jvm.demo.service.JavaVMStackSOF.stackLeak(JavaVMStackSOF.java:25)
 */
public class JavaVMStackSOF implements Runnable {

    private int stackLenth = 1;

    public void stackLeak() {
        stackLenth++;
        stackLeak();
    }

    @Override
    public void run() {
        this.stackLeak();
    }

    public static void main(String[] args) {

        // 测试栈容量溢出
		new Thread(new JavaVMStackSOF()).start();

        System.out.println("JavaVMStackSOF is up.");
        try {
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

定义大量本地变量，增大此方法增大此方法帧中本地变量表的长度，实验代码如下：

```java
package com.example.jvm.demo.service;

import java.io.IOException;

/**
 * 使用 -Xss 参数减小栈内存容量，JDK 8 默认值为 1024（K）
 * <p>
 * 启动命令：
 * java -Xms100M -Xmx100M -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=92 -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.service.JavaVMStackSOFWithManyVars
 * <p>
 * 异常内容：
 * Exception in thread "Thread-0" java.lang.StackOverflowError
 * 	at com.example.jvm.demo.service.JavaVMStackSOFWithManyVars.test(JavaVMStackSOFWithManyVars.java:47)
 * 	at com.example.jvm.demo.service.JavaVMStackSOFWithManyVars.test(JavaVMStackSOFWithManyVars.java:47)
 * 	at com.example.jvm.demo.service.JavaVMStackSOFWithManyVars.test(JavaVMStackSOFWithManyVars.java:47)
 * 	at com.example.jvm.demo.service.JavaVMStackSOFWithManyVars.test(JavaVMStackSOFWithManyVars.java:47)
 * 	at com.example.jvm.demo.service.JavaVMStackSOFWithManyVars.test(JavaVMStackSOFWithManyVars.java:47)
 */
public class JavaVMStackSOFWithManyVars implements Runnable {

    private int stackLenth = 1;

    public void test() {
        long unused1, unused2, unused3, unused4, unused5,
                unused6, unused7, unused8, unused9, unused10,
                unused11, unused12, unused13, unused14, unused15,
                unused16, unused17, unused18, unused19, unused20,
                unused21, unused22, unused23, unused24, unused25,
                unused26, unused27, unused28, unused29, unused30,
                unused31, unused32, unused33, unused34, unused35,
                unused36, unused37, unused38, unused39, unused40,
                unused41, unused42, unused43, unused44, unused45,
                unused46, unused47, unused48, unused49, unused50,
                unused51, unused52, unused53, unused54, unused55,
                unused56, unused57, unused58, unused59, unused60,
                unused61, unused62, unused63, unused64, unused65,
                unused66, unused67, unused68, unused69, unused70,
                unused71, unused72, unused73, unused74, unused75,
                unused76, unused77, unused78, unused79, unused80,
                unused81, unused82, unused83, unused84, unused85,
                unused86, unused87, unused88, unused89, unused90,
                unused91, unused92, unused93, unused94, unused95,
                unused96, unused97, unused98, unused99, unused100;

        stackLenth++;

        test();

        unused1 = unused2 = unused3 = unused4 = unused5 =
        unused6 = unused7 = unused8 = unused9 = unused10 =
        unused11 = unused12 = unused13 = unused14 = unused15 =
        unused16 = unused17 = unused18 = unused19 = unused20 =
        unused21 = unused22 = unused23 = unused24 = unused25 =
        unused26 = unused27 = unused28 = unused29 = unused30 =
        unused31 = unused32 = unused33 = unused34 = unused35 =
        unused36 = unused37 = unused38 = unused39 = unused40 =
        unused41 = unused42 = unused43 = unused44 = unused45 =
        unused46 = unused47 = unused48 = unused49 = unused50 =
        unused51 = unused52 = unused53 = unused54 = unused55 =
        unused56 = unused57 = unused58 = unused59 = unused60 =
        unused61 = unused62 = unused63 = unused64 = unused65 =
        unused66 = unused67 = unused68 = unused69 = unused70 =
        unused71 = unused72 = unused73 = unused74 = unused75 =
        unused76 = unused77 = unused78 = unused79 = unused80 =
        unused81 = unused82 = unused83 = unused84 = unused85 =
        unused86 = unused87 = unused88 = unused89 = unused90 =
        unused91 = unused92 = unused93 = unused94 = unused95 =
        unused96 = unused97 = unused98 = unused99 = unused100 = 0;
    }

    @Override
    public void run() {
        try {
            test();
        } catch (Error e) {
            System.out.println("stack length: " + stackLenth);
            throw e;
        }
    }

    public static void main(String[] args) {
        // 测试栈容量溢出
        new Thread(new JavaVMStackSOFWithManyVars()).start();

        System.out.println("JavaVMStackSOFWithManyVars is up.");
        try {
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

```

实验结果表明：无论是由于栈帧太大还是虚拟机栈容量太小，当新的栈帧内存无法分配的时候，HotSpot 虚拟机抛出的都是 `StackOverflowError` 异常。

**操作系统分配给每个进程的内存是有限制的**，譬如 32 位 Windows 的单个进程最大内存限制为 2GB。HotSpot虚拟机提供了参数可以控制**Java 堆和方法区**这两部分的内存的最大值，那剩余的内存即 2GB（操作系统限制）减去最大堆容量，再减去最大方法区容量，由于程序计数器消息内存很小，可以忽略掉，如果把直接内存和虚拟机进程本身耗费的内存也去掉的话，剩下的内存就由虚拟机栈和本地方法栈来分配了。因此为每个进程分配的栈内存越大，可以建立的线程数量自然就越少，建立线程时就越容易把剩下的内存耗尽。

创建线程导致内存溢出异常（OOM），需在 32 为系统下运行。由于在 Windows 平台的虚拟机中，Java 的线程是映射到操作系统的内核线程上，无限制地创建线程会对操作系统带来很大压力，以下代码执行时有很高的风险，可能会由于创建线程数量过多而导致操作系统假死。实验代码：

```java
package com.example.jvm.demo.service;

import java.io.IOException;

/**
 * 使用 -Xss 参数调大栈内存容量，JDK 8 默认值为 1024（K），请在 Windows 32 位系统下运行
 *
 * 启动命令：
 *     java -Xms100M -Xmx100M -Xss2M -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=92 -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.service.JavaVMStackSOF
 *
 * 异常内容：
 * Exception in thread "main" java.lang.OutOfMemoryError: unable to create native thread，possibly out of memory or process/resource limits reached
 */
public class JavaVMStackOOM {

    private void loop() {
        while (true) {

        }
    }

    public void stackLeakByThread() {
        while (true) {
            Thread thread = new Thread(() -> {
                loop();
            });
            thread.start();
        }
    }

    public static void main(String[] args) {
        JavaVMStackOOM oom = new JavaVMStackOOM();
        oom.stackLeakByThread();
        try {
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

如果使用 HotSpot 虚拟机默认参数，栈深度在大多数情况下（因为每个方法压入栈的帧大小并不是一样的，所以只能说大多数情况下）到达 1000~2000 是完全没有问题的，对于正常的方法调用（包括不能做尾递归优化的递归调用），这个深度应该完全够用了。但是，如果是**建立过多线程导致的内存溢出**，在不能减少线程数量或者更换 64 位虚拟机的情况下，就只能通过**减少最大堆和减少栈容量**来换取更多的线程。

### 方法区和运行时常量池溢出

运行时常量池是方法区的一部分。HotSpot 从 JDK 7 开始逐步“去永久代”的计划，并在 JDK 8 中完全使用元空间代替永久代。

`String::intern()` 是一个本地方法，它的作用是如果**字符串常量池**中包含一个等于此 String 对象的字符串，则返回代表池中这个字符串的 String 对象的引用；否则，会将此 String 对象包含的字符串添加到常量池中，并返回此 String 对象的引用。在 JDK 6 或更早之前的 HotSpot 虚拟机中，常量池都是分配在永久代中。自 JDK 7 起，原本存放在永久代的**字符串常量池**被移至 Java 堆中。

### 本机直接内存溢出

`-XX:MaxDirectMemorySize` 参数设置直接内存（Direct Memory）的容量大小，如果不去指定，则默认与 Java 堆的最大值一致。实验代码：

```java
package com.example.jvm.demo.service;

import sun.misc.Unsafe;

import java.lang.reflect.Field;

/**
 * `-XX:MaxDirectMemorySize` 参数设置直接内存（Direct Memory）的容量大小
 *
 * 启动命令：
 *     java -Xms20M -Xmx20M -XX:MaxDirectMemorySize=10M -XX:+UseConcMarkSweepGC -XX:CMSInitiatingOccupancyFraction=92 -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.service.DirectMemoryOOM
 *
 * 异常内容：
 * Exception in thread "main" java.lang.OutOfMemoryError
 * 	at sun.misc.Unsafe.allocateMemory(Native Method)
 * 	at com.example.jvm.demo.service.DirectMemoryOOM.main(DirectMemoryOOM.java:28)
 */
public class DirectMemoryOOM {

    private static final int _1MB = 1024 * 1024;

    public static void main(String[] args) throws IllegalAccessException {

        Field unsafeField = Unsafe.class.getDeclaredFields()[0];
        unsafeField.setAccessible(true);
        Unsafe unsafe = (Unsafe) unsafeField.get(null);
        while (true) {
            unsafe.allocateMemory(_1MB);
        }
    }
}

```

## 调优

### 1. 将新对象预留在新生代

大多数情况下，对象在新生代 Eden 区中分配，当 Eden 区没有足够空间进行分配时，虚拟机将发起一次 `Minor GC`。由于 Full GC 的成本要远远高于 Minor GC，因此尽可能将对象分配在新生代是一项明智的做法。

> 由于新生代垃圾回收的**速度**高于老年代回收。因此，将年轻对象预留在新生代有利于提高整体的 GC 效率。


和新生代设置相关的参数如下：

- `-Xmn` 参数用于设置新生代的大小。设置一个较大的新生代会减少老年代的大小，这个参数对系统性能以及 GC 行为有很大的影响。新生代的大小一般设置为整个堆空间的 1/4 到 1/3 左右。设置 `-Xmn` 的效果等同于设置了相同的 `-XX:NewSize` （新生代的初始大小）和 `-XX:MaxNewSize` （新生代的最大值）。
- `-XX:NewSize`：设置新生代的大小。
- `-XX:NewRatio`：设置老年代与新生代的比例，它等于老年代大小除以新生代大小。
- `-XX:SurvivorRatio`：新生代中 eden 区与 survior 区的比例。
- `-XX:TargetSurvivorRatio`：设置 survior 区的可使用率（默认值为 50，表示 50%）。当 survior 区的空间使用率达到这个数值时，会将对象送入老年代。


在 JVM 参数调优中，可以为应用程序分配一个合理的新生代空间，以最大效度避免新对象直接进入老年代的情况。`-Xmn` 、`-XX:NewSize` 和 `-XX:NewRatio` 都可以指定新生代大小。

通过设置一个较大的新生代预留新对象，设置合理的 `survior` 区并且提高 `survior` 区的使用率，可以将年轻对象保存在新生代。一般来说，当 `survior` 区的空间不够，或者占用量到达 `-XX:TargetSurvivorRatio` （默认值为 50，表示 50%）时，就会将对象送入老年代（不管对象的年龄有多大）。通过 `-XX:SurvivorRatio` 可以设置一个更大的 `survior` 区。

示例一：

```java
package com.example.jvm.demo.performance;

import java.io.IOException;

/**
 * 将新对象预留在新生代
 *
 * 示例一：
 *
 * 启动命令：
 *     java -Xms60M -Xmx60M -Xmn6M -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.PutInEden
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6461484k free), swap 7929852k(1201492k free)
 * CommandLine flags: -XX:InitialHeapSize=62914560 -XX:MaxHeapSize=62914560 -XX:MaxNewSize=6291456 -XX:NewSize=6291456 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * 2021-01-30T14:08:54.841+0800: 0.111: [GC (Allocation Failure) 2021-01-30T14:08:54.841+0800: 0.111: [DefNew: 4731K->303K(5568K), 0.0028289 secs] 4731K->4399K(60864K), 0.0029275 secs] [Times: user=0.00 sys=0.00, real=0.01 secs]
 * 2021-01-30T14:08:54.844+0800: 0.115: [GC (Allocation Failure) 2021-01-30T14:08:54.844+0800: 0.115: [DefNew: 4399K->0K(5568K), 0.0022167 secs] 8495K->8490K(60864K), 0.0022657 secs] [Times: user=0.00 sys=0.01, real=0.00 secs]
 * 2021-01-30T14:08:54.847+0800: 0.117: [GC (Allocation Failure) 2021-01-30T14:08:54.847+0800: 0.117: [DefNew: 4096K->0K(5568K), 0.0016308 secs] 12586K->12586K(60864K), 0.0016651 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 *
 * Heap
 *  def new generation   total 5568K, used 4243K [0x00000000fc400000, 0x00000000fca00000, 0x00000000fca00000)
 *   eden space 4992K,  85% used [0x00000000fc400000, 0x00000000fc824ff0, 0x00000000fc8e0000)
 *   from space 576K,   0% used [0x00000000fc970000, 0x00000000fc970000, 0x00000000fca00000)
 *   to   space 576K,   0% used [0x00000000fc8e0000, 0x00000000fc8e0000, 0x00000000fc970000)
 *  tenured generation   total 55296K, used 12586K [0x00000000fca00000, 0x0000000100000000, 0x0000000100000000)
 *    the space 55296K,  22% used [0x00000000fca00000, 0x00000000fd64a8b8, 0x00000000fd64aa00, 0x0000000100000000)
 *  Metaspace       used 2923K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 123406, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 62914560 (60.0MB)
 *    NewSize                  = 6291456 (6.0MB)
 *    MaxNewSize               = 6291456 (6.0MB)
 *    OldSize                  = 56623104 (54.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 5701632 (5.4375MB)
 *    used     = 4294352 (4.0954132080078125MB)
 *    free     = 1407280 (1.3420867919921875MB)
 *    75.31794405531609% used
 * Eden Space:
 *    capacity = 5111808 (4.875MB)
 *    used     = 4294352 (4.0954132080078125MB)
 *    free     = 817456 (0.7795867919921875MB)
 *    84.00847606169872% used
 * From Space:
 *    capacity = 589824 (0.5625MB)
 *    used     = 0 (0.0MB)
 *    free     = 589824 (0.5625MB)
 *    0.0% used
 * To Space:
 *    capacity = 589824 (0.5625MB)
 *    used     = 0 (0.0MB)
 *    free     = 589824 (0.5625MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 56623104 (54.0MB)
 *    used     = 12888248 (12.291191101074219MB)
 *    free     = 43734856 (41.70880889892578MB)
 *    22.761465001989293% used
 *
 * 803 interned Strings occupying 54232 bytes.
 *
 * 分析：
 * b1, b2, b3, b4 的大小都是 4MB，且都是新对象，但是 eden 空间 5MB 多，仅仅可以存放一个数组对象，
 * 因此，在经过连续 4 次空间分配后，只有 b4 可以在 eden 区，而其他 3 个数组
 * 对象都已经移动到了老年代。
 *
 * 示例二：
 *
 * 使用 `-Xmn`  或者 `-XX:NewRatio` 参数分配足够大的新生代空间
 *
 * 启动命令：
 *     java -Xms60M -Xmx60M -Xmn30M -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.PutInEden
 *     java -Xms60M -Xmx60M -XX:NewRatio=1 -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.PutInEden
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6460844k free), swap 7929852k(1201648k free)
 * CommandLine flags: -XX:InitialHeapSize=62914560 -XX:MaxHeapSize=62914560 -XX:NewRatio=1 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * Heap
 *  def new generation   total 27648K, used 17859K [0x00000000fc400000, 0x00000000fe200000, 0x00000000fe200000)
 *   eden space 24576K,  72% used [0x00000000fc400000, 0x00000000fd570c10, 0x00000000fdc00000)
 *   from space 3072K,   0% used [0x00000000fdc00000, 0x00000000fdc00000, 0x00000000fdf00000)
 *   to   space 3072K,   0% used [0x00000000fdf00000, 0x00000000fdf00000, 0x00000000fe200000)
 *  tenured generation   total 30720K, used 0K [0x00000000fe200000, 0x0000000100000000, 0x0000000100000000)
 *    the space 30720K,   0% used [0x00000000fe200000, 0x00000000fe200000, 0x00000000fe200200, 0x0000000100000000)
 *  Metaspace       used 2923K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 123583, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 62914560 (60.0MB)
 *    NewSize                  = 31457280 (30.0MB)
 *    MaxNewSize               = 31457280 (30.0MB)
 *    OldSize                  = 31457280 (30.0MB)
 *    NewRatio                 = 1
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 28311552 (27.0MB)
 *    used     = 17783944 (16.96009063720703MB)
 *    free     = 10527608 (10.039909362792969MB)
 *    62.81515050817419% used
 * Eden Space:
 *    capacity = 25165824 (24.0MB)
 *    used     = 17783944 (16.96009063720703MB)
 *    free     = 7381880 (7.039909362792969MB)
 *    70.66704432169597% used
 * From Space:
 *    capacity = 3145728 (3.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 3145728 (3.0MB)
 *    0.0% used
 * To Space:
 *    capacity = 3145728 (3.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 3145728 (3.0MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 31457280 (30.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 31457280 (30.0MB)
 *    0.0% used
 *
 * 803 interned Strings occupying 54232 bytes.
 *
 * 分析：
 * b1, b2, b3, b4 的大小都是 4MB，且都是新对象，eden 空间有 24MB，足以容纳这 4 个数组对象
 *
 */
public class PutInEden {

    private static final int _1MB = 1024 * 1024;

    public static void main(String[] args) {
        byte[] b1, b2, b3, b4;  // 定义变量
        b1 = new byte[4 * _1MB];  // 分配4MB堆空间，考察堆空间的使用情况
        b2 = new byte[4 * _1MB];
        b3 = new byte[4 * _1MB];
        b4 = new byte[4 * _1MB];

        try {
            System.out.println("PutInEden is started.");
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

```

示例二：

```java
package com.example.jvm.demo.performance;

import java.io.IOException;

/**
 *
 * 将新对象预留在新生代
 *
 * 示例一：
 *
 * 启动命令：
 *     java -Xms60M -Xmx60M -Xmn10M -XX:SurvivorRatio=8 -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.PutInEden2
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6457420k free), swap 7929852k(1203676k free)
 * CommandLine flags: -XX:InitialHeapSize=62914560 -XX:MaxHeapSize=62914560 -XX:MaxNewSize=10485760 -XX:NewSize=10485760 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:SurvivorRatio=8 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * 2021-01-30T14:59:12.619+0800: 0.137: [GC (Allocation Failure) 2021-01-30T14:59:12.619+0800: 0.137: [DefNew: 5279K->815K(9216K), 0.0033567 secs] 5279K->4911K(60416K), 0.0034568 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * 2021-01-30T14:59:12.623+0800: 0.141: [GC (Allocation Failure) 2021-01-30T14:59:12.623+0800: 0.141: [DefNew: 4911K->0K(9216K), 0.0010104 secs] 9007K->4906K(60416K), 0.0010550 secs] [Times: user=0.00 sys=0.00, real=0.01 secs]
 *
 * Heap
 *  def new generation   total 9216K, used 4337K [0x00000000fc400000, 0x00000000fce00000, 0x00000000fce00000)
 *   eden space 8192K,  52% used [0x00000000fc400000, 0x00000000fc83c448, 0x00000000fcc00000)
 *   from space 1024K,   0% used [0x00000000fcc00000, 0x00000000fcc00000, 0x00000000fcd00000)
 *   to   space 1024K,   0% used [0x00000000fcd00000, 0x00000000fcd00000, 0x00000000fce00000)
 *  tenured generation   total 51200K, used 4906K [0x00000000fce00000, 0x0000000100000000, 0x0000000100000000)
 *    the space 51200K,   9% used [0x00000000fce00000, 0x00000000fd2ca8a8, 0x00000000fd2caa00, 0x0000000100000000)
 *  Metaspace       used 2923K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 126131, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 62914560 (60.0MB)
 *    NewSize                  = 10485760 (10.0MB)
 *    MaxNewSize               = 10485760 (10.0MB)
 *    OldSize                  = 52428800 (50.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 9437184 (9.0MB)
 *    used     = 4356904 (4.155067443847656MB)
 *    free     = 5080280 (4.844932556152344MB)
 *    46.16741604275174% used
 * Eden Space:
 *    capacity = 8388608 (8.0MB)
 *    used     = 4356904 (4.155067443847656MB)
 *    free     = 4031704 (3.8449325561523438MB)
 *    51.9383430480957% used
 * From Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * To Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 52428800 (50.0MB)
 *    used     = 5023912 (4.791175842285156MB)
 *    free     = 47404888 (45.208824157714844MB)
 *    9.582351684570312% used
 *
 * 803 interned Strings occupying 54232 bytes.
 *
 * 分析：
 * 两个 4MB 对象一个在新生代，一个在老年代。0.5MB 的 b1 对象被存放在老年代，
 * 在垃圾回收的过程中，b1 对象的大小已经占据了 from 区的一半，故被直接送入了
 * 老年代。
 *
 * 示例二：
 *
 * 使用 `-XX:TargetSurvivorRatio` 参数（默认值为 50，表示 50%）提高 survior 区的利用率
 *
 * 启动命令：
 *     java -Xms60M -Xmx60M -Xmn10M -XX:SurvivorRatio=8 -XX:TargetSurvivorRatio=90 -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.PutInEden2
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6452952k free), swap 7929852k(1205720k free)
 * CommandLine flags: -XX:InitialHeapSize=62914560 -XX:MaxHeapSize=62914560 -XX:MaxNewSize=10485760 -XX:NewSize=10485760 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:SurvivorRatio=8 -XX:TargetSurvivorRatio=90 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * 2021-01-30T15:48:34.710+0800: 0.115: [GC (Allocation Failure) 2021-01-30T15:48:34.710+0800: 0.115: [DefNew: 5279K->815K(9216K), 0.0046713 secs] 5279K->4911K(60416K), 0.0048096 secs] [Times: user=0.01 sys=0.00, real=0.01 secs]
 * 2021-01-30T15:48:34.715+0800: 0.120: [GC (Allocation Failure) 2021-01-30T15:48:34.715+0800: 0.120: [DefNew: 4911K->810K(9216K), 0.0013748 secs] 9007K->4906K(60416K), 0.0014388 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * Heap
 *  def new generation   total 9216K, used 5147K [0x00000000fc400000, 0x00000000fce00000, 0x00000000fce00000)
 *   eden space 8192K,  52% used [0x00000000fc400000, 0x00000000fc83c448, 0x00000000fcc00000)
 *   from space 1024K,  79% used [0x00000000fcc00000, 0x00000000fccca898, 0x00000000fcd00000)
 *   to   space 1024K,   0% used [0x00000000fcd00000, 0x00000000fcd00000, 0x00000000fce00000)
 *  tenured generation   total 51200K, used 4096K [0x00000000fce00000, 0x0000000100000000, 0x0000000100000000)
 *    the space 51200K,   8% used [0x00000000fce00000, 0x00000000fd200010, 0x00000000fd200200, 0x0000000100000000)
 *  Metaspace       used 2923K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 128765, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 62914560 (60.0MB)
 *    NewSize                  = 10485760 (10.0MB)
 *    MaxNewSize               = 10485760 (10.0MB)
 *    OldSize                  = 52428800 (50.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 9437184 (9.0MB)
 *    used     = 5186496 (4.94622802734375MB)
 *    free     = 4250688 (4.05377197265625MB)
 *    54.958089192708336% used
 * Eden Space:
 *    capacity = 8388608 (8.0MB)
 *    used     = 4356904 (4.155067443847656MB)
 *    free     = 4031704 (3.8449325561523438MB)
 *    51.9383430480957% used
 * From Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 829592 (0.7911605834960938MB)
 *    free     = 218984 (0.20883941650390625MB)
 *    79.11605834960938% used
 * To Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 52428800 (50.0MB)
 *    used     = 4194320 (4.0000152587890625MB)
 *    free     = 48234480 (45.99998474121094MB)
 *    8.000030517578125% used
 *
 * 803 interned Strings occupying 54232 bytes.
 *
 * 分析：
 * 两个 4MB 对象一个在新生代，一个在老年代。0.5MB 的 b1 对象未超过 from 区的 90%，
 * 因此保存在新生代中。
 *
 * 示例三：
 *
 * 使用 `-XX:SurvivorRatio` 参数设置更大的 surivor 空间
 *
 * 启动命令：
 *     java -Xms60M -Xmx60M -Xmn10M -XX:SurvivorRatio=2 -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.PutInEden2
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6454016k free), swap 7929852k(1205840k free)
 * CommandLine flags: -XX:InitialHeapSize=62914560 -XX:MaxHeapSize=62914560 -XX:MaxNewSize=10485760 -XX:NewSize=10485760 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:SurvivorRatio=2 -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * 2021-01-30T15:50:31.623+0800: 0.109: [GC (Allocation Failure) 2021-01-30T15:50:31.623+0800: 0.109: [DefNew: 1143K->815K(7680K), 0.0018282 secs] 1143K->815K(58880K), 0.0019345 secs] [Times: user=0.00 sys=0.00, real=0.01 secs]
 * 2021-01-30T15:50:31.626+0800: 0.111: [GC (Allocation Failure) 2021-01-30T15:50:31.626+0800: 0.111: [DefNew: 4911K->810K(7680K), 0.0027650 secs] 4911K->4906K(58880K), 0.0028187 secs] [Times: user=0.01 sys=0.00, real=0.00 secs]
 * 2021-01-30T15:50:31.629+0800: 0.115: [GC (Allocation Failure) 2021-01-30T15:50:31.629+0800: 0.115: [DefNew: 4906K->810K(7680K), 0.0009220 secs] 9002K->4906K(58880K), 0.0009620 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * Heap
 *  def new generation   total 7680K, used 5060K [0x00000000fc400000, 0x00000000fce00000, 0x00000000fce00000)
 *   eden space 5120K,  83% used [0x00000000fc400000, 0x00000000fc826800, 0x00000000fc900000)
 *   from space 2560K,  31% used [0x00000000fcb80000, 0x00000000fcc4a898, 0x00000000fce00000)
 *   to   space 2560K,   0% used [0x00000000fc900000, 0x00000000fc900000, 0x00000000fcb80000)
 *  tenured generation   total 51200K, used 4096K [0x00000000fce00000, 0x0000000100000000, 0x0000000100000000)
 *    the space 51200K,   8% used [0x00000000fce00000, 0x00000000fd200010, 0x00000000fd200200, 0x0000000100000000)
 *  Metaspace       used 2923K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 128921, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 62914560 (60.0MB)
 *    NewSize                  = 10485760 (10.0MB)
 *    MaxNewSize               = 10485760 (10.0MB)
 *    OldSize                  = 52428800 (50.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 2
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 7864320 (7.5MB)
 *    used     = 5128792 (4.891197204589844MB)
 *    free     = 2735528 (2.6088027954101562MB)
 *    65.21596272786458% used
 * Eden Space:
 *    capacity = 5242880 (5.0MB)
 *    used     = 4299200 (4.10003662109375MB)
 *    free     = 943680 (0.89996337890625MB)
 *    82.000732421875% used
 * From Space:
 *    capacity = 2621440 (2.5MB)
 *    used     = 829592 (0.7911605834960938MB)
 *    free     = 1791848 (1.7088394165039062MB)
 *    31.64642333984375% used
 * To Space:
 *    capacity = 2621440 (2.5MB)
 *    used     = 0 (0.0MB)
 *    free     = 2621440 (2.5MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 52428800 (50.0MB)
 *    used     = 4194320 (4.0000152587890625MB)
 *    free     = 48234480 (45.99998474121094MB)
 *    8.000030517578125% used
 *
 * 803 interned Strings occupying 54232 bytes.
 *
 * 分析：
 * 两个 4MB 对象一个在新生代，一个在老年代。0.5MB 的 b1 对象未超过 from 区（2.5MB）的 一半，
 * 因此保存在新生代中。
 *
 */
public class PutInEden2 {

    private static final int _1MB = 1024 * 1024;

    public static void main(String[] args) {
        byte[] b1, b2, b3;  // 定义变量
        b1 = new byte[1024 * 512];  // 分配 0.5MB 堆空间
        b2 = new byte[4 * _1MB];    // 分配 4MB 堆空间
        b3 = new byte[4 * _1MB];

        b3 = null;  // 使 b3 可以回收

        b3 = new byte[4 * _1MB];

        try {
            System.out.println("PutInEden2 is started.");
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

```

### 2. 大对象直接进入老年代

大对象就是指需要**大量连续内存空间**的 Java 对象，最典型的大对象便是那种**很长的字符串**，或者**元素数量很庞大的数组**。

在分配空间时，大对象容易导致内存明明还有不好空间时就提前触发垃圾收集，以便获取足够的**连续空间**才能安置它们，而当复制对象时，大对象就意味着高额的内存复制开销。

`-XX:PretenureSizeThreshold` 参数用于指定大于该设置值的对象直接在老年代分配（默认值为 0），避免在 Eden 区及两个 Survior 区之间来回复制，产生大量的内存复制操作。该参数只对 `Serial` 和 `ParNew` 两款新生代收集器有效。

将大对象直接分配到老年代，保持新生代对象结构的完整性，可以提高 GC 的效率。如果非常不幸的，**一个大对象同时又是短命的对象**，假设这种情况出现得比较频繁，那么对于 GC 来说将会是一场灾难。原本应该用于存放永久对象的老年代，被短命的对象塞满，这也意味着对堆空间进行了洗牌，扰乱了分代内存回收的基本思路。

> 短命的大对象对垃圾回收是一场灾难。目前没有一种特别好的回收方法处理这个问题。因此，开发人员应该尽量避免使用短命的大对象。

示例一：

```java
package com.example.jvm.demo.performance;

import java.io.IOException;

/**
 * 大对象直接分配到老年代
 *
 * 示例一：
 *
 * 启动命令：
 *     java -Xms60M -Xmx60M -XX:PretenureSizeThreshold=3145728 -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.PutBigObjectInOld
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6449836k free), swap 7929852k(1208500k free)
 * CommandLine flags: -XX:InitialHeapSize=62914560 -XX:MaxHeapSize=62914560 -XX:PretenureSizeThreshold=3145728 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * Heap
 *  def new generation   total 18432K, used 1639K [0x00000000fc400000, 0x00000000fd800000, 0x00000000fd800000)
 *   eden space 16384K,  10% used [0x00000000fc400000, 0x00000000fc599d08, 0x00000000fd400000)
 *   from space 2048K,   0% used [0x00000000fd400000, 0x00000000fd400000, 0x00000000fd600000)
 *   to   space 2048K,   0% used [0x00000000fd600000, 0x00000000fd600000, 0x00000000fd800000)
 *  tenured generation   total 40960K, used 16384K [0x00000000fd800000, 0x0000000100000000, 0x0000000100000000)
 *    the space 40960K,  40% used [0x00000000fd800000, 0x00000000fe800040, 0x00000000fe800200, 0x0000000100000000)
 *  Metaspace       used 2923K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 132339, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 62914560 (60.0MB)
 *    NewSize                  = 20971520 (20.0MB)
 *    MaxNewSize               = 20971520 (20.0MB)
 *    OldSize                  = 41943040 (40.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 18874368 (18.0MB)
 *    used     = 1006760 (0.9601211547851562MB)
 *    free     = 17867608 (17.039878845214844MB)
 *    5.33400641547309% used
 * Eden Space:
 *    capacity = 16777216 (16.0MB)
 *    used     = 1006760 (0.9601211547851562MB)
 *    free     = 15770456 (15.039878845214844MB)
 *    6.000757217407227% used
 * From Space:
 *    capacity = 2097152 (2.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 2097152 (2.0MB)
 *    0.0% used
 * To Space:
 *    capacity = 2097152 (2.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 2097152 (2.0MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 41943040 (40.0MB)
 *    used     = 16777280 (16.00006103515625MB)
 *    free     = 25165760 (23.99993896484375MB)
 *    40.000152587890625% used
 *
 * 803 interned Strings occupying 54232 bytes.
 *
 * 分析：
 * 4个 4MB 对象直接分配在老年代
 *
 */
public class PutBigObjectInOld {

    private static final int _1MB = 1024 * 1024;

    public static void main(String[] args) {
        byte[] b1, b2, b3, b4;  // 定义变量
        b1 = new byte[4 * _1MB];  // 分配4MB堆空间，考察堆空间的使用情况
        b2 = new byte[4 * _1MB];
        b3 = new byte[4 * _1MB];
        b4 = new byte[4 * _1MB];

        try {
            System.out.println("PutBigObjectInOld is started.");
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

```

### 3. 长期存活的对象将进入老年代

虚拟机给每个对象定义了一个对象年龄（Age）计数器，存储在对象头中。对象通常在 Eden 区里诞生，如果经过第一次 Minor GC 后仍然存活，并且能够被 Survior 容纳的话，该对象会被移动到 Survior 空间中，并且将其对象年龄设置为 1 岁。对象在 Survior 区中每熬过一次 Minor GC，年龄就增加 1 岁，当它的年龄增加到一定程度（默认值为15），就会被晋升到老年代中。`-XX:MaxTenuringThreshold` 参数指定对象晋升老年代的年龄阈值。

示例一：

```java
package com.example.jvm.demo.performance;

import java.io.IOException;

/**
 * 长期存活的对象将进入老年代
 *
 * 示例一：
 *
 * 启动命令：
 *     java -Xms20M -Xmx20M -Xmn10M -XX:MaxTenuringThreshold=1 -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.TenuringThreshold
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6447404k free), swap 7929852k(1210088k free)
 * CommandLine flags: -XX:InitialHeapSize=20971520 -XX:InitialTenuringThreshold=1 -XX:MaxHeapSize=20971520 -XX:MaxNewSize=10485760 -XX:MaxTenuringThreshold=1 -XX:NewSize=10485760 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * 2021-01-30T17:32:31.114+0800: 0.119: [GC (Allocation Failure) 2021-01-30T17:32:31.114+0800: 0.119: [DefNew: 5023K->559K(9216K), 0.0043981 secs] 5023K->4655K(19456K), 0.0045328 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * 2021-01-30T17:32:31.119+0800: 0.124: [GC (Allocation Failure) 2021-01-30T17:32:31.119+0800: 0.124: [DefNew: 4655K->0K(9216K), 0.0012754 secs] 8751K->4650K(19456K), 0.0013380 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * Heap
 *  def new generation   total 9216K, used 4336K [0x00000000fec00000, 0x00000000ff600000, 0x00000000ff600000)
 *   eden space 8192K,  52% used [0x00000000fec00000, 0x00000000ff03c340, 0x00000000ff400000)
 *   from space 1024K,   0% used [0x00000000ff400000, 0x00000000ff400000, 0x00000000ff500000)
 *   to   space 1024K,   0% used [0x00000000ff500000, 0x00000000ff500000, 0x00000000ff600000)
 *  tenured generation   total 10240K, used 4650K [0x00000000ff600000, 0x0000000100000000, 0x0000000100000000)
 *    the space 10240K,  45% used [0x00000000ff600000, 0x00000000ffa8a8c8, 0x00000000ffa8aa00, 0x0000000100000000)
 *  Metaspace       used 2923K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 134480, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 20971520 (20.0MB)
 *    NewSize                  = 10485760 (10.0MB)
 *    MaxNewSize               = 10485760 (10.0MB)
 *    OldSize                  = 10485760 (10.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 9437184 (9.0MB)
 *    used     = 4356640 (4.154815673828125MB)
 *    free     = 5080544 (4.845184326171875MB)
 *    46.16461859809028% used
 * Eden Space:
 *    capacity = 8388608 (8.0MB)
 *    used     = 4356640 (4.154815673828125MB)
 *    free     = 4031968 (3.845184326171875MB)
 *    51.93519592285156% used
 * From Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * To Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 10485760 (10.0MB)
 *    used     = 4761800 (4.541206359863281MB)
 *    free     = 5723960 (5.458793640136719MB)
 *    45.41206359863281% used
 *
 * 803 interned Strings occupying 54232 bytes.
 *
 * 分析：
 * `-XX:MaxTenuringThreshold=1` 设置为1 ，b1 对象在第 2 次
 * GC 时晋升到老年代，survior 空间使用为 0。
 *
 */
public class TenuringThreshold {

    private static final int _1MB = 1024 * 1024;

    public static void main(String[] args) {
        byte[] b1, b2, b3;  // 定义变量
        b1 = new byte[_1MB / 4];  // 什么时候进入老年代决定于 -XX:MaxTenuringThreshold 的设置
        b2 = new byte[4 * _1MB];    // 分配 4MB 堆空间
        b3 = new byte[4 * _1MB];

        b3 = null;  // 使 b3 可以回收

        b3 = new byte[4 * _1MB];

        try {
            System.out.println("TenuringThreshold is started.");
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}

```

### 4. 动态对象年龄判定

HotSpot 虚拟机并不是永远要求对象的年龄必须达到 `-XX:MaxTenuringThreshold` 才能晋升老年代，如果在 Survior 空间中**相同年龄**所有对象大小的总和大于 Survior 空间的一半，年龄**大于或等于**该年龄的对象就可以直接进入老年代。

示例一：

```java
package com.example.jvm.demo.performance;

import java.io.IOException;

/**
 * 动态对象年龄判定
 *
 * 示例一：
 *
 * 启动命令：
 *     java -Xms20M -Xmx20M -Xmn10M -XX:MaxTenuringThreshold=15 -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.TenuringThresholdDynamic
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6445968k free), swap 7929852k(1210592k free)
 * CommandLine flags: -XX:InitialHeapSize=20971520 -XX:MaxHeapSize=20971520 -XX:MaxNewSize=10485760 -XX:MaxTenuringThreshold=15 -XX:NewSize=10485760 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * 2021-01-30T17:45:01.896+0800: 0.117: [GC (Allocation Failure) 2021-01-30T17:45:01.896+0800: 0.117: [DefNew: 5279K->815K(9216K), 0.0038975 secs] 5279K->4911K(19456K), 0.0040133 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * 2021-01-30T17:45:01.901+0800: 0.121: [GC (Allocation Failure) 2021-01-30T17:45:01.901+0800: 0.121: [DefNew: 4911K->0K(9216K), 0.0011728 secs] 9007K->4906K(19456K), 0.0012285 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * Heap
 *  def new generation   total 9216K, used 4337K [0x00000000fec00000, 0x00000000ff600000, 0x00000000ff600000)
 *   eden space 8192K,  52% used [0x00000000fec00000, 0x00000000ff03c448, 0x00000000ff400000)
 *   from space 1024K,   0% used [0x00000000ff400000, 0x00000000ff400000, 0x00000000ff500000)
 *   to   space 1024K,   0% used [0x00000000ff500000, 0x00000000ff500000, 0x00000000ff600000)
 *  tenured generation   total 10240K, used 4906K [0x00000000ff600000, 0x0000000100000000, 0x0000000100000000)
 *    the space 10240K,  47% used [0x00000000ff600000, 0x00000000ffaca8f8, 0x00000000ffacaa00, 0x0000000100000000)
 *  Metaspace       used 2923K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 135189, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 20971520 (20.0MB)
 *    NewSize                  = 10485760 (10.0MB)
 *    MaxNewSize               = 10485760 (10.0MB)
 *    OldSize                  = 10485760 (10.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 9437184 (9.0MB)
 *    used     = 4356904 (4.155067443847656MB)
 *    free     = 5080280 (4.844932556152344MB)
 *    46.16741604275174% used
 * Eden Space:
 *    capacity = 8388608 (8.0MB)
 *    used     = 4356904 (4.155067443847656MB)
 *    free     = 4031704 (3.8449325561523438MB)
 *    51.9383430480957% used
 * From Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * To Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 10485760 (10.0MB)
 *    used     = 5023992 (4.791252136230469MB)
 *    free     = 5461768 (5.208747863769531MB)
 *    47.91252136230469% used
 *
 * 803 interned Strings occupying 54256 bytes.
 *
 * 分析：
 * `-XX:MaxTenuringThreshold=1` 设置为15 ，b1 和 b2 对象在第 2 次
 * GC 时晋升到老年代，因为 b1 和 b2 对象所占空间已达 Survior 空间的一半，
 * 且它们同龄，survior 空间使用为 0。
 *
 */
public class TenuringThresholdDynamic {

    private static final int _1MB = 1024 * 1024;

    public static void main(String[] args) {
        byte[] b1, b2, b3, b4;  // 定义变量
        b1 = new byte[_1MB / 4];
        b2 = new byte[_1MB / 4];  // b1 + b2 大于 survior 空间的一半
        b3 = new byte[4 * _1MB];
        b4 = new byte[4 * _1MB];
        b4 = null;  // 使 b3 可以回收

        b4 = new byte[4 * _1MB];

        try {
            System.out.println("TenuringThresholdDynamic is started.");
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
```

### 5. 空间分配担保

在发生 Minor GC 之前，虚拟机必须检查老年代**最大可用的连续空间是否大于新生代所有对象总空间**，如果这个条件成立，那这一次 Minor GC 可以确保是安全的。如果不成立，则虚拟机（JDK 6 Update 24之前）会先查看 `-XX:HandlePromotionFailure` 参数的设置值是否允许担保失败（Handle Promotion Failure）：如果允许，那么继续检查老年代最大可用的连续空间是否大于历次晋升到老年代对象的平均大小，如果大于，将尝试进行一次 Minor GC，尽管这次 Minor GC 是有风险的；如果小于，或者 `-XX:HandlePromotionFailure` 设置不允许冒险，那这时就要改为进行一次 Full GC。

在 JDK 6 Update 24 之后，`-XX:HandlePromotionFailure` 参数不会再影响到虚拟机的空间分配担保策略，规则变为只要老年代的连续空间大于**新生代对象总大小**或**历次晋升的平均大小**（即默认支持空间分配担保失败），就会进行 Minor GC，否则将进行 Full GC。


示例一：

```java
package com.example.jvm.demo.performance;

import java.io.IOException;

/**
 * 空间分配担保
 *
 * 示例一：
 *
 * 启动命令：
 *     java -Xms20M -Xmx20M -Xmn10M -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.HandlePromotion success
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6261460k free), swap 7929852k(1321252k free)
 * CommandLine flags: -XX:+HeapDumpOnOutOfMemoryError -XX:InitialHeapSize=20971520 -XX:MaxHeapSize=20971520 -XX:MaxNewSize=10485760 -XX:NewSize=10485760 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * 2021-02-01T14:13:04.596+0800: 0.101: [GC (Allocation Failure) 2021-02-01T14:13:04.596+0800: 0.101: [DefNew: 6815K->304K(9216K), 0.0030751 secs] 6815K->4400K(19456K), 0.0031933 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * 2021-02-01T14:13:04.600+0800: 0.105: [GC (Allocation Failure) 2021-02-01T14:13:04.600+0800: 0.105: [DefNew: 6608K->298K(9216K), 0.0016443 secs] 10704K->6442K(19456K), 0.0016882 secs] [Times: user=0.00 sys=0.00, real=0.00 secs]
 * Heap
 *  def new generation   total 9216K, used 2653K [0x00000000fec00000, 0x00000000ff600000, 0x00000000ff600000)
 *   eden space 8192K,  28% used [0x00000000fec00000, 0x00000000fee4caa8, 0x00000000ff400000)
 *   from space 1024K,  29% used [0x00000000ff400000, 0x00000000ff44aaa8, 0x00000000ff500000)
 *   to   space 1024K,   0% used [0x00000000ff500000, 0x00000000ff500000, 0x00000000ff600000)
 *  tenured generation   total 10240K, used 6144K [0x00000000ff600000, 0x0000000100000000, 0x0000000100000000)
 *    the space 10240K,  60% used [0x00000000ff600000, 0x00000000ffc00030, 0x00000000ffc00200, 0x0000000100000000)
 *  Metaspace       used 2924K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 38349, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 20971520 (20.0MB)
 *    NewSize                  = 10485760 (10.0MB)
 *    MaxNewSize               = 10485760 (10.0MB)
 *    OldSize                  = 10485760 (10.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 9437184 (9.0MB)
 *    used     = 2548864 (2.4307861328125MB)
 *    free     = 6888320 (6.5692138671875MB)
 *    27.00873480902778% used
 * Eden Space:
 *    capacity = 8388608 (8.0MB)
 *    used     = 2243032 (2.1391220092773438MB)
 *    free     = 6145576 (5.860877990722656MB)
 *    26.739025115966797% used
 * From Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 305832 (0.29166412353515625MB)
 *    free     = 742744 (0.7083358764648438MB)
 *    29.166412353515625% used
 * To Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 10485760 (10.0MB)
 *    used     = 6291504 (6.0000457763671875MB)
 *    free     = 4194256 (3.9999542236328125MB)
 *    60.000457763671875% used
 *
 * 806 interned Strings occupying 54472 bytes.
 *
 * 分析：分配b4之前，由于 eden 区空间不足，且此时老年代空间足够，进行 1 次 Minor GC，
 * 回收 b1 所占内存，b2 和 b3 由于所占内存大于 survior 空间容量，晋升到老年代；
 * 分配b7之前，由于支持空间分配担保失败，且老年代的连续空间大于新生代对象总大小，进行第 2 次 Minor GC
 *
 *
 * 示例二：
 *
 * 启动命令：
 *     java -Xms20M -Xmx20M -Xmn10M -XX:+UseSerialGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -Xloggc:logs/gc-%t.log -XX:+HeapDumpOnOutOfMemoryError -cp jvm-demo-0.0.1-SNAPSHOT.jar com.example.jvm.demo.performance.HandlePromotion failure
 *
 * GC 日志内容：
 * Java HotSpot(TM) 64-Bit Server VM (25.131-b11) for linux-amd64 JRE (1.8.0_131-b11), built on Mar 15 2017 01:23:40 by "java_re" with gcc 4.3.0 20080428 (Red Hat 4.3.0-8)
 * Memory: 4k page, physical 15673600k(6261060k free), swap 7929852k(1321304k free)
 * CommandLine flags: -XX:+HeapDumpOnOutOfMemoryError -XX:InitialHeapSize=20971520 -XX:MaxHeapSize=20971520 -XX:MaxNewSize=10485760 -XX:NewSize=10485760 -XX:+PrintGC -XX:+PrintGCDateStamps -XX:+PrintGCDetails -XX:+PrintGCTimeStamps -XX:+UseCompressedClassPointers -XX:+UseCompressedOops -XX:+UseSerialGC
 * 2021-02-01T14:18:23.860+0800: 0.117: [GC (Allocation Failure) 2021-02-01T14:18:23.860+0800: 0.117: [DefNew: 6815K->304K(9216K), 0.0033107 secs] 6815K->4400K(19456K), 0.0034206 secs] [Times: user=0.00 sys=0.00, real=0.01 secs]
 * 2021-02-01T14:18:23.864+0800: 0.121: [GC (Allocation Failure) 2021-02-01T14:18:23.864+0800: 0.121: [DefNew (promotion failed) : 6608K->6602K(9216K), 0.0026229 secs]2021-02-01T14:18:23.867+0800: 0.124: [Tenured: 8192K->6441K(10240K), 0.0043042 secs] 10704K->6441K(19456K), [Metaspace: 2917K->2917K(1056768K)], 0.0070045 secs] [Times: user=0.01 sys=0.00, real=0.00 secs]
 * Heap
 *  def new generation   total 9216K, used 2354K [0x00000000fec00000, 0x00000000ff600000, 0x00000000ff600000)
 *   eden space 8192K,  28% used [0x00000000fec00000, 0x00000000fee4caa8, 0x00000000ff400000)
 *   from space 1024K,   0% used [0x00000000ff400000, 0x00000000ff400000, 0x00000000ff500000)
 *   to   space 1024K,   0% used [0x00000000ff500000, 0x00000000ff500000, 0x00000000ff600000)
 *  tenured generation   total 10240K, used 6441K [0x00000000ff600000, 0x0000000100000000, 0x0000000100000000)
 *    the space 10240K,  62% used [0x00000000ff600000, 0x00000000ffc4a6c8, 0x00000000ffc4a800, 0x0000000100000000)
 *  Metaspace       used 2924K, capacity 4486K, committed 4864K, reserved 1056768K
 *   class space    used 320K, capacity 386K, committed 512K, reserved 1048576K
 *
 * 堆信息：
 * Attaching to process ID 38681, please wait...
 * Debugger attached successfully.
 * Server compiler detected.
 * JVM version is 25.131-b11
 *
 * using thread-local object allocation.
 * Mark Sweep Compact GC
 *
 * Heap Configuration:
 *    MinHeapFreeRatio         = 40
 *    MaxHeapFreeRatio         = 70
 *    MaxHeapSize              = 20971520 (20.0MB)
 *    NewSize                  = 10485760 (10.0MB)
 *    MaxNewSize               = 10485760 (10.0MB)
 *    OldSize                  = 10485760 (10.0MB)
 *    NewRatio                 = 2
 *    SurvivorRatio            = 8
 *    MetaspaceSize            = 21807104 (20.796875MB)
 *    CompressedClassSpaceSize = 1073741824 (1024.0MB)
 *    MaxMetaspaceSize         = 17592186044415 MB
 *    G1HeapRegionSize         = 0 (0.0MB)
 *
 * Heap Usage:
 * New Generation (Eden + 1 Survivor Space):
 *    capacity = 9437184 (9.0MB)
 *    used     = 2243032 (2.1391220092773438MB)
 *    free     = 7194152 (6.860877990722656MB)
 *    23.768022325303818% used
 * Eden Space:
 *    capacity = 8388608 (8.0MB)
 *    used     = 2243032 (2.1391220092773438MB)
 *    free     = 6145576 (5.860877990722656MB)
 *    26.739025115966797% used
 * From Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * To Space:
 *    capacity = 1048576 (1.0MB)
 *    used     = 0 (0.0MB)
 *    free     = 1048576 (1.0MB)
 *    0.0% used
 * tenured generation:
 *    capacity = 10485760 (10.0MB)
 *    used     = 6596296 (6.290718078613281MB)
 *    free     = 3889464 (3.7092819213867188MB)
 *    62.90718078613281% used
 *
 * 791 interned Strings occupying 53432 bytes.
 *
 * 分析：分配b4之前，由于 eden 区空间不足，且此时老年代空间足够，进行 1 次 Minor GC，
 * 回收 b1 所占内存，b2 和 b3 由于所占内存大于 survior 空间容量，晋升到老年代；
 * 分配b7之前，由于支持空间分配担保失败，但是老年代的连续空间小于新生代对象总大小，
 * 空间担保失败，进行 Full GC
 *
 */
public class HandlePromotion {

    private static final int _1MB = 1024 * 1024;

    public static void main(String[] args) {

        if (args == null || args.length == 0 || "success".equals(args[0])) {
            handlePromotionSuccess();
        } else {
            handlePromotionFailure();
        }

        try {
            System.out.println("HandlePromotion is started.");
            System.in.read();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    /**
     * JDK 8 默认支持空间分配担保失败，
     * 老年代的连续空间大于新生代对象总大小或历次晋升的平均大小（即默认支持空间分配担保失败）
     * 就会进行 Minor GC，否则将进行 Full GC。
     *
     */
    public static void handlePromotionSuccess() {
        byte[] b1, b2, b3, b4, b5, b6, b7;  // 定义变量
        b1 = new byte[2 * _1MB];
        b2 = new byte[2 * _1MB];
        b3 = new byte[2 * _1MB];
        b1 = null;

        b4 = new byte[2 * _1MB];  // 第 1 次 Minor GC
        b5 = new byte[2 * _1MB];
        b6 = new byte[2 * _1MB];
        b5 = null;
        b6 = null;

        b7 = new byte[2 * _1MB];  // 由于支持空间分配担保失败，且老年代的连续空间大于新生代对象总大小，进行第 2 次 Minor GC
    }

    /**
     * 空间担保失败，进行Full GC
     *
     */
    public static void handlePromotionFailure() {
        byte[] b1, b2, b3, b4, b5, b6, b7;  // 定义变量
        b1 = new byte[2 * _1MB];
        b2 = new byte[2 * _1MB];
        b3 = new byte[2 * _1MB];
        b1 = null;

        b4 = new byte[2 * _1MB];  // 1 次 Minor GC
        b5 = new byte[2 * _1MB];
        b6 = new byte[2 * _1MB];

        b2 = null;
        b3 = null;  // 释放b2空间，确保老年代空间能容纳

        b7 = new byte[2 * _1MB];  // 由于支持空间分配担保失败，但是老年代的连续空间小于新生代对象总大小，空间担保失败，进行 Full GC
    }
}
```
