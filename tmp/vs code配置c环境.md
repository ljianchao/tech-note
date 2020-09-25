# vs code配置c环境

## 安装编译环境

我们使用**MinGW**作为编译环境。

### 下载MinGW

离线下载地址是：https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/

下载的版本为**x86_64-win32-seh**，下载后为压缩包，包名`x86_64-8.1.0-release-win32-seh-rt_v6-rev0.7z`

### 配置MinGW

解压下载的压缩包到自己指定的位置，我是默认解压在`C:\DevEnvironment`，里面的`mingw64`就是安装的编译器了，将里面的`bin`文件夹的路径加入环境变量中，即将`C:\DevEnvironment\mingw64\bin`路径加入windows的环境变量的系统变量`Path`中。

打开命令行工具，使用以下命令验证安装：

```shell
    gcc -v
```

## 配置vs code

### 安装插件vscode-cpptools

安装步骤：

- 离线下载插件vscode-cpptools：https://github.com/microsoft/vscode-cpptools/releases；
- 在vscode中使用快捷键`Ctrl+Shift+P`调出命令输入框，输入`"Install from VSIX..."`进行选择要安装的插件；
- 重启vscode。

## 参考

- 
