调整为CMS模式：

```
    -XX:+UseConcMarkSweepGC：启用CMS垃圾收集器。设置年老代为并发收集。
    -XX:+CMSParallelRemarkEnabled	降低标记停顿
    -XX:+UseCMSInitiatingOccupancyOnly 表示只在到达阀值的时候，才进行CMS GC
    -XX:CMSInitiatingOccupancyFraction=70 设置阀值为70%，默认为92%。 
    -XX:+ScavengeBeforeFullGC 默认启用	在Full GC前触发一次Minor GC
    -XX:+CMSScavengeBeforeRemark  它的意思是在执行CMS remark之前进行一次youngGC，这样能有效降低remark的时间。
```

下面是内存日志收集：

```
    -XX:+PrintGCDateStamps  输出gc的触发时间
    -verbose:gc 
    -XX:+PrintGCDetails 
    -Xloggc:/work/heapdump/gc.log
    -XX:+UseGCLogFileRotation 
    -XX:NumberOfGCLogFiles=10 
    -XX:GCLogFileSize=100M
    -XX:+HeapDumpOnOutOfMemoryError 
    -XX:HeapDumpPath=/work/heapdump/java_<pid>_<date>_<time>_heapDump.hprof
 ```
