# JVM结构（The Structure of the Java Virtual Machine）

## 运行时数据区域（Run-Time Data Areas）

### pc寄存器(The pc Register)

每一个Java虚拟机线程都有自己的`pc（program counter）`寄存器。在任意时刻，每个java虚拟机线程只会执行一个方法的代码，这个正在被线程执行的方法称为该线程的当前方法（`current method`）。如果这个方法不是`native`的那pc寄存器就保存Java虚拟机正在执行的**字节码指令的地址**，如果该方法是`native`的，那pc寄存器的值就是`undefined`。pc寄存器的容量至少应当能保存一个`returnAddress`类型的数据或者一个与平台相关的本地指针的值。

### Java虚拟机栈（Java Virtual Machine Stacks）
