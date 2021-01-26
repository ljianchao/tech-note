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
