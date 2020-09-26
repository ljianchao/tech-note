# vs code配置c环境

## 安装编译环境

我们使用**Mingw-w64**作为编译环境。

### 下载Mingw-w64

离线下载地址是：https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/

下载的版本为**x86_64-win32-seh**，下载后为压缩包，包名`x86_64-8.1.0-release-win32-seh-rt_v6-rev0.7z`

### 配置Mingw-w64

解压下载的压缩包到自己指定的位置，我是默认解压在`C:\DevEnvironment`，里面的`mingw64`就是安装的编译器了，将里面的`bin`文件夹的路径加入环境变量中，即将`C:\DevEnvironment\mingw64\bin`路径加入windows的环境变量的系统变量`Path`中。

打开命令行工具，使用以下命令验证安装：

```shell
    gcc -v
```

## 配置vs code

### 安装插件vscode-cpptools

一、安装步骤：

- 离线下载插件vscode-cpptools：https://github.com/microsoft/vscode-cpptools/releases；
- 在vscode中使用快捷键`Ctrl+Shift+P`调出命令输入框，输入`"Install from VSIX..."`进行选择要安装的插件；
- 重启vscode。

二、配置vscode-cpptools

要使用vscode-cpptools，需要配置几个json文件。首先，在工作区的根目录创建**文件夹**`.vscode`，然后创建以下4个json文件：

- `launch.json` (debugger settings)
- `tasks.json` (build instructions)
- `c_cpp_properties.json` (可选配置，compiler path and IntelliSense settings)
- `settings.json` (可选配置)



`launch.json`内容：

```json
// https://code.visualstudio.com/docs/editor/variables-reference
// https://code.visualstudio.com/docs/cpp/launch-json-reference
// https://code.visualstudio.com/docs/editor/debugging#_launch-configurations
{
    "version": "0.2.0",
    "configurations": [{
        "name": "(gdb) Launch", // 配置名称，将会在启动配置的下拉菜单中显示
        "type": "cppdbg", // 配置类型，cppdbg对应cpptools提供的调试功能；可以认为此处只能是cppdbg
        "request": "launch", // 请求配置类型，可以为launch（启动）或attach（附加）
        "program": "${fileDirname}\\${fileBasenameNoExtension}.exe", // 将要进行调试的程序的路径
        "args": [], // 程序调试时传递给程序的命令行参数，一般设为空即可
        "stopAtEntry": false, // 设为true时程序将暂停在程序入口处，相当于在main上打断点
        "cwd": "${workspaceFolder}", // 调试程序时的工作目录，此为工作区文件夹；改成${fileDirname}可变为文件所在目录
        "environment": [], // 环境变量
        "externalConsole": true, // 使用单独的cmd窗口，与其它IDE一致；为false时使用内置终端
        "internalConsoleOptions": "neverOpen", // 如果不设为neverOpen，调试时会跳到“调试控制台”选项卡，你应该不需要对gdb手动输命令吧？
        "MIMode": "gdb", // 指定连接的调试器，可以为gdb或lldb。但我没试过lldb
        "miDebuggerPath": "gdb.exe", // 调试器路径，Windows下后缀不能省略，Linux下则不要
        "setupCommands": [
            { // 模板自带，好像可以更好地显示STL容器的内容，具体作用自行Google
                "description": "Enable pretty-printing for gdb",
                "text": "-enable-pretty-printing",
                "ignoreFailures": false
            }
        ],
        "preLaunchTask": "Compile" // 调试会话开始前执行的任务，一般为编译程序。与tasks.json的label相对应
    }]
}
```

`tasks.json`内容：

```json
// https://code.visualstudio.com/docs/editor/variables-reference
// https://code.visualstudio.com/docs/editor/tasks
{
    "version": "2.0.0",
    "tasks": [{
        "type": "process", // process是把预定义变量和转义解析后直接全部传给command；shell相当于先打开shell再输入命令，所以args还会经过shell再解析一遍
        "label": "Compile", // 任务名称，与launch.json的preLaunchTask相对应
        "command": "gcc",   // 要使用的编译器，C++用g++
        "args": [
            "${file}",
            "-o",    // 指定输出文件名，不加该参数则默认输出a.exe，Linux下默认a.out
            "${fileDirname}\\${fileBasenameNoExtension}.exe",
            "-g",    // 生成和调试有关的信息
            "-m64", // 不知为何有时会生成16位应用而无法运行，加上此条可强制生成64位的
            "-Wall", // 开启额外警告
            "-static-libgcc",     // 静态链接libgcc，一般都会加上
            "-fexec-charset=GBK", // 生成的程序使用GBK编码，不加这条会导致Win下输出中文乱码；繁体系统改成BIG5
            // "-std=c11", // 要用的语言标准，根据自己的需要修改。c++可用c++14
        ], // 编译的命令，其实相当于VSC帮你在终端中输了这些东西
        "group": {
            "kind": "build",
            "isDefault": true // 不为true时ctrl shift B就要手动选择了
        },
        "presentation": {
            "echo": true,
            "reveal": "always", // 执行任务时是否跳转到终端面板，可以为always，silent，never。具体参见VSC的文档
            "focus": false,     // 设为true后可以使执行task时焦点聚集在终端，但对编译C/C++来说，设为true没有意义
            "panel": "shared"   // 不同的文件的编译信息共享一个终端面板
        },
        "problemMatcher": [
            "$gcc"  // 捕捉编译时终端里的报错信息到问题面板中，修改代码后需要重新编译才会再次触发；本来有Lint，再开problemMatcher就有双重报错，但MinGW的Lint效果实在太差了；用Clang可以注释掉
        ]
    }]
}
```

`settings.json`内容：

