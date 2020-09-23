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

自定义`UserDetailsService`接口实现：

```java
/**
 * 自定义用户服务
 *
 */
@Service
public class UserService implements UserDetailsService {

    private PasswordEncoder passwordEncoder = new BCryptPasswordEncoder();

    private StudentService service;

    public UserService(StudentService service) {
        this.service = service;
    }

    @Override
    public UserDetails loadUserByUsername(String username) throws UsernameNotFoundException {
        Student student = service.findOneByName(username);

        if (student != null) {
            List<GrantedAuthority> authorities = new ArrayList<>();

            // 创建权限列表
            authorities.add(new SimpleGrantedAuthority("ROLE_STUDENT"));

            // 返回User
            return new User(
                    student.getName(),
                    passwordEncoder.encode("123456"),  // 设置密码转码器
                    authorities);
        }

        throw new UsernameNotFoundException("User " + username + " not found.");
    }
}
```

启用用户自定义服务：

```java
/**
 * 启用Web安全性
 */
@Configuration
@EnableWebSecurity
public class SecurityConfig extends WebSecurityConfigurerAdapter {


    @Autowired
    private StudentService studentService;

    /**
     * 设置用户存储
     *
     * @param auth
     * @throws Exception
     */
    @Override
    protected void configure(AuthenticationManagerBuilder auth) throws Exception {

        // 使用自定义的用户服务
        auth.userDetailsService(new UserService(studentService))
                .passwordEncoder(new BCryptPasswordEncoder());  // 设置密码转码器
    }
}
```

### 拦截请求

对每个请求进行细粒度安全性控制的关键在于重载`configure(HttpSecurity)`方法。

```java
    /**
     * 设置请求拦截
     *
     * @param http
     * @throws Exception
     */
    @Override
    protected void configure(HttpSecurity http) throws Exception {
        // 这些规则会按照给定的顺序发挥作用
        // 将最为具体的请求路径放在前面，
        // 而最不具体的路径（如anyRequest()）放在最后面
        http.authorizeRequests()
                .antMatchers("/student/register").permitAll()
                // 设置不拦截的请求
                .antMatchers("/", "/student/list").permitAll()
                // 使用SpEL表达式进行同一路径的多重控制
                .antMatchers("/student/list2").access("hasRole('ROLE_STUDENT') and hasIpAddress('192.168.1.2')")
                // 设置具有"ROLE_STUDENT"权限才可以访问
                // hasAuthority("ROLE_STUDENT")可以使用hasRole("STUDENT")替代，它会自动给使用“ROLE_”前缀
                .antMatchers("/api/student/**").hasAuthority("ROLE_STUDENT")
                // 拦截其他所有请求
                .anyRequest().authenticated();
//            .and()
//            // 强制通道的安全性，并自动将请求重定向到HTTPS上
//            .requiresChannel()
//                .antMatchers("/student/register").requiresSecure();

        // 开启表单登录，否则会直接返回403状态页面
        http.formLogin();
        // 开启basic认证
        http.httpBasic();
        // 禁用CSRF（跨站请求伪造）防护功能
        http.csrf().disable();
    }
```

`antMatchers()`方法中设定的的路径支持Ant风格的通配符，可以指定多个路径。`regexMatchers`方法则能够接受正则表达式来定义请求路径。

我们还通过`authenticated()`和`permitAll()`来定义如何保护路径。`authenticated()`要求在执行该请求时，必须已经登录了应用。如果用户没有认证的话，Spring Security的Filter将会捕获该请求，并将用户重定向到应用的登录页面。

用于定义如何保护路径的配置方法包括：

- access(String)：如果给定的SpEL表达式计算结果为true，就允许访问；
- anonymous()：允许匿名用户访问；
- authenticated()：允许认证过的用户访问；
- denyAll()：无条件拒绝所有访问；
- fullyAuthenticated()：如果用户是完整认证的话（不是通过Remember-me功能认证的），就允许访问；
- hasAnyAuthority(String ...)：如果用户具备给定权限中的某一个的话，就允许访问；
- hasAnyRole(String ...)：如果用户具备给定角色中的某一个的话，就允许访问；
- hasAuthority(String)：如果用户具备给定权限的话，就允许访问；
- hasIpAddress(String)：如果请求来自给定IP地址的话，就允许访问；
- hasRole(String)：如果用户具备给定角色的话，就允许访问；
- not()：对其他访问方法的结果求反；
- permitAll()：无条件允许访问；
- rememberMe()：如果用户是通过Remember-me功能认证的，就允许访问。

可以使用任意数量的`antMatchers()`、`regexMatchers`和`anyRequest()`连接起来，以满足Web应用安全规则的需要。但是，我们需要知道，这些规则会按照给定的顺序发挥作用。所有，很重要的一点就是将**最为具体的请求路径放在前面**，而最不具体的路径（如anyRequest()）放在最后面。

#### 使用Spring表达式（SpEL）进行安全保护

要实现在同一路径下的多种访问控制，我们可以借助`access()`方法，将SpEL作为声明访问限制的一种方式。

