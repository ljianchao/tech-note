# gitlab集成SonarQube实现代码检测

## 环境

- 操作系统: Linux - Centos-7-x86_64-1810
- Docker版本: Docker-CE 19.03.13
- gitlab版本：13.5.4-ce.0
- gitlab-runner版本：gitlab-ci-multi-runner-9.5.1-1.x86_64
- SonarQube版本：8.4-community
- SonarScanner版本：4.5

## gitlab-runner的安装和使用

一般来说，构建任务都会占用很多的系统资源 (譬如编译代码)，而 `GitLab CI` 又是`GitLab`的一部分，如果由`GitLab CI`来运行构建任务的话，在执行构建任务的时候，`GitLab`的性能会大幅下降。

`GitLab CI`最大的作用是管理各个项目的构建状态，因此，运行构建任务这种浪费资源的事情就交给`GitLab Runner`来做。`GitLab Runner`可以分布在不同的主机上，同一个主机上也可以有多个Runner，所以在构建任务运行期间并不会影响到 `GitLab`的性能。

GitLab-Runner可以分为两种类型：Shared Runner（共享型）和Specific Runner（指定型）。

- `Shared Runner`：这种Runner是所有工程都能够用的。只有系统管理员能够创建Shared Runner。
- `Specific Runner`：这种Runner只能为指定的工程服务。拥有该工程访问权限的人都能够为该工程创建Specific Runner。

### 安装

#### 1. 添加yum源

```shell
  curl -L https://packages.gitlab.com/install/repositories/runner/gitlab-ci-multi-runner/script.rpm.sh | sudo bash
```

#### 2. 安装gitlab-runner

```shell
    yum install gitlab-ci-multi-runner
```

#### 3. 设置权限

在`gitlab-runner`执行过程中大多数文件夹不存在，无权限。需要先卸载，然后增加`root`用户重新安装。

```shell
    # 创建缓存目录
    mkdir -p /opt/data/gitlab-runner & \
    mkdir /opt/data/gitlab-runner/builds & \
    mkdir /opt/data/gitlab-runner/cache

    # 卸载gitlab-runner
    sudo gitlab-runner uninstall 

    # 重新安装服务并设置--user(设置为root)
    gitlab-runner install --working-directory /opt/data/gitlab-runner --user root

    # 重启gitlab-runner
    sudo service gitlab-runner restart  

    # 查看当前runner用户
    ps aux|grep gitlab-runner  
```

#### 4. 向gitLab-CI注册runner

向`GitLab-CI`注册一个Runner需要两样东西：GitLab-CI的url和注册token。打开`GitLab`中的项目页面，在项目的`Settings`设置页面，选择`CI/CD Pipelines`选项卡，找到GitLab-CI的url和注册token。

使用以下命令来进行交互注册：

```shell
    gitlab-runner register --name my-runner --url http://gitlab.example.com --registration-token my-registration-token --executor= shell
```

按照提示一步一步执行就完成注册了。注册完成会生成注册文件`/etc/gitlab-runner/config.toml`，可以修改本文件增加缓存的目录的设置：

```toml
concurrent = 1
check_interval = 0

[[runners]]
  name = "my-runner"
  url = "http://gitlab.example.com"
  token = "my-registration-token"
  executor = "shell"
  builds_dir = "/opt/data/gitlab-runner/builds"
  cache_dir = "/opt/data/gitlab-runner/cache"
  [runners.cache]
```

更新后重启`gitlab-runner`服务：

```shell
    gitlab-runner restart
```

注册完成后，登录gitlab，打开项目页面，在项目的`Settings`设置页面，选择`CI/CD Pipelines`选项卡，可以看到已注册的runner。

### gitlab-runner常用命令

```shell
    gitlab-runner list  # 查看各个runner的状态
    gitlab-runner start  # 启动服务
    gitlab-runner stop  # 停止服务
    gitlab-runner restart # 重启服务
```

### git-lab runner使用

#### 1. 配置`.gitlab-ci.yml`文件

在项目的根目录下创建`.gitlab-ci.yml`文件，用来定义要进行的操作。

