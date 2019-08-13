## dependency相关命令

### 查看有效pom配置

定位到`pom.xml`文件所在的位置运行命令：
```shell
    mvn help:effective-pom
```

### 解析依赖

定位到`pom.xml`文件所在的位置运行命令：
```shell
    mvn dependency:resolve
```

### 依赖分析

定位到`pom.xml`文件所在的位置运行命令：
```shell
    mvn dependency:analyze
```

输出
```shell
    [WARNING] Used undeclared dependencies found:
    [WARNING]    org.springframework.boot:spring-boot-autoconfigure:jar:2.1.3.RELEASE:compile
    [WARNING]    org.springframework:spring-tx:jar:5.1.8.RELEASE:compile
    [WARNING]    org.springframework.boot:spring-boot:jar:2.1.3.RELEASE:compile
    [WARNING]    io.zipkin.brave:brave:jar:5.6.5:compile
    [WARNING]    org.springframework:spring-web:jar:5.1.8.RELEASE:compile
    [WARNING]    org.springframework:spring-beans:jar:5.1.8.RELEASE:compile
    [WARNING]    org.springframework.data:spring-data-redis:jar:2.1.9.RELEASE:compile
    [WARNING]    org.mybatis:mybatis:jar:3.4.4:compile
    [WARNING]    com.alibaba:druid:jar:1.1.17:compile
    [WARNING]    org.mybatis.spring.boot:mybatis-spring-boot-autoconfigure:jar:1.3.0:compile
    [WARNING]    org.springframework:spring-context:jar:5.1.8.RELEASE:compile
    [WARNING]    org.mybatis:mybatis-spring:jar:1.3.1:compile
    [WARNING] Unused declared dependencies found:
    [WARNING]    org.springframework.boot:spring-boot-starter-thymeleaf:jar:2.1.3.RELEASE:compile
    [WARNING]    org.springframework.boot:spring-boot-configuration-processor:jar:2.1.3.RELEASE:compile
    [WARNING]    org.springframework.boot:spring-boot-starter-redis:jar:1.3.8.RELEASE:compile
    [WARNING]    org.mybatis.spring.boot:mybatis-spring-boot-starter:jar:1.3.0:compile
    [WARNING]    com.alibaba:druid-spring-boot-starter:jar:1.1.17:compile
    [WARNING]    com.github.pagehelper:pagehelper:jar:3.7.5:compile
    [WARNING]    javax.inject:javax.inject:jar:1:compile
    [WARNING]    javax.servlet:javax.servlet-api:jar:4.0.1:compile
    [WARNING]    commons-codec:commons-codec:jar:1.11:compile
    [WARNING]    com.alibaba:fastjson:jar:1.2.31:compile
    [WARNING]    commons-io:commons-io:jar:2.5:compile
    [WARNING]    org.apache.httpcomponents:httpclient:jar:4.5.7:compile
    [WARNING]    org.apache.httpcomponents:httpmime:jar:4.5.7:compile
    [WARNING]    org.apache.poi:ooxml-schemas:jar:1.1:compile
    [WARNING]    org.springframework.cloud:spring-cloud-starter-zipkin:jar:2.1.2.RELEASE:compile
    [WARNING]    io.zipkin.brave:brave-instrumentation-mysql:jar:5.6.5:compile
    [WARNING]    org.springframework.cloud:spring-cloud-starter-sleuth:jar:2.1.2.RELEASE:compile
    [WARNING]    org.springframework.cloud:spring-cloud-sleuth-zipkin:jar:2.1.2.RELEASE:compile
    [WARNING]    org.springframework.kafka:spring-kafka:jar:2.2.4.RELEASE:compile
    [WARNING]    org.springframework.boot:spring-boot-starter-web:jar:2.1.3.RELEASE:compile
```

### 查看依赖树

定位到`pom.xml`文件所在的位置运行命令：
```shell
    mvn dependency:tree
```



