# spring-cloud-config

## spring-cloud-config-server

### bootstrap.properties

- 应用名：{application}
- 环境名：{profile}
- 分支名：{label}

### Git配置仓库

application.properties增加的内容：

```properties
    spring.cloud.config.server.git.uri=
    spring.cloud.config.server.git.username=
    spring.cloud.config.server.git.password=
```

### 本地文件系统

application.properties增加的内容：

```properties
    # 使用本地文件系统的存储方式来保存配置
    spring.profiles.active=native
    # 指定搜索文件路径的配置\
    spring.cloud.config.server.native.searchLocation=classpath:/config
```

## spring-cloud-config-client

### URI指定配置中心

bootstrap.properties增加以下内容：

```properties
    # 对应配置文件中的{application}内容
    spring.application.name=
    # spring-cloud-config-server的地址
    spring.cloud.config.uri=
    # 对应配置文件中的{profile}内容
    spring.cloud.config.profile=
    # 对应分支内容，如不配置，默认为master
    spring.cloud.label=

```



