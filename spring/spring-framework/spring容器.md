
# spring容器

spring自带了多个容器实现，可以归为两种不同的类型：

- `BeanFactory`是最简单的容器，提供基本的DI支持。
- `ApplicationContext`基于`BeanFactory`构建，并提供应用框架级别的服务。

spring自带的几种常用的应用上下文`ApplicationContext`如下：

- `AnnotationConfigApplicationContext`：从一个或多个基于java的配置类中加载spring应用上下文。
- `AnnotationConfigWebApplicationContext`：从一个或多个基于java的配置类中加载spring web应用上下文。
- `ClassPathXmlApplicationContext`：从类路径下的一个或多个xml配置文件中加载上下文定义，把应用上下文的定义文件作为类资源。
- `FileSystemXmlApplicationContext`：从文件系统下的一个或多个xml配置文件种加载上下文定义。
- `XmlWebApplicationContext`：从web应用下的一个或多个xml配置文件中加载上下文定义。

## spring容器中bean的生命周期

spring容器中bean的生命周期相对比较复杂，下图展示了bean装载到spring应用上下文中的一个典型的生命周期过程：

![spring容器中bean的生命周期](resources/bean_life.jpg)

生命周期的详细描述如下：

- Spring对bean进行实例化，即执行构造函数;
- Spring将值和**bean的引用注入**到bean对应的属性中； 
- 如果bean实现了`BeanNameAware`接口，Spring将bean的ID传递给`setBean-Name()`方 法； 
- 如果bean实现了`BeanFactoryAware`接口，Spring将调用`setBeanFactory()`方法，将 `BeanFactory`容器实例传入；
- 如果bean实现了`ApplicationContextAware`接口，Spring将调 用s`etApplicationContext()方`法，将bean所在的应用上下文的引用传入进来；
- 如果bean实现了`BeanPostProcessor`接口，Spring将调用它们的`postProcessBeforeInitialization()`方法；
- 如果bean实现了`InitializingBean`接口，Spring将调用它们的`afterPropertiesSet()`方法。类似地，如果bean使用`init-method`声明了初始化方法，该方 法也会被调用；
- 如果bean实现了`BeanPostProcessor`接口，Spring将调用它们的`postProcessAfterInitialization()`方法；
- 此时，bean已经准备就绪，可以被应用程序使用了，它们将一直驻留在应用上下文中， 直到该应用上下文被销毁；
- 如果bean实现了`DisposableBean`接口，Spring将调用它的`destroy()`接口方法。同 样，如果bean使用`destroy-method`声明了销毁方法，该方法也会被调用。

## 装配Bean

Spring提供了三种主要的装配机制：

- 在XML中进行显示配置
- 在Java代码中进行显示配置
- 隐式的bean发现机制和自动装配

### 自动化装配bean

Spring从两个角度来实现自动化装配：

- 组件扫描（component scanning）：Spring会自动发现应用上下文中创建的bean
- 自动装配（autowiring）：Spring自动满足bean之间的依赖

开启组件扫描的两种方式：

- XML中使用以下配置：
    ```xml
        <context:component-scan base-package="cn.demo.spring.service"/>
    ```

- java中使用以下配置：
    ```java
        @Configuration
        @ComponentScan(basePackages = {"cn.demo.spring.service"})
        public class MainConfiguration {

        }
    ```