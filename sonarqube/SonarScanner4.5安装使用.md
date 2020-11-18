# SonarScanner4.5安装使用

## 环境

- 操作系统: Linux - Centos-7-x86_64-1810
- Docker版本: Docker-CE 19.03.13
- SonarQube版本：8.4-community
- SonarScanner版本：4.5

## 下载地址

Linux64位版本[下载](https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-4.5.0.2216-linux.zip)。

Windows64位版本[下载](https://binaries.sonarsource.com/Distribution/sonar-scanner-cli/sonar-scanner-cli-4.5.0.2216-windows.zip)

## 配置项目

在需要分析的项目的根目录下创建配置文件`sonar-project.properties`，内容如下：

```properties
# must be unique in a given SonarQube instance
sonar.projectKey=demo

# --- optional properties ---

# defaults to project key
#sonar.projectName=My project
# defaults to 'not provided'
#sonar.projectVersion=1.0
 
# Path is relative to the sonar-project.properties file. Defaults to .
#sonar.sources=.
 
# Encoding of the source code. Default is default system encoding
#sonar.sourceEncoding=UTF-8
```

## 运行SonarScanner 

进入解压后的文件夹`/opt/soft/sonar-scanner-4.5.0.2216-linux`，修改配置文件`conf/sonar-scanner.properties`，更新和`SonarQube server`相关的全局配置：

```properties
#----- Default SonarQube server
sonar.host.url=http://localhost:9000

#----- Default source code encoding
sonar.sourceEncoding=UTF-8
```

编辑`/etc/profile`文件，将目录`/opt/soft/sonar-scanner-4.5.0.2216-linux/bin`加入到系统的环境变量中：

```
   export PATH=$PATH:/opt/soft/sonar-scanner-4.5.0.2216-linux/bin
```

刷新环境变量：

```shell
   source /etc/profile
```

验证环境变量是否生效：

```shell
    sonar-scanner -h
```

在项目的根目录运行如下命令进行分析：

```shell
    sonar-scanner
```

也可以使用完整的参数运行命令：

```shell
    sonar-scanner -Dsonar.host.url=http://localhost:9000 \
    -Dsonar.projectKey=demo  \
    -Dsonar.java.binaries=. \
    -Dsonar.java.source=8 \
    -Dsonar.branch.name=master \
    -X
```

windows系统的命令如下：

```shell
    sonar-scanner -Dsonar.host.url=http://localhost:9000 ^
    -Dsonar.projectKey=demo  ^
    -Dsonar.java.binaries=. ^
    -Dsonar.java.source=8 ^
    -Dsonar.branch.name=master ^
    -X
```

如maven项目中有需要排除分析的模块，可以使用`-Dsonar.exclusions`参数

```
    -Dsonar.exclusions=demo-redis/**/*, demo-cache/**/*
```

## 参考

- [SonarScanner官方介绍](https://docs.sonarqube.org/latest/analysis/scan/sonarscanner/#)
- [SonarQube Analysis Parameters](https://docs.sonarqube.org/latest/analysis/analysis-parameters/)
- [SonarQube Java](https://docs.sonarqube.org/latest/analysis/languages/java/)
- [解决新版sonar-java插件需要配置sonar.java.binaries参数的问题](https://blog.csdn.net/u013111003/article/details/79927668)