```json
{
    "files.defaultLanguage": "c", // ctrl+N新建文件后默认的语言
    "editor.formatOnType": true,  // 输入分号(C/C++的语句结束标识)后自动格式化当前这一行的代码
    "editor.suggest.snippetsPreventQuickSuggestions": false, // clangd的snippets有很多的跳转点，不用这个就必须手动触发Intellisense了
    "editor.acceptSuggestionOnEnter": "off", // 我个人的习惯，按回车时一定是真正的换行，只有tab才会接受Intellisense
    // "editor.snippetSuggestions": "top", // （可选）snippets显示在补全列表顶端，默认是inline

    "code-runner.runInTerminal": true, // 设置成false会在“输出”中输出，无法输入
    "code-runner.executorMap": {
        "c": "gcc '$fileName' -o '$fileNameWithoutExt.exe' -Wall -O2 -m64 -lm -static-libgcc -std=c11 -fexec-charset=GBK && &'./$fileNameWithoutExt.exe'",
        "cpp": "g++ '$fileName' -o '$fileNameWithoutExt.exe' -Wall -O2 -m64 -static-libgcc -std=c++14 -fexec-charset=GBK && &'./$fileNameWithoutExt.exe'"
        // "c": "gcc $fileName -o $fileNameWithoutExt.exe -Wall -O2 -m64 -lm -static-libgcc -std=c11 -fexec-charset=GBK && $dir$fileNameWithoutExt.exe",
        // "cpp": "g++ $fileName -o $fileNameWithoutExt.exe -Wall -O2 -m64 -static-libgcc -std=c++14 -fexec-charset=GBK && $dir$fileNameWithoutExt.exe"
    }, // 右键run code时运行的命令；未注释的仅适用于PowerShell（Win10默认）和pwsh，文件名中有空格也可以编译运行；注释掉的适用于cmd（win7默认）、PS和bash，但文件名中有空格时无法运行
    "code-runner.saveFileBeforeRun": true, // run code前保存
    "code-runner.preserveFocus": true,     // 若为false，run code后光标会聚焦到终端上。如果需要频繁输入数据可设为false
    "code-runner.clearPreviousOutput": false, // 每次run code前清空属于code runner的终端消息，默认false
    "code-runner.ignoreSelection": true,   // 默认为false，效果是鼠标选中一块代码后可以单独执行，但C是编译型语言，不适合这样用
    "code-runner.fileDirectoryAsCwd": true, // 将code runner终端的工作目录切换到文件目录再运行，对依赖cwd的程序产生影响；如果为false，executorMap要加cd $dir

    "C_Cpp.clang_format_sortIncludes": true, // 格式化时调整include的顺序（按字母排序）
}
```

三、Variables Reference

`launch.json`和`tasks.json`文件中支持变量引用。

以下列出一些预定义的变量:

- `${workspaceFolder}` - the path of the folder opened in VS Code
- `${workspaceFolderBasename}` - the name of the folder opened in VS Code without any slashes (/)
- `${file}` - the current opened file
- `${relativeFile}` - the current opened file relative to workspaceFolder
- `${relativeFileDirname}` - the current opened file's dirname relative to workspaceFolder
- `${fileBasename}` - the current opened file's basename
- `${fileBasenameNoExtension}`- the current opened file's basename with no file extension
- `${fileDirname}` - the current opened file's dirname
- `${fileExtname}` - the current opened file's extension
- `${cwd}` - the task runner's current working directory on startup
- `${lineNumber}` - the current selected line number in the active file
- `${selectedText}` - the current selected text in the active file
- `${execPath}` - the path to the running VS Code executable
- `${defaultBuildTask}` - the name of the default build task

三一、预定义变量使用的示例

假如你拥有如下的需求：

- 你的编辑器已打开的文件位于`/home/your-username/your-project/folder/file.ext`；
- 已打开的目录`/home/your-username/your-project`作为你的**root workspace**。

所有针对每个变量的值如下：

- `${workspaceFolder}` - /home/your-username/your-project
- `${workspaceFolderBasename}` - your-project
- `${file}` - /home/your-username/your-project/folder/file.ext
- `${relativeFile}` - folder/file.ext
- `${relativeFileDirname}` - folder
- `${fileBasename}` - file.ext
- `${fileBasenameNoExtension}` - file
- `${fileDirname}` - /home/your-username/your-project/folder
- `${fileExtname}` - .ext
- `${lineNumber}` - line number of the cursor
- `${selectedText}` - text selected in your code editor
- `${execPath}` - location of Code.exe
Tip: Use IntelliSense inside string values for tasks.json and launch.json to get a full list of predefined variables.

## 参考

- [Variables Reference](https://code.visualstudio.com/docs/editor/variables-reference)
- [C/C++ for Visual Studio Code](https://code.visualstudio.com/docs/languages/cpp)
- [Using GCC with MinGW](https://code.visualstudio.com/docs/cpp/config-mingw)
- [Debugging](https://code.visualstudio.com/docs/editor/debugging#_launch-configurations)
- [Configuring C/C++ debugging](https://code.visualstudio.com/docs/cpp/launch-json-reference)
- [Customizing default settings](https://code.visualstudio.com/docs/cpp/customize-default-settings-cpp)
- [vscode-cpptools](https://github.com/microsoft/vscode-cpptools)
- [Visual Studio Code 如何编写运行 C、C++ 程序？](https://www.zhihu.com/question/30315894/answer/154979413)
- [Visual Studio Code 如何编写运行 C、C++ 程序？](https://blog.csdn.net/lyw851230/article/details/89352839)
- [vscode 配置 C/C++ 编译环境 教程](https://blog.csdn.net/qq_43041976/article/details/100542557)
- [整理：Visual Studio Code (vscode) 配置C、C++环境](https://blog.csdn.net/bat67/article/details/76095813)
