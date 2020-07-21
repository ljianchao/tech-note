
# pattern表达式

- 输出logger的名称

```
c{precision}
logger{precision}
```

- 输出日期

```
d{pattern}
date{pattern}
```

- 输出消息体

```
m{nolookups}{ansi}
msg{nolookups}{ansi}
message{nolookups}{ansi}
```

- 输出换行

```
n
```

- 输出进程ID

```
pid{[defaultValue]}
processId{[defaultValue]}
```

- 输出日志级别

```
p| level{ level= label, level= label, ...} p|
level{length= n} p| level{lowerCase= true| false}
```

- 输出异常

The same as the %throwable conversion word
but the stack trace is printed starting with the
first exception that was thrown followed by each
subsequent wrapping exception.

```
rEx| rException| rThrowable
{
["none" | "short" | "full" | depth]
[,filters(package,package,...)]
[,separator( separator)]
}
{ansi(
Key=Value,Value,...
Key=Value,Value,...
...)
}
{suffix( pattern)}
```

The same as the %throwable conversion word
but also includes class packaging information.

```
xEx| xException| xThrowable
{
["none" | "short" | "full" | depth]
[,filters(package,package,...)]
[,separator( separator)]
}
{ansi(
Key=Value,Value,...
Key=Value,Value,...
...)
}
{suffix( pattern)}
```

- 输出线程id

```
T
tid
threadId
```

- 输出线程名称

```
t
tn
thread
threadName
```
