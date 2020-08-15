# Spring Boot in Action

## 简介

Spring Boot产生的背景

- Spring的组件扫描减少了配置量，Java配置是一种类型安全的可重构配置方式，让配置看上去简洁不少，但Spring还是需要不少配置。

- 项目的依赖管理是个棘手的难题，不仅需要决定要用哪些库，还要知道这些库的哪个版本和其他库不会有冲突。

### Spring Boot精要

Spring Boot最重要的四个核心：

- 自动配置：针对很多Spring应用程序常见的应用功能，Spring Boot能自动提供相关配置（解决配置问题）。利用Spring 4的条件化配置特性，以及Maven和Gradle提供的传递依赖解析，以此实现Spring应用程序
- 起步依赖：告诉Spring Boot需要什么功能，它就能引入需要的库（解决项目的依赖管理）。起步依赖其实就是特殊的Maven依赖和Gradle依赖，利用了**传递依赖解析**，把常用库聚合在一起，组成了几个为特定功能而定制的依赖。
- 命令行界面：Spring Boot的可选特性，借此你只需写代码就能完成完整的应用程序。Spring Boot CLI利用了起步依赖和自动配置，让你专注与代码本身。
- Actuator：提供在运行时检视应用程序内部情况的能力。

## 实践

### 启动引导Spring

启动引导类示例：

```java
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
public class ReadingListApplication {

    public static void main(String[] args) {
        SpringApplication.run(ReadingListApplication.class, args);
    }
}
```

启动引导类（bootstrap class）在Spring Boot应用程序里有两个作用：配置和启动引导。

`@SpringBootApplication`注解开启了Spring的组件扫描和Spring Boot的自动配置功能。实际上`@SpringBootApplication`将三个有用的注解组合在了一起。

- Spring的`@Configuration`：标明该类使用Spring基于Java的配置。
- Spring的`@ComponentScan`：启用组件扫描。
- Spring Boot的`@EnableAutoConfiguration`：开启了Spring Boot自动配置的魔力。

具体定义如下：

```java
@Target(ElementType.TYPE)
@Retention(RetentionPolicy.RUNTIME)
@Documented
@Inherited
@SpringBootConfiguration
@EnableAutoConfiguration
@ComponentScan(excludeFilters = { @Filter(type = FilterType.CUSTOM, classes = TypeExcludeFilter.class),
		@Filter(type = FilterType.CUSTOM, classes = AutoConfigurationExcludeFilter.class) })
public @interface SpringBootApplication {

	@AliasFor(annotation = EnableAutoConfiguration.class)
	Class<?>[] exclude() default {};

	@AliasFor(annotation = EnableAutoConfiguration.class)
	String[] excludeName() default {};

	@AliasFor(annotation = ComponentScan.class, attribute = "basePackages")
	String[] scanBasePackages() default {};

	@AliasFor(annotation = ComponentScan.class, attribute = "basePackageClasses")
	Class<?>[] scanBasePackageClasses() default {};

	@AliasFor(annotation = ComponentScan.class, attribute = "nameGenerator")
	Class<? extends BeanNameGenerator> nameGenerator() default BeanNameGenerator.class;

	@AliasFor(annotation = Configuration.class)
	boolean proxyBeanMethods() default true;
}
```

`main`方法让你可以在命令行里把应用程序当作一个可执行JAR文件了运行。这里向`SpringApplication.run()`方法传递了一个`ReadingListApplication`类的引用，还有命令行参数，通过这些东西启动应用程序。

### 使用起步依赖

起步依赖本质上是一个Maven项目对象模型（Project Object Model，POM），定义了对其他库的传递依赖，这些东西加在一起即支持某项功能。很多起步依赖的命名都暗示了它们提供的某种或某类功能。

### 使用自动配置

Spring Boot提供的条件化注解：

