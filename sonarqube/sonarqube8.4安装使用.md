# SonarQube使用

## UI界面操作

UI界面的地址和账号信息如下：

```
   URL: http://192.168.1.124:9000
   用户名/密码: admin/admin
```

## maven方式提交检查

```shell
    mvn sonar:sonar -Dsonar.projectKey=youlin-health-rj -Dsonar.host.url=http://192.168.1.124:9000 -Dsonar.login=a7da7f65aefe48c4ba0e200e6ab7f337f702f81f -Dsonar.branch.name=circle
```

- 参数`-Dsonar.projectKey`指定项目名称
- 参数`-Dsonar.host.url`指定管理端地址
- 参数`-Dsonar.login`指定管理界面的令牌
- 参数`-Dsonar.branch.name`指定分支名称

## Idea代码检查插件 - SonarLint 安装使用

SonarLint 是 SonarQube的单机版，可以作为Idea的插件进行代码规范检查，也可以连接到SonarQube中生成统计报告。

### 插件安装

#### 通过下载安装包进行安装

- 插件已上传到跳板机的`D:\idea plugins`目录下
- `IntelliJ IDEA` 打开`Settings >> Plugins >> Install plugin from disk...`，选择刚刚下载的zip包安装，然后重启IDE

### 插件使用

右键单击项目(或文件)，找到`SonarLint`并点击`Analyze with SonarLint`进行分析，查看分析结果。

## Idea代码检查插件 - ali-p3c 安装使用

### 插件安装

#### 通过下载安装包进行安装

- 插件已上传到跳板机的`D:\idea plugins`目录下
- `IntelliJ IDEA` 打开`Settings >> Plugins >> Install plugin from disk...`，选择刚刚下载的zip包安装，然后重启IDE

### 插件使用

右键单击项目(或文件)，点击`编码规约扫描`进行分析，查看分析结果。
