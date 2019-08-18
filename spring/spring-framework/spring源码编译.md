## Build from Source
[wiki](https://github.com/spring-projects/spring-framework/wiki/Build-from-Source)

## 编译

### 增加maven本地仓库

删除maven的默认配置`USER_HOME/.m2`，修改根目录的`build.gradle`文件，增加`mavenLocal()`内容到`repositories`中，内容如下：

```
    repositories {
		mavenLocal()
		maven { url "https://repo.spring.io/plugins-release" }
	}
```

### 增加远程仓库

```
	repositories {
		mavenLocal()
		maven { url "https://maven.aliyun.com/repository/public" }
		maven { url "http://uk.maven.org/maven2/" }
		maven { url "http://repo.springsource.org/plugins-release" }
	}
```

Tip: `repositories`元素有2处，都需要修改，分别位于`buildscript`和`configure`元素中.

Tip: Gradle uses the same logic as Maven to identify the location of your local Maven cache. If a local repository location is defined in a `settings.xml`, this location will be used. The `settings.xml` in `USER_HOME/.m2` takes precedence over the `settings.xml` in `M2_HOME/conf`. If no `settings.xml` is available, Gradle uses the default location `USER_HOME/.m2/`repository.

### Install all spring-\* jars into your local Maven cache
```
    ./gradlew install -x javadoc
```

### Compile and test; build all jars, distribution zips, and docs
```
    ./gradlew build
```

## 导入idea

_Within your locally cloned spring-framework working directory:_

1. Pre-compile `spring-oxm` with `./gradlew cleanIdea :spring-oxm:compileTestJava`
2. Import into IDEA (File->import project->import from external model->Gradle)
3. Set the Project JDK as appropriate (1.8+)
4. Exclude the `spring-aspects` module (Go to File->Project Structure->Modules)
5. Code away


