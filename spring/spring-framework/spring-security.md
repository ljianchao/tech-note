
# 简介

Spring Security从两个角度来解决安全性问题

- 使用Servlet规范中的Filter保护Web请求并限制URL级别的访问。

- 使用Spring AOP保护方法调用——借助于对象代理和使用通知，能够确保只有具备适当权限的用户才能访问安全保护的方法

## Spring Security模块

应用程序的类路径下至少要包含Core和Configuration这两个模块，需要添加Web模块保护Web应用

## 过滤Web请求

### 配置DelegatingFilterProxy

DelegatingFilterProxy是一个特殊的ServletFilter，它本身所做的工作并不多。只是将工作委托给一个javax.servlet.Filter实现类，这个实现类作为一个<bean>注册在Spring的应用上下文中。

web.xml配置，spring容器中存在一个名为`springSecurityFilterChain`的Filter bean，DelegatingFilterProxy会将过滤逻辑委托给它。

```xml
  <filter>
    <filter-name>springSecurityFilterChain</filter-name>
    <filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
  </filter>
```

java配置方式，创建类`AbstractSecurityWebApplicationInitializer`扩展的新类`AbstractSecurityWebApplicationInitializer`实现了`WebApplicationInitializer`，因此Spring会发现它，并用它在Web容器中注册`DelegatingFilterProxy`

```java
import org.springframework.security.web.context.AbstractSecurityWebApplicationInitializer;

public class SecurityWebInitializer extends AbstractSecurityWebApplicationInitializer {
}
```

### 编写安全性配置

#### 启用Web安全性功能

@EnableWebSecurity将会启用Web安全功能。但它本身并没有什么用处，Spring Security必须配置在一个实现WebSecurityConfigurer的bean中，或者（简单起见）扩展WebSecurityConfigurerAdapter。

@EnableWebMvcSecurity可以用在spring mvc项目中。

```java
@Configuration
@EnableWebSecurity
public class SecurityBaseConfig extends WebSecurityConfigurerAdapter {
}
```

跨站请求伪造（cross-site request forgery，CSRF）

#### 配置web安全性

通过重载WebSecurityConfigurerAdapter的三个configure()方法来配置Web安全性

- configure(WebSecurity)：通过重载，配置Spring Security的Filter链
- configure(HttpSecurity)：通过重载，配置如何通过拦截器保护请求
- configure(AuthenticationManagerBuilder)：通过重载，配置user-detail服务

为了让Spring Security满足我们应用的需求，还需要再添加一点配置。具体来讲，我们需要：

- 配置用户存储；
- 指定哪些请求需要认证，哪些请求不需要认证，以及所需要的权限；
- 提供一个自定义的登录页面，替代原来简单的默认登录页。

#### 配置自定义的用户服务

假设我们需要认证的用户存储在非关系型数据库中，如Mongo或Neo4j，在这种情况下，我们需要提供一个自定义的`UserDetailsService`接口实现。

#### 拦截请求

对每个请求进行细粒度安全性控制的关键在于重载configure(HttpSecurity)方法。

#### 防止跨站请求伪造

跨站请求伪造（cross-site request forgery，CSRF），简单来讲，如果一个站点欺骗用户提交请求到其他服务器的话，就会发生CSRF攻击，这可能会带来消极的后果。

从Spring Security 3.2开始，默认就会启用CSRF防护。

Spring Security通过一个同步token的方式来实现CSRF防护的功能。它将会拦截状态变化的请求（例如，非GET、HEAD、OPTIONS和TRACE的请求）并检查CSRF token。如果请求中不包含CSRF token的话，或者token不能与服务器端的token相匹配，请求将会失败，并抛出CsrfException异常。

处理CSRF的另外一种方式就是根本不去处理它。我们可以在配置中通过调用csrf().disable()禁用Spring Security的CSRF防护功能：

```java
@Override
protected void configure(HttpSecurity http) throws Exception {

    http.csrf()
          .disable();
}
```

### 用户认证

- 用户自定义登录页，适用于web项目
- 启用HTTP Basic认证，适用于RESTful API。
- 启用Remember-me功能，默认情况下，这个功能是通过在cookie中存储一个token完成的，这个token最多两周内有效。
- 退出（logout），退出功能是通过Servlet容器中的Filter实现的（默认情况下），这个Filter会拦截针对“/logout”的请求。这个请求会被Spring Security的LogoutFilter所处理。用户会退出应用，所有的Remember-me token都会被清除掉。
