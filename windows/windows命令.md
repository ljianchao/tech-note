# windows命令

## 输出环境变量

- `echo %JAVA_HOME%`

## 进程命令

- 查看进程列表: `tasklist`
- 查看某个进程号：`tasklist|findstr "8080"`
- 关闭某个进程：`taskkill -f -t -im javaw.exe`

## 网络命令

- 查看端口占用情况：`netstat -ano|findstr "2181"`
