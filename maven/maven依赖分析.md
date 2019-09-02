## dependency配置

dependency配置示例如下：
```xml
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-web</artifactId>
        <version>2.1.3.RELEASE</version>
        <type>jar</type>
        <scope>compile</scope>
        <optional>true</optional>
        <exclusions>
            <exclusion>
                <groupId>org.springframework</groupId>
                <artifactId>spring-web</artifactId>
            </exclusion>
        </exclusions>
    </dependency>
```

### 元素简介

- `groupId`、`artifactId`和`version`：依赖的基本坐标。
- `type`：依赖的类型，对应于项目坐标定义的packaging。大部分情况下，该元素不必声明，默认值为`jar`。
- `scope`：依赖的范围。
- `optional`：标记依赖是否可选。
- `exclusions`：用来排除传递性依赖。

## 依赖范围

依赖范围是用来控制依赖与三种`classpath`（编译`classpath`、测试`classpath`、运行`classpath`）的关系，Maven有以下几种依赖范围：

- `compile`：编译依赖范围。如果没有指定，默认使用`compile`范围。此依赖范围的Maven依赖，对于编译、测试和运行三种`classpath`都有效。
- `test`：测试依赖范围。此依赖范围的Maven依赖，只对于测试`classpath`有效。只有在编译测试代码及运行测试的时候才需要。
- `provided`：已提供依赖范围。此依赖范围的Maven依赖，对于编译和测试`classpath`有效，在运行时无效。典型例子`servlet-api`，编译和测试项目的时候需要该依赖，但在运行项目的时候，由于容器已提供，就不需要Maven重复地引入一遍。
- `runtime`：运行时依赖范围。此依赖范围的Maven依赖，对于测试和运行`classpath`有效，但在编译主代码时无效。`JDBC`驱动实现，项目主代码的编译只需要`JDK`提供的`JDBC`的接口，只有在执行测试或者运行项目的时候才需要实现上述接口的具体`JDBC`驱动。
- `system`：系统依赖范围。依赖范围和`provided`范围完全一致。使用`system`范围的依赖必须通过`systemPath`元素显示指定依赖文件的路径。此类依赖不是通过Maven仓库解析的，且与本机系统绑定，可能造成构建的不可移植，请谨慎使用。`systemPath`元素可以引用环境变量，如：
```xml
    <dependency>
        <groupId>javax.sql</groupId>
        <artifactId>jdbc-stdext</artifactId>
        <version>2.0</version>
        <scope>system</scope>
        <systemPath>${java.home}/lib/rt.jar</systemPath>
    </dependency>
```

- `import`（Maven 2.0.9及以上）：导入依赖范围。该依赖范围不会对三种`classpath`产生实际的影响，通常用在`dependencyManagement`元素中。

各依赖范围与三种`classpath`的关系如表：

| 依赖范围(`Scope`) | 编译`classpath`有效 | 测试`classpath`有效 | 运行时`classpath`有效 |            列举             |
| :---------------: | :-----------------: | :-----------------: | :-------------------: | :-------------------------: |
|      compile      |          Y          |          Y          |           Y           |         spring-core         |
|       test        |          -          |          Y          |           -           |            JUnit            |
|     provided      |          Y          |          Y          |           -           |         servlet-api         |
|      runtime      |          -          |          Y          |           Y           |        JDBC驱动实现         |
|      system       |          Y          |          Y          |           -           | 本地的，Maven仓库之外的依赖 |

## 传递性依赖

Maven会解析各个直接依赖的POM，将那些必要的间接依赖，以传递性依赖的形式引入到当前的项目中。

### 传递性依赖和依赖范围

第一直接依赖的范围和第二直接依赖的范围决定了传递性依赖的范围。

下表最左边一列表示第一直接依赖范围，最上面一行表示第二直接依赖范围，中间的交叉单元格则表示传递性依赖范围。

|          | compile  | test  | provided | runtime  |
| :------: | :------: | :---: | :------: | :------: |
| compile  | compile  |   -   |    -     | runtime  |
|   test   |   test   |   -   |    -     |   test   |
| provided | provided |   -   | provided | provided |
| runtime  | runtime  |   -   |    -     | runtime  |

传递性依赖的规律：
- 当第二直接依赖的范围是`compile`的时候，传递性依赖的范围与第一直接依赖的范围一致。
- 当第二直接依赖的范围是`test`的时候，依赖不会得以传递。
- 当第二直接依赖的范围是`provided`的时候，只传递第一直接依赖范围也为`provided`的依赖，且传递性依赖的范围同样为`provided`。
- 当第二直接依赖的范围是`runtime`的时候，传递性依赖的范围与第一直接依赖的范围一直，但`compile`例外，此时传递性依赖的范围为`runtime`。

## 依赖调解

- Maven依赖调解（Denpendency Mediation）的第一原则：路径最近者优先。
- Maven依赖调解的第二原则：第一声明者优先。在依赖路径长度相等的前提下，在POM中依赖声明的顺序决定了谁会被解析使用，顺序最靠前的那个依赖优胜。

## 可选依赖

可选依赖不会得以传递。之所以使用可选依赖，可能项目中实现了两个特性，其中的特性一依赖于X，特性二依赖于Y，而且两个特性互斥，用户不可能同时使用两个特性。使用该项目的功能时，需显示地声明依赖X或者Y。

在理想的情况下，是不应该使用可选依赖的。更好的做法是分别为X和Y创建一个Maven项目，基于同样的groupId分配不同的artifactId。

## 排除依赖

排除传递依赖的几种情况

- 当前项目依赖的第三方依赖，由于某些原因依赖了另外一个类库的`SNAPSHOT`版本，不稳定性会直接影响到当前项目。这时需要排除掉该`SNAPSHOT`，并且在当前项目中显示的声明该类库的某个正式发布的版本。
- 有一些特殊情况，你可能也想要替换某个传递性依赖。例如，由于版权的因素，某类库不在中央仓库中，而另一个项目有一个对应的实现。

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

### 查看已解析依赖

查看已解析依赖以及依赖范围，命令：
```shell
    mvn dependency:list
```

### 查看依赖树

定位到`pom.xml`文件所在的位置运行命令：
```shell
    mvn dependency:tree
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
    [WARNING]    org.springframework:sprinfg-web:jar:5.1.8.RELEASE:compile
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

`Used undeclared dependencies`，意指项目中使用到的，但是没有显示声明的依赖，例如很多相关的`Java import`声明，而这种依赖是通过直接依赖传递进来的，当升级直接依赖的时候，相关传递性依赖的版本也可能发生变化，这种变化不易察觉，但是可能导致当前项目出错。因此，显示地声明任何项目中直接使用到的依赖。

`Unused declared dependencies`，意指项目中未使用的，但显示声明的依赖。对于这类依赖，我们不应该简单地直接删除其声明。由于`dependency:analyze`只会分析编译主代码和测试代码需要用到的依赖，一些执行测试和运行时需要的依赖它就发现不了。

### 冲突查找

定位到`pom.xml`文件所在的位置运行命令：
```xml
    mvn -X compile dependency:tree -Dverbose >debug.log
```

在文件`debug.log`里查找是否有"omitted for conflict with"，如果有，则表示jar包有版本冲突。




