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

```shell
    java -Xms1024m -Xmx1024m -jar spring-boot-demo-0.0.1-SNAPSHOT.jar
```

使用命令 `jmap -heap <pid>` 查看内存分配：

```
Attaching to process ID 202318, please wait...
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
   used     = 165777344 (158.09759521484375MB)
   free     = 102658112 (97.90240478515625MB)
   61.75687313079834% used
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
   used     = 5836704 (5.566314697265625MB)
   free     = 710340704 (677.4336853027344MB)
   0.8149801899364019% used

10936 interned Strings occupying 928112 bytes.
```

## 参考

- [Java - jmx远程调优](https://www.jianshu.com/p/923580d3a5a2)
