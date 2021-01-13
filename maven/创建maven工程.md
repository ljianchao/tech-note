# 创建maven工程

## maven创建项目命令

```
# 创建java项目
mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring -Dpackage=com.demo.spring -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false -DarchetypeCatalog=local

# 创建web项目
mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring -Dpackage=com.demo.spring -DarchetypeArtifactId=maven-archetype-webapp -DinteractiveMode=false -DarchetypeCatalog=local
```

## 创建层级目录项目

创建父目录：

```
    mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring -DinteractiveMode=false
```

进入父目录，删除 `src` 目录，修改 `pom.xml` 文件中的属性 `packaging` 为 `pom`，然后在当前目录下运行以下命令创建子目录：

```
    mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring-api -Dpackage=com.demo.api -DinteractiveMode=false

    mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring-common -Dpackage=com.demo.common -DinteractiveMode=false

    mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring-dao -Dpackage=com.demo.dao -DinteractiveMode=false

    mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring-service -Dpackage=com.demo.service -DinteractiveMode=false

    mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring-server -Dpackage=com.demo.server -DinteractiveMode=false
```

## 依赖分析

```shell
    mvn clean dependency:analyze > dep.tree

    mvn clean dependency:tree > dep2.tree
```

## 编译指定模块及依赖模块

```shell
    mvn clean install -pl demo-spring-api -am
```
