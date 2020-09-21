# spring-security

Spring Security是一种基于**Spring AOP**和**Servlet规范中的Filter**实现的安全框架。

## Spring Security简介

Spring Security是为基于Spring的应用程序提供**声明式**安全保护的安全性框架。Spring Security提供了完整的安全性解决方案，它能够在**Web请求级别**和**方法调用级别**处理身份认证和授权。

Spring Security从两个角度来解决安全性问题

- 使用Servlet规范中的Filter保护Web请求并限制URL级别的访问；

- 使用Spring AOP保护方法调用——借助于对象代理和使用通知，能够确保只有具备适当权限的用户才能访问安全保护的方法。

### 理解Spring Security模块

Spring Security3.2分为11个模块：

- ACL：支持通过访问控制列表（access control list，ACL）为**域对象**提供安全性；
- 切面（Aspect）：一个很小的模块，当使用Spring Security注解时，会使用基于**AspectJ**的切面，而不是使用标准的Spring AOP；
- CAS客户端（CAS Client）：提供于Jasig的中心认证服务（Central Authentication Service，CAS）进行集成的功能；
- 配置（Configuration）：包含通过XML和Java配置Spring Security的功能支持；
- 核心（core）：提供Spring Security的基本库；
- 加密（Cryptography）：提供了加密和密码编码的功能；
- LDAP：支持基于LDAP进行认证；
- OpenID：支持使用OpenID进行集中式认证；
- Remoting：提供了对Spring Remoting的支持；
- 标签库（Tag Library）：Spring Security的JSP标签库；
- Web：提供了Spring Security基于Filter的Web安全性支持。

应用程序的类路径下至少要包含**Core**和**Configuration**这两个模块。Spring Security经常被用于保护Web应用，所有我们还需要添加Web模块。

pom配置：

```xml
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/maven-v4_0_0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.demo</groupId>
    <artifactId>demo-spring-mvc</artifactId>
    <packaging>war</packaging>
    <version>1.0-SNAPSHOT</version>
    <name>demo-spring-mvc</name>
    <url>http://maven.apache.org</url>

    <properties>
        <spring.version>5.2.7.RELEASE</spring.version>
        <spring.security.version>5.4.0</spring.security.version>
    </properties>

    <dependencyManagement>
        <dependencies>
            <!-- Spring -->
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-framework-bom</artifactId>
                <version>${spring.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
            <!-- Spring Security -->
            <dependency>
                <groupId>org.springframework.security</groupId>
                <artifactId>spring-security-bom</artifactId>
                <version>${spring.security.version}</version>
                <type>pom</type>
                <scope>import</scope>
            </dependency>
            <!-- Servlet api -->
            <dependency>
                <groupId>javax.servlet</groupId>
                <artifactId>javax.servlet-api</artifactId>
                <version>3.1.0</version>
            </dependency>
            <!-- jstl标签 -->
            <dependency>
                <groupId>javax.servlet</groupId>
                <artifactId>jstl</artifactId>
                <version>1.2</version>
            </dependency>
            <!-- HttpMessageConverter -->
            <!-- json -->
            <dependency>
                <groupId>com.fasterxml.jackson.core</groupId>
                <artifactId>jackson-databind</artifactId>
                <version>2.11.2</version>
            </dependency>
            <!-- xml -->
            <dependency>
                <groupId>com.fasterxml.jackson.dataformat</groupId>
                <artifactId>jackson-dataformat-xml</artifactId>
                <version>2.11.2</version>
            </dependency>
            <!-- Java校验API，JSR-303 -->
            <dependency>
                <groupId>org.hibernate.validator</groupId>
                <artifactId>hibernate-validator</artifactId>
                <version>6.1.5.Final</version>
            </dependency>
            <dependency>
                <groupId>junit</groupId>
                <artifactId>junit</artifactId>
                <version>3.8.1</version>
                <scope>test</scope>
            </dependency>
        </dependencies>
    </dependencyManagement>
    <dependencies>
        <!-- spring web -->
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-web</artifactId>
            <scope>compile</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework</groupId>
            <artifactId>spring-webmvc</artifactId>
            <scope>compile</scope>
        </dependency>
        <!-- spring security -->
        <dependency>
            <groupId>org.springframework.security</groupId>
            <artifactId>spring-security-config</artifactId>
            <scope>compile</scope>
        </dependency>
        <dependency>
            <groupId>org.springframework.security</groupId>
            <artifactId>spring-security-web</artifactId>
            <scope>compile</scope>
        </dependency>
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>javax.servlet-api</artifactId>
            <scope>provided</scope>
        </dependency>
        <!-- jstl标签 -->
        <dependency>
            <groupId>javax.servlet</groupId>
            <artifactId>jstl</artifactId>
        </dependency>
        <!-- HttpMessageConverter -->
        <!-- json -->
        <dependency>
            <groupId>com.fasterxml.jackson.core</groupId>
            <artifactId>jackson-databind</artifactId>
        </dependency>
        <!-- xml -->
        <dependency>
            <groupId>com.fasterxml.jackson.dataformat</groupId>
            <artifactId>jackson-dataformat-xml</artifactId>
        </dependency>
        <!-- Java校验API，JSR-303 -->
        <dependency>
            <groupId>org.hibernate.validator</groupId>
            <artifactId>hibernate-validator</artifactId>
        </dependency>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <scope>test</scope>
        </dependency>
    </dependencies>

    <build>
        <!-- 打包后的文件名 -->
        <finalName>${artifactId}</finalName>
        <resources>
            <!-- 指定默认配置资源 -->
            <resource>
                <directory>src/main/resources</directory>
                <filtering>true</filtering>
            </resource>
        </resources>
        <plugins>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-compiler-plugin</artifactId>
                <configuration>
                    <!-- 配置JDK8，避免IDEA的lambda表达式报红 -->
                    <source>8</source>
                    <target>8</target>
                    <encoding>UTF-8</encoding>
                    <compilerArgs>-Xlint:deprecation,-Xlint:unchecked</compilerArgs>
                </configuration>

            </plugin>
            <plugin>
                <groupId>org.apache.maven.plugins</groupId>
                <artifactId>maven-war-plugin</artifactId>
            </plugin>
        </plugins>
    </build>
</project>

```