```java
    http.authorizeRequests()
            // 使用SpEL表达式进行同一路径的多重控制
            .antMatchers("/student/list2").access("hasRole('ROLE_STUDENT') and hasIpAddress('192.168.1.2')")
            .anyRequest().authenticated();  // 拦截其他所有请求
```

Spring Security通过一些安全性相关的表达式扩展了Spring表达式语言，以下列出：

- authentication：用户的认证对象；
- denyAll：结果始终为false；
- hasAnyRole(list of roles)：如果用户被授予了列表中任意的指定角色，结果为true；
- hasRole(role)：如果用户被授予了指定角色，结果为true；
- hasIpAddress(IP Address)：如果请求来自指定IP的话，结果为true；
- isAnonymous()：如果当前用户为匿名用户，结果为true；
- isAuthenticated()：如果当前用户进行了认证的话，结果为true；
- isFullyAuthenticated()：如果用户是完整认证的话（不是通过Remember-me功能认证的），结果为true；
- isRememberMe()：如果用户是通过Remember-me功能认证的，结果为true；
- permitAll：结果始终为true；
- pricipal：用户的principal对象

#### 强制通道的安全性

借用`requiresChannel()`方法能够为各种URL模式声明所要求的通道。

```java
    http.authorizeRequests()
            .antMatchers("/student/register").permitAll()
            // 拦截其他所有请求
            .anyRequest().authenticated()
        .and()
        // 强制通道的安全性，并自动将请求重定向到HTTPS上
        .requiresChannel()
            .antMatchers("/student/register").requiresSecure();
```

#### 防止跨站请求伪造

**跨站请求伪造（cross-site request forgery，CSRF）**，简单来讲，如果一个站点欺骗用户提交请求到其他服务器的话，就会发生CSRF攻击，这可能会带来消极的后果。

从Spring Security 3.2开始，**默认就会启用CSRF防护**。

Spring Security通过一个**同步token**的方式来实现CSRF防护的功能。它将会**拦截状态变化的请求**（例如，非GET、HEAD、OPTIONS和TRACE的请求）并检查CSRF token。如果请求中不包含CSRF token的话，或者token不能与服务器端的token相匹配，请求将会失败，并抛出`CsrfException`异常。

这意味着在你的应用中，所有的表单必须在一个“_csrf”域中提交token，而且这个token必须要与服务器端计算并存储的token一致，这样的话当表单提交的时候，才能进行匹配。

处理CSRF的另外一种方式就是根本不去处理它。我们可以在配置中通过调用`csrf().disable()`禁用Spring Security的CSRF防护功能：

```java
    // 禁用CSRF（跨站请求伪造）防护功能
    http.csrf().disable();
```

### 用户认证

启用默认的登录页，可以在`configure(HttpSecurity)`方法中调用`formLogin()`方法：

```java
    // Web项目开启表单登录，否则会直接返回403状态页面
    http.formLogin();
```

一、添加自定义的登录页

在web项目中，可以添加自定义的登录页。

二、启用HTTP Basic认证

在RESTful API项目中，通常采用HTTP Basic认证完成安全性验证。HTTP Basic认证（HTTP Basic Authentication）会直接通过HTTP请求本身，对要访问应用程序的用户进行认证。当在Web浏览器中使用时，它将向用户弹出一个简单的模态对话框。

但这只是Web浏览器的显示方式。本质上，这是一个HTTP 401响应（Status:401 Unauthorized），表明必须要在请求中包含一个用户名和密码。在REST客户端向它使用的服务进行认证的场景中，这种方式比较合适。

启用HTTP Basic认证，可以在`configure(HttpSecurity)`方法中调用`httpBasic()`方法：

```java
    // RESTful API项目开启basic认证
    http.httpBasic();
```

三、启用Remember-me功能

许多站点提供了Remember-me功能，用户只要登录过一次，应用就会记住该用户，当再次回到应用的时候你就不需要登录了。

默认情况下，这个功能是通过在**cookie**中存储一个**token**完成的，这个token最多两周内有效。存储在cookie中的token包含用户名、密码、过期时间和一个私钥-在写入cookie前都进行了MD5哈希。默认情况下的，私钥名为**SpringSecured**。

启用Remember-me功能，可以在`configure(HttpSecurity)`方法中调用`rememberMe()`方法：

```java
    // 启用Remember-me功能
    http.rememberMe()
            .tokenValiditySeconds(2419200)  // 设置过期时间
            .key("studentKey");  // 设置私钥
```

四、退出

退出功能是通过Servlet容器中的Filter实现的（默认情况下），这个Filter会拦截针对“/logout”的请求。因此，为应用添加退出功能只需要添加"/logout"的链接即可。点击该链接会对"/logout"进行请求，这个请求会被Spring Security的`LogoutFilter`所处理。用户会退出应用，所有的Remember-me token都会被清除掉。

启用退出功能，可以在`configure(HttpSecurity)`方法中调用`logout()`方法：

```java
    // 退出设置
    http.logout()
            .logoutSuccessUrl("/")  // 设置退出后重定向的页面
            .logoutUrl("/logout");  // 设置LogoutFilter拦截的路径
```