<table>
    <tr>
        <td>条件化注解</td>
        <td>配置生效条件</td>
    </tr>
    <tr>
        <td>@ConditionalOnBean</td>
        <td>配置了某个特定的Bean</td>
    </tr>
    <tr>
        <td>@ConditionalOnMissingBean</td>
        <td>没有配置特定的Bean</td>
    </tr>
    <tr>
        <td>@ConditionalOnClass</td>
        <td>Classpath里有指定的类</td>
    </tr>
    <tr>
        <td>@ConditionalOnMissingClass</td>
        <td>Classpath里缺少指定的类</td>
    </tr>
    <tr>
        <td>@ConditionalOnExpression</td>
        <td>给定的Spring Expression Language(SpEL)表达式计算结果为true</td>
    </tr>
    <tr>
        <td>@ConditionalOnJava</td>
        <td>Java的版本匹配特定值或者一个范围值</td>
    </tr>
    <tr>
        <td>@ConditionalOnJndi</td>
        <td>参数中指定的JNDI位置必须存在一个，如果没有给参数，则要有JNDI InitialContext</td>
    </tr>
    <tr>
        <td>@ConditionalOnProperty</td>
        <td>指定的配置属性要有一个明确的值</td>
    </tr>
    <tr>
        <td>@ConditionalOnResource</td>
        <td>Classpath里有指定的资源</td>
    </tr>
    <tr>
        <td>@ConditionalOnWebApplication</td>
        <td>这是一个Web应用程序</td>
    </tr>
    <tr>
        <td>@ConditionalOnNotWebApplication</td>
        <td>这不是一个Web应用程序</td>
    </tr>
</table>

## 自定义配置

有两种方式可以影响自动配置，**使用显示配置进行覆盖**和**使用属性进行精细化配置**。

### 覆盖Spring Boot自动配置

Spring Boot自动配置自带了很多配置类，每一个都能运用在你的应用程序里。它们都使用了Spring 4.0的**条件化配置**，可以在运行时判断这个配置是该被运用，还是该被忽略。

### 通过属性文件外置配置

Spring Boot自动配置的Bean提供了很多用于微调的属性。当你调整设置时，只要在**环境变量、Java系统属性、JNDI（Java Naming and Directory Interface）、命令行参数或者属性文件**里进行指定就好了。

Spring Boot能从多种属性源获得属性，包括如下几处（列表按照优先级排序，任何在高优先级属性源里设置的属性都会覆盖低优先级的相同属性。例如，命令行参数会覆盖其他属性源里的属性）。

- 命令行参数。
- `java:comp/env`里的JNDI属性
- JVM系统属性
- 操作系统环境变量
- 随机生成的带`random.*`前缀的属性（在设置其他属性时，可以引用它们，比如`${random.long}`）
- 应用程序以外的`application.properties`或者`application.yml`文件
- 打包在应用程序内的`application.properties`或者`application.yml`文件
- 通过`@PropertySource标注的属性源`
- 默认属性

`application.properties`和`application.yml`文件能放在以下四个位置（按照优先级从高到低排序）

- 外置，在相对于应用程序运行目录的`/config`子目录里。
- 外置，在应用程序运行的目录里。
- 内置，在`config`包内。
- 内置，在`Classpath`根目录。

此外在同一优先级位置同时有`application.properties`和`application.yml`，那么`application.yml`里的属性会覆盖`application.properties`里的属性。

#### 应用程序Bean的配置外置

在一个类里上配置`@ConfigurationProperties`注解来实现收集属性的功能。

Note:
> 从技术上来说，`@ConfigurationProperties`注解不会生效，除非先向Spring配置类添加`@EnableConfigurationProperties`注解。但通常无需这么做，因为Spring Boot自动配置后面的全部配置类都已经加上了`@EnableConfigurationProperties`注解。因此，除非你完全不使用自动配置（那怎么可能？），否则就无需显示地添加`@EnableConfigurationProperties`注解。
