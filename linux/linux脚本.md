# shell脚本

## 输入和输出

### 标准文件描述符
Linux系统将每个对象当做文件处理。这包括输入和输出进程。Linux用`文件描述符(file descriptor)`来标识每个文件对象。文件描述符是一个非负整数，可以唯一标识会话中打开的文件。每个进程一次最多可以打开九个文件描述符。出于特殊目的，`bash shell`保留了前三个文件描述符（0、1和2），见下表：

| 文件描述符 |  缩写  |   描述   |   POSIX名称   |    文件句柄     |
| :--------: | :----: | :------: | :-----------: | :-------------: |
|     0      | STDIN  | 标准输入 | STDIN_FILENO  | /proc/self/fd/0 |
|     1      | STDOUT | 标准输出 | STDOUT_FILENO | /proc/self/fd/1 |
|     2      | STDERR | 标准错误 | STDERR_FILENO | /proc/self/fd/2 |

#### STDIN
`STDIN`文件描述符代表shell的标准输入。对终端界面来说，标准输入是键盘。许多`bash`命令能接收`STDIN`输入，尤其是没有在命令行上指定文件的话。用`cat`命令处理`STDIN`输入的数据：
```shell
    $ cat 
    this is a test 
    this is a test 
    this is a second test. 
    this is a second test.
```

使用输入重定向符合（`<`）时，Linux会用重定向指定的文件来替换标准输入文件描述符。它会读取文件并提取数据，就如同它是键盘上输入的。
```shell
    $ cat < testfile 
    This is the first line. 
    This is the second line. 
    This is the third line. 
    $
```

#### STDOUT
`STDOUT`文件描述符代表`shell`的标准输出。在终端界面上，标准输出就是终端显示器。`shell` 的所有输出（包括shell中运行的程序和脚本）会被定向到标准输出中，也就是显示器。

通过输出重定向符号（`>`），通常会显示到显示器的所有输出会被shell重定向到指定的重定向文件。 你也可以将数据追加到某个文件。这可以用`>>`符号来完成。

输出重定向：
```shell
    $ ls -l > test2 
    $ cat test2
```

追加内容：
```shell
    $ who >> test2
```

当命令生成错误消息时，shell并未将错误消息重定向到输出重定向文件。`shell`创建了输出重 定向文件，但错误消息却显示在了显示器屏幕上。

#### STDERR
`shell`通过特殊的`STDERR`文件描述符来处理错误消息。`STDERR`文件描述符代表shell的标准错误输出。`shell`或`shell`中运行的程序和脚本出错时生成的错误消息都会发送到这个位置。

##### 只重定向错误
`STDERR`文件描述符被设成2。可以选择只重定向错误消息，将该文 件描述符值放在重定向符号前。该值必须紧紧地放在重定向符号前，否则不会工作。
```shell
    $ ls -al badfile 2> test4 
    $ cat test4 
    ls: cannot access badfile: No such file or directory 
    $
```

##### 重定向错误和数据
使用两个重定向符号重定向错误和正常输出，错误数据文件和正常输出文件分开。
```shell
    $ ls -al test test2 test3 badtest 2> test6 1> test7 
    $ cat test6 
    ls: cannot access test: No such file or directory 
    ls: cannot access badtest: No such file or directory 
    $ cat test7
    -rw-rw-r-- 1 rich rich 158 2014-10-16 11:32 test2
    -rw-rw-r-- 1 rich rich 0 2014-10-16 11:33 test3 $
``` 

使用`bash shell`提供的特殊重定向符号`&>`将错误输出和正常输出到一个文件中。
```shell
    $ ls -al test test2 test3 badtest &> test7 
    $ cat test7 
    ls: cannot access test: No such file or directory 
    ls: cannot access badtest: No such file or directory
    -rw-rw-r-- 1 rich rich 158 2014-10-16 11:32 test2
    -rw-rw-r-- 1 rich rich 0 2014-10-16 11:33 test3 $
```

`&>file`是一种特殊的用法，也可以写成`>&file`，二者的意思完全相同，都等价于`>file 2>&1`。

### 阻止命令输出

#### /dev/null
有时候，你可能不想显示脚本的输出。这在将脚本作为后台进程运行时很常见。要解决这个问题，可以将`STDERR`重定向到一个叫作`null`文件的特殊文件。`shell`输出到`null`文件的任何数据都不会保存，全部都被丢掉了。在Linux系统上null文件的标准位置是`/dev/null`。你重定向到该位置的任何数据都会被丢掉，不会显示。

`/dev/null` 表示空设备文件。`commond 1> /dev/null`表示执行command命令产生了标准输出`stdout`（用1表示），重定向到`/dev/null`的设备文件中。

`/dev/null`理解为`/dev`路径下的空文件，该命令将command命令的标准输出输出到空文件中。

`2>/dev/null`把错误信息输出到空设备文件。

`>/dev/null 2>&1`等同于`1>/dev/null 2>&1`。把标准输出重定向到空设备文件，同时把错误输出`2`重定向到标准输出`1`，即标准输出和错误输出都输入到空设备文件。

`2>&1 >/dev/null` 把错误输出`2`重定向到标准输出`1`，此时是屏幕，然后把标准输出`1`重定向到空设备文件，即错误输出到屏幕，标准输出到空设备文件。

`2>1`表示把错误输出重定向到当前路径下文件名为`1`的文件中。
