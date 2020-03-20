# Linux文件描述符

## 概述

在Linux系統中一切都可以看成是文件，文件可以分为：普通文件、目录文件、链接文件和设备文件等。

文件描述符（fild descriptor，fd）是内核为了高效管理**已被打开**的文件所创建的索引，其是一个非负整数（通常是小整数），用于指代被打开的文件，所有执行`I/O`操作的系统调用都通过文件描述符。

每个进程按照惯例都会至少有三个打开的文件描述符：0、1和2，除非进程显示地关闭它们，0是`标准输入（stdin）`，1是`标准输出（stdout）`，2是`标准错误(stderr)`。C标准库提了预处理器宏：`STDIN_FILENO`，`STDOUT_FILENO`和`STDERR_FILENO`宏，以取代对以上整数的直接引用。

![标准文件描述符使用过程](resources/fd-std.png)

## 参考

- [Linux文件描述符](https://www.jianshu.com/p/430340c4a37a)
- [Linux中的文件描述符与打开文件之间的关系](https://blog.csdn.net/cywosp/article/details/38965239)
- [linux file description](http://guoshaoguang.com/blog/2013/08/03/linux-file-description/)
- [linux文件描述符限制及使用详解](https://www.jianshu.com/p/20f1e96557e3)