### 过滤Web请求

Spring Security借助**一系列Servlet Filter**来提供各种安全性功能，但是并不意味着我们需要在`web.xml`或`WebApplicationInitializer`中配置多个Filter。我们可以借助于Spring的小技巧，只需要配置一个Filter（DelegatingFilterProxy）就可以了。

#### 配置DelegatingFilterProxy

`DelegatingFilterProxy`是一个特殊的Servlet Filter，它本身所做的工作并不多。只是将工作委托给一个`javax.servlet.Filter`实现类，这个实现类作为一个`<bean>`注册在Spring的应用上下文中，即位于Servlet上下文中的`DelegatingFilterProxy`把Filter的处理逻辑委托给了Spring应用上下中所定义的一个`代理Filter bean`。

基于`web.xml`的配置，假设Spring容器中存在一个名为`springSecurityFilterChain`的`Filter bean`，`DelegatingFilterProxy`会将过滤逻辑委托给它。

```xml
  <filter>
    <filter-name>springSecurityFilterChain</filter-name>
    <filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
  </filter>
```

基于Java配置方式，借助于`WebApplicationInitializer`，创建类`AbstractSecurityWebApplicationInitializer`扩展的一个新类：

```java
/**
 * 默认配置DelegatingFilterProxy
 *
 * DelegatingFilterProxy会拦截发往应用中的请求，
 * 并将请求委托给ID为`springSecurityFilterChain`（固定的ID）的bean
 *
 * `springSecurityFilterChain`本身是另一个特殊的Filter，
 * 它也被称为`FilterChainProxy`。它可以链接任意一个或多个其他的Filter。
 * 我们不需要显示声明`springSecurityFilterChain`以及它所链接在一起的
 * 其他Filter。当我们启用Web安全性(使用`@EnableWebSecurity`注解)的时候，
 * 会自动创建这些Filter。
 *
 */
public class SecurityWebInitializer extends AbstractSecurityWebApplicationInitializer {
}
```