当有新内容`push`到仓库，或者有代码合并后，`GitLab`会查找是否有`.gitlab-ci.yml`文件，如果文件存在，`Runners`将会根据该文件的内容开始`build`本次`commit`。

示例：

```yml
stages:
  - test

job1:
  stage: test
  only:
    - master
  script:
    - echo "hi iam gitlab-ci"
```

#### 2. 提交代码

对项目进行代码修改，然后提交。

#### 3. 查看结果

登录gitlab管理页面，查看本次提交的执行状态。

## 集成SonarQube

> `sonar-scanner`必须和`gitlab-runner`安装在同一服务器上，这样`gitlab-runner`才可以调用`sonar-scanner`的命令进行分析。

gitlab集成SonarQube可以修改项目目录下的`.gitlab-ci.yml`文件，`script`内容设置为`sonar-scanner`运行的指令

```yml
variables:
  SONAR_URL: "http://127.0.0.1:9000"
  SONAR_LOGIN: 6d190aebe5c01fd18f22c212f47fc5f657a29f1d
  SONAR_EXCLUSIONS: "demo-redis/**/*,demo-mq/**/*"

stages:
  - test

job1:
  stage: test
  only:
    - master
  script:
    - sonar-scanner -Dsonar.branch.name=master -Dsonar.exclusions=$SONAR_EXCLUSIONS -Dsonar.projectKey=$CI_PROJECT_NAME -Dsonar.host.url=$SONAR_URL -Dsonar.login=$SONAR_LOGIN -Dsonar.sources=.  -Dsonar.java.binaries=. -Dsonar.java.source=8 -Dsonar.analysis.CI_COMMIT_REF_NAME=$CI_COMMIT_REF_NAME -Dsonar.analysis.GITLAB_USER_EMAIL=$GITLAB_USER_EMAIL -Dsonar.analysis.GITLAB_USER_NAME=$GITLAB_USER_NAME -Dsonar.analysis.CI_PROJECT_PATH=$CI_PROJECT_PATH -Dsonar.scm.disabled=true

job2:
  stage: test
  only:
    - dev
  script:
    - sonar-scanner -Dsonar.branch.name=dev -Dsonar.exclusions=$SONAR_EXCLUSIONS -Dsonar.projectKey=$CI_PROJECT_NAME -Dsonar.host.url=$SONAR_URL -Dsonar.login=$SONAR_LOGIN -Dsonar.sources=.  -Dsonar.java.binaries=. -Dsonar.java.source=8 -Dsonar.analysis.CI_COMMIT_REF_NAME=$CI_COMMIT_REF_NAME -Dsonar.analysis.GITLAB_USER_EMAIL=$GITLAB_USER_EMAIL -Dsonar.analysis.GITLAB_USER_NAME=$GITLAB_USER_NAME -Dsonar.analysis.CI_PROJECT_PATH=$CI_PROJECT_PATH -Dsonar.scm.disabled=true

```

## 参考

- [sonarQube + gitlab-runner 实现代码规范自动化检测](https://www.jianshu.com/p/2c974d8fdac0)
- [基于 SonarQube7.4 实现代码规范自动化检测解决方案](https://www.jianshu.com/p/4df14f20b118)
- [GitLab Runner的安装与使用](https://blog.csdn.net/lizhiqiang1217/article/details/88803783)
- [Gitlab Runner安装注册](https://www.jianshu.com/p/a7a8d4fcdfee)
- [gitlab-runner用户设置root权限](https://www.cnblogs.com/wu-wu/p/13426658.html)
- [持续集成之.gitlab-ci.yml篇](https://segmentfault.com/a/1190000019540360)
- [gitlab runner命令](https://blog.csdn.net/qq_34206560/article/details/88827395)
- [SonarQube 之 gitlab-plugin 配合 gitlab-ci 完成每次 commit 代码检测](https://blog.csdn.net/aixiaoyang168/article/details/78115646)
- [GitLab CI/CD environment variables](https://docs.gitlab.com/ee/ci/variables/#creating-a-custom-environment-variable)
