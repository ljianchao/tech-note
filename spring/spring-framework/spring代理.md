
# spring代理

spring支持两种代理方式：

- jdk提供的创建基于接口的代理
- CGLib提供的生成基于类的代理

## spring依赖注入时代理

当一个较长生命期的bean（如单例bean）依赖一个较短生命期的bean（如会话bean）时，Spring容器会注入一个到该短生命期bean的代理。这个代理会暴露与原bean相同的方法。当调用其方法时，代理会对其进行懒解析并将调用委托给会话作用域内真正的bean实例。

代码示例：

```java
    @Component
    @Scope(value = WebApplicationContext.SCOPE_SESSION, proxyMode = ScopedProxyMode.TARGET_CLASS)
    public class SessionScope {
    }
```

## AOP代理

在AOP编程中，有一个织入（Weaving）的过程，就是把切面应用到目标对象并创建新的代理对象的过程。Spring AOP会在应用运行的某个时刻将切面织入，在织入切面时，AOP容器会为目标对象动态地创建一个代理对象。

AspectJ自动代理会为使用@Aspect注解的bean创建一个代理，这个代理会围绕着所有该切面的切点所匹配的bean。

启用自动代理功能：

- 编程时如果使用JavaConfig的话，必须在配置类的类级别上通过使用`@EnableAspectJAutoProxy`注解启用自动代理功能。
- 编程时如果使用XML来装配bean的话，必须使用Spring aop命令空间中的`<aop:aspectj-autoproxy />`元素启用自动代理功能。