不管我们通过`web.xml`还是通过`AbstractSecurityWebApplicationInitializer`的子类来配置`DelegatingFilterProxy`，它都会拦截发往应用中的请求，并将请求委托给ID为`springSecurityFilterChain`（固定的ID）的bean。

`springSecurityFilterChain`本身是另一个特殊的Filter，它也被称为`FilterChainProxy`。它可以链接任意一个或多个其他的Filter。我们不需要显示声明`springSecurityFilterChain`以及它所链接在一起的其他Filter。当我们启用Web安全性(使用`@EnableWebSecurity`注解)的时候，会自动创建这些Filter。

#### 编写安全性配置

`@EnableWebSecurity`注解将会启用Web安全功能。但它本身并没有什么用处，Spring Security必须配置在一个实现`WebSecurityConfigurer`的bean中，或者扩展`WebSecurityConfigurerAdapter`类。

```java
/**
 * 启用Web安全性
 */
@Configuration
@EnableWebSecurity
public class SecurityConfig extends WebSecurityConfigurerAdapter {
}
```

通过重载`WebSecurityConfigurerAdapter`的三个`configure()`方法来配置Web安全性，这个过程中会使用传递进来的参数设置行为：

- configure(WebSecurity)：通过重载，配置Spring Security的Filter链；
- configure(HttpSecurity)：通过重载，配置如何通过拦截器保护请求（默认要求所有进入应用的HTTP请求都有进行认证）；
- configure(AuthenticationManagerBuilder)：通过重载，配置user-detail服务（默认没有用户存储支撑认证过程，实际上等于没有用户，不能进行登录）。

为了让Spring Security满足我们应用的需求，还需要再添加一点配置。具体来讲，我们需要：

- 配置用户存储；
- 指定哪些请求需要认证，哪些请求不需要认证，以及所需要的权限；
- 提供一个自定义的登录页面，替代原来简单的默认登录页。

### 选择查询用户详细信息的服务

Spring Security非常灵活，能够基于各种数据存储来认证用户。它内置了多种常见的用户存储常见，如内存、关系型数据库以及LDAP。

一、使用基于内存的用户存储

通过重载`configure(AuthenticationManagerBuilder)`方法，设置`AuthenticationManagerBuilder.`方法启用、配置并任意填充基于内存的用户存储。

```java
    /**
     * 设置用户存储
     *
     * @param auth
     * @throws Exception
     */
    @Override
    protected void configure(AuthenticationManagerBuilder auth) throws Exception {

        // 使用基于内存的用户存储
        auth.inMemoryAuthentication()
                .passwordEncoder(new BCryptPasswordEncoder())
                .withUser("user").password(new BCryptPasswordEncoder().encode("123456")).roles("USER").and()  // 添加"user"用户
                .withUser("admin").password(new BCryptPasswordEncoder().encode("123456")).roles("USER", "ADMIN");  // 添加"admin"用户
    }
```

二、基于数据库表进行认证

用户数据通常会存储在关系型数据库中，并通过JDBC进行访问。为了配置Spring Security使用以JDBC为支撑的用户存储，我们可以使用`jdbcAuthentication()`方法。

三、基于LDAP进行认证

为了让Spring Security使用基于LDAP的认证，我们可以使用`ldapAuthentication()`方法。

四、配置自定义的用户服务

假设我们需要认证的用户存储在非关系型数据库中，如Mongo或Neo4j，在这种情况下，我们需要提供一个自定义的`UserDetailsService`接口实现。

`UserDetailsService`接口定义：

```java
public interface UserDetailsService {

	/**
	 * Locates the user based on the username. In the actual implementation, the search
	 * may possibly be case sensitive, or case insensitive depending on how the
	 * implementation instance is configured. In this case, the <code>UserDetails</code>
	 * object that comes back may have a username that is of a different case than what
	 * was actually requested..
	 * @param username the username identifying the user whose data is required.
	 * @return a fully populated user record (never <code>null</code>)
	 * @throws UsernameNotFoundException if the user could not be found or the user has no
	 * GrantedAuthority
	 */
	UserDetails loadUserByUsername(String username) throws UsernameNotFoundException;

}
```

### 拦截请求

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
