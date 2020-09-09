# Spring Web

## Spring MVC

Spring MVC请求的处理流程：`请求 -> DispatchServlet -> 处理器映射 -> 控制器 -> 模型及逻辑视图名 -> 视图解析器 -> 视图 -> 响应`

- `DispatchServlet`是一个前端控制器，Spring MVC的所有请求都会通过`DispatchServlet`。`DispatchServlet`的任务是将请求发送给Spring MVC控制器（Controller）。
- 处理器映射。`DispatchServlet`会查询一个或多个处理器映射（handler mapping）来确定请求的控制器。处理器映射会根据请求所携带的URL信息来进行决策。
- 控制器是一个用于处理请求的Spring组件。请求到了控制器，会卸载下其负载（用户提交的信息）并耐心等待控制器处理这些信息。控制器完成逻辑处理后，会产生一些信息，需要返回给用户并在浏览器上显示，这些信息被称为模型（model）。控制器最后将模型数据打包，并且标示出用于渲染输出的视图名。接下来将请求连同模型和视图名发送回`DispatchServlet`。
- `DispatchServlet`将会使用视图解析器（view resolver）来将逻辑视图名匹配为一个特定的视图实现，它可能是也可能不是JSP。
- `DispatchServlet`确定由哪个视图渲染结果，进行视图的实现（可能是JSP），请求在这里交付模型数据（model），任务就完成了。视图将使用model渲染输出，这个输出会通过响应对象（Response）传递给客户端。

### Maven搭建Spring MVC项目

搭建项目命令：

```
# 创建java项目
mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring-mvc -Dpackage=com.demo.spring.mvc -DarchetypeArtifactId=maven-archetype-quickstart -DinteractiveMode=false -DarchetypeCatalog=local

# 创建web项目，包含由web-app目录
mvn archetype:generate -DgroupId=com.demo -DartifactId=demo-spring-mvc -Dpackage=com.demo.spring.mvc -DarchetypeArtifactId=maven-archetype-webapp -DinteractiveMode=false -DarchetypeCatalog=local
```

POM文件增加Spring MVC依赖

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

    <dependencyManagement>
        <dependencies>
            <!-- Spring -->
            <dependency>
                <groupId>org.springframework</groupId>
                <artifactId>spring-framework-bom</artifactId>
                <version>5.2.7.RELEASE</version>
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

            <dependency>
                <groupId>junit</groupId>
                <artifactId>junit</artifactId>
                <version>3.8.1</version>
                <scope>test</scope>
            </dependency>
        </dependencies>
    </dependencyManagement>
    <dependencies>
        <!-- spring -->
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
        <!-- Servlet api -->
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
        <!-- Java校验API，JSR-303 -->
        <dependency>
            <groupId>org.hibernate.validator</groupId>
            <artifactId>hibernate-validator</artifactId>
        </dependency>
        <!-- xml -->
        <dependency>
            <groupId>com.fasterxml.jackson.dataformat</groupId>
            <artifactId>jackson-dataformat-xml</artifactId>
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

### 搭建Spring MVC

#### 配置DispatcherServlet

在基于XML进行配置的Spring MVC应用中，可以在`web.xml`文件中配置`DispatcherServlet`。

借助于Servlet 3规范和Spring3.1的功能增强，可以使用Java配置的方式将`DispatcherServlet`配置在Servlet容器中。

在Servlet3.0环境中，容器会在类路径中查找实现`javax.servlet.ServletContainerInitializer`接口的类，如果能发现的话，就会用它来配置Servlet容器。

Spring提供了接口`javax.servlet.ServletContainerInitializer`的实现类`SpringServletContainerInitializer`，在`onStartup`方法中会查找实现接口`WebApplicationInitializer`的类并将配置的任务交给它们来完成。Spring3.2引入了`WebApplicationInitializer`的基础实现类`AbstractAnnotationConfigDispatcherServletInitializer`。我看可以对`AbstractAnnotationConfigDispatcherServletInitializer`基础实现类进行扩展，用来配置Servlet上下文。

```java
/**
 * 初始化配置
 */
public class CustomWebAppInitializer extends AbstractAnnotationConfigDispatcherServletInitializer {

    // 指定ContextLoaderListener配置类
    @Override
    protected Class<?>[] getRootConfigClasses() {
        return new Class[]{RootConfig.class};
    }

    // 指定DispatcherServlet配置类
    @Override
    protected Class<?>[] getServletConfigClasses() {
        return new Class[]{WebConfig.class};
    }

    // 将一个或多个路径映射到DispatcherServlet上
    @Override
    protected String[] getServletMappings() {
        // "/"表示会处理进入应用的所有请求
        return new String[]{"/"};
    }
}
```

##### DispatcherServlet和Servlet监听器（ContextLoaderListener）关系

当`DispatcherServlet`启动的时候，它会创建**Spring应用上下文**，并**加载配置文件**或**配置类中所声明的bean**。`getServletConfigClasses()`方法中，我们要求DispatcherServlet加载应用上下文时，使用定义在`WebConfig`配置类（使用Java配置）中的bean。

在Spring Web应用中，`ContextLoaderListener`会创建另外一个应用上下文。我们希望`DispatcherServlet`加载包含Web组件的bean，如控制器、视图解析器以及处理器映射，而`ContextLoaderListener`要加载应用中的其他bean，这些bean通常是驱动应用后端的中间件和数据层组件。

`AbstractAnnotationConfigDispatcherServletInitializer`会同时创建`DispatcherServlet`和`ContextLoaderListener`。`getServletConfigClasses()`方法返回的带有`@Configuration`注解的类将会用来定义`DispatcherServlet`应用上下文的bean。`getRootConfigClasses()`方法返回的带有`@Configuration`注解的类将会用来配置`ContextLoaderListener`创建的应用上下文中的bean。

#### 启用Spring MVC

在基于XML进行配置的Spring MVC应用中，可以使用`<mvc:annotation-driven>`启用**注解驱动**的Spring MVC，例如`@RequestMapping`。

在基于Java进行配置的Spring MVC应用中，需要进行以下配置

- 配置带有`@EnableWebMvc`注解的类，启用**注解驱动**的Spring MVC。
- 配置视图解析器，Spring默认会使用`BeanNameViewResolver`，这个视图解析器会查找ID与视图名称匹配的bean，并且要查找的bean要实现View接口。
- 启用组件扫描，扫描带有`@Controller`的控制器类，不用在配置类中显示声明控制器。
- 设置`DispatcherServlet`将对静态资源的请求转法到Servlet容器中默认的Servlet上，而不是使用`DispatcherServlet`本身来处理此类请求。扩展`WebMvcConfigurerAdapter`进行实现。

```java
/**
 * Web相关的配置
 */
@Configuration
@EnableWebMvc   // 启用Spring MVC
@ComponentScan("com.demo.spring.mvc.web")  // 启用组件扫描
public class WebConfig extends WebMvcConfigurerAdapter {

    // 配置JSP视图
    @Bean
    public ViewResolver viewResolver() {
        InternalResourceViewResolver resolver =
                new InternalResourceViewResolver();
        resolver.setPrefix("/WEB-INF/views/");
        resolver.setSuffix(".jsp");
        resolver.setExposeContextBeansAsAttributes(true);
        // 设置将视图解析为JstlView实例
        resolver.setViewClass(JstlView.class);
        return resolver;
    }

    // 配置静态资源的处理
    @Override
    public void configureDefaultServletHandling(DefaultServletHandlerConfigurer configurer) {
        configurer.enable();
    }
}

/**
 * 非Web组件的配置
 */
@Configuration
@ComponentScan(basePackages = {"com.demo.spring.mvc"},
        excludeFilters = @ComponentScan.Filter(type = FilterType.ANNOTATION, value = EnableWebMvc.class))
public class RootConfig {
}

```

#### 编写控制器

一、最基本的控制器

```java
@Controller // 声明控制器
public class HomeController {

    @RequestMapping(value = "/", method = RequestMethod.GET)
    public String home() {
        // 返回视图名
        return "home";
    }
}

// 类级别视图控制器
@Controller // 声明控制器
@RequestMapping(value = "/")
public class HomeController {

    @RequestMapping(method = RequestMethod.GET)
    public String index() {
        // 返回视图名
        return "index";
    }
}

```

二、传递模型数据到视图中

```java
@Controller
@RequestMapping("/student")
public class StudentController {

    private static List<Student> studentList;

    static {
        studentList = studentList = new ArrayList<Student>();
        studentList.add(new Student(1, "Zhangsan", 3));
        studentList.add(new Student(2, "Lisi", 4));
        studentList.add(new Student(3, "Wangwu", 5));
    }

    /**
     * 传递模型数据到视图
     * @param model
     * @return
     */
    @RequestMapping(value = "/list", method = RequestMethod.GET)
    public String list(Model model) {
        // 设置模型数据
        model.addAttribute("studentList", studentList);
        // 返回逻辑视图名，逻辑视图名中包含斜线时，这个斜线也会带到资源的路径名中
        // 查找"WEB-INF/views/student/list.jsp"
        return "student/list";
    }

    /**
     * 使用java.util.Map代替Model，设置模型数据
     * @param model
     * @return
     */
    @RequestMapping(value = "/list2", method = RequestMethod.GET)
    public String listWithMap(Map model) {
        model.put("studentList", studentList);
        // 返回逻辑视图名，逻辑视图名中包含斜线时，这个斜线也会带到资源的路径名中
        // 查找"WEB-INF/views/student/list.jsp"
        return "student/list";
    }

    /**
     * 当处理器方法返回对象或集合时，这个值会放到模型中，
     * 模型的key会根据其类型推断得出（studentList）
     *
     * 逻辑视图的名称根据请求路径推断得出（student/list3.jsp）
     * @return
     */
    @RequestMapping(value = "/list3", method = RequestMethod.GET)
    public List<Student> studentList() {
        return studentList;
    }
}
```

视图`list.jsp`内容：

```jsp
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ page session="false" %>
<%@ page isELIgnored="false" %>
<html>
<head>
    <title>The list of Student</title>
</head>
<body>
    <h1>The list of Student</h1>

    <c:forEach items="${studentList}" var="student" >
        <li>
            <div>
                <c:out value="${student.name}" />
            </div>
            <div>
                <span><c:out value="${student.age}" /></span>
            </div>
        </li>
    </c:forEach>
</body>
</html>
```

三、接收请求的输入

Spring MVC允许以多种方式将客户端中的数据传送到控制器的处理器方法中，包括：

- 查询参数（Query Parameter）
- 表单参数（Form Parameter）
- 路径变量（Path Variable）

```java
/**
     * 接收请求的输入
     * 处理查询参数：/student/list3?count=2
     *
     * @param count 请求参数，count参数为null，使用defaultValue的值
     * @return
     */
    @RequestMapping(value = "/list4", method = RequestMethod.GET)
    public List<Student> topList(@RequestParam(value = "count", defaultValue = "1") int count) {
        return studentList.subList(0, count);
    }


    /**
     * 接收请求的输入
     * 处理路径变量：/student/2
     *
     * @param id 路径变量
     * @param model
     * @return
     */
    @RequestMapping(value = "/{id}", method = RequestMethod.GET)
    public String student(@PathVariable("id") int id, Map model) {
        List<Student> list = studentList.stream().filter(student -> (student.getId() == id)).collect(Collectors.toList());
        model.put("student", list.get(0));

        // 返回逻辑视图名
        return "student/student";
    }

    @RequestMapping(value = "/register", method = RequestMethod.GET)
    public String showRegisterForm() {
        return "student/registerForm";
    }

    /**
     * 接收请求的输入
     * 处理表单参数
     *
     * @param student
     * @return
     */
    @RequestMapping(value = "/register", method = RequestMethod.POST)
    public String processRegisterForm(Student student) {
        studentList().add(student);
        // "redirect:"前缀重定向，"forward:"前缀前往指定的URL路径
        return "redirect: /student/" + student.getId();
    }
```

视图`student.jsp`内容：

```jsp
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ page session="false" %>
<%@ page isELIgnored="false" %>
<html>
<head>
    <title>The list of Student</title>
</head>
<body>
    <h1>Welcome: </h1>
    <div>
        Id: <c:out value="${student.id}" />
    </div>
    <div>
        Name: <c:out value="${student.name}" />
    </div>
    <div>
        Age: <span><c:out value="${student.age}" /></span>
    </div>
</body>
</html>
```

视图`registerForm.jsp`内容：

```jsp
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c" %>
<%@ page session="false" %>
<%@ page isELIgnored="false" %>
<html>
<head>
    <title>The list of Student</title>
</head>
<body>
<h1>Register</h1>
    <form method="POST">
        Id: <input type="text" name="id" /><br/>
        Name: <input type="text" name="name" /><br/>
        Age: <input type="text" name="age" /><br/>
        <input type="submit" value="Register" />
    </form>
</body>
</html>
```

四、校验表单

Spring提供对Java校验API（Java Validation API）的支持。在Spring MVC中使用Java校验API，只需要保证在类路径下包含这个Java API的实现即可，比如`hibernate-validator`。

POM文件增加依赖：

```xml
<!-- Java校验API，JSR-303 -->
<dependency>
    <groupId>org.hibernate.validator</groupId>
    <artifactId>hibernate-validator</artifactId>
    <version>6.1.5.Final</version>
</dependency>
```

实体类属性增加注解：

```java
public class Student {

    private int id;

    @NotBlank
    private String name;

    private int age;

    public Student() {}

    public Student(int id, String name, int age) {
        this.id = id;
        this.name = name;
        this.age = age;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public int getAge() {
        return age;
    }

    public void setAge(int age) {
        this.age = age;
    }
}
```

控制器参数增加校验注解`@Valid`，启用校验功能：

```java
    /**
     * 接收请求的输入
     * 处理表单参数
     * `@Valid`注解开启校验
     *  Errors参数要紧跟在带有@Valid注解的参数后面
     *  
     * @param student
     * @return
     */
    @RequestMapping(value = "/register", method = RequestMethod.POST)
    public String processRegisterForm(@Valid Student student, Errors errors) {
        if (errors.hasErrors()) {
            return "student/registerForm";  // 如果校验出错，重新返回表单
        }
        studentList().add(student);
        // "redirect:"前缀重定向，"forward:"前缀前往指定的URL路径
        return "redirect: /student/" + student.getId();
    }
```

#### 渲染Web视图

将**控制器**中请求处理的逻辑和**视图**中的渲染实现解耦是Spring MVC的一个重要特性。控制器方法和视图的实现会在**模型**内容上达成一致，这是两者最大的关联，除此之外，两者应该保持足够的距离。

主要接口`ViewResolver`和`View`：

```java
public interface ViewResolver {

	/**
	 * Resolve the given view by name.
	 * <p>Note: To allow for ViewResolver chaining, a ViewResolver should
	 * return {@code null} if a view with the given name is not defined in it.
	 * However, this is not required: Some ViewResolvers will always attempt
	 * to build View objects with the given name, unable to return {@code null}
	 * (rather throwing an exception when View creation failed).
	 * @param viewName name of the view to resolve
	 * @param locale the Locale in which to resolve the view.
	 * ViewResolvers that support internationalization should respect this.
	 * @return the View object, or {@code null} if not found
	 * (optional, to allow for ViewResolver chaining)
	 * @throws Exception if the view cannot be resolved
	 * (typically in case of problems creating an actual View object)
	 */
	@Nullable
	View resolveViewName(String viewName, Locale locale) throws Exception;

}

public interface View {

	/**
	 * Return the content type of the view, if predetermined.
	 * <p>Can be used to check the view's content type upfront,
	 * i.e. before an actual rendering attempt.
	 * @return the content type String (optionally including a character set),
	 * or {@code null} if not predetermined
	 */
	@Nullable
	default String getContentType() {
		return null;
	}

	/**
	 * Render the view given the specified model.
	 * <p>The first step will be preparing the request: In the JSP case, this would mean
	 * setting model objects as request attributes. The second step will be the actual
	 * rendering of the view, for example including the JSP via a RequestDispatcher.
	 * @param model a Map with name Strings as keys and corresponding model
	 * objects as values (Map can also be {@code null} in case of empty model)
	 * @param request current HTTP request
	 * @param response he HTTP response we are building
	 * @throws Exception if rendering failed
	 */
	void render(@Nullable Map<String, ?> model, HttpServletRequest request, HttpServletResponse response)
			throws Exception;

}
```

`ViewResolver`接口的`resolveViewName()`方法传入一个视图名和Locale对象时，它会返回一个`View`实例。

`View`接口的任务就是接收模型以及`Servlet`的`request`和`response`对象，并将结果渲染到`response`对象中。

Spring自带了13个视图解析器，能够将逻辑视图名转换为物理实现，常用的视图如下：

- BeanNameViewResolver，默认的视图解析器，将视图解析为Spring应用上下文的bean，其中bean的ID与视图的名字相同。
- ContentNegotiatingViewResolver，通过考虑客户端需要的内容类型来解析视图，委托给另一个能产生对应内容类型的视图解析器。
- InternalResourceViewResolver，将视图解析为Web应用的内部资源（一般为JSP）。

##### 创建JSP视图

Spring提供了两种支持JSP视图的方式：
- InternalResourceViewResolver会将视图名解析为JSP文件。另外，如果在你的JSP页面中使用了JSP标准标签库（JavaServer Pages Standard Tag Library，JSTL）的话，InternalResourceViewResolver能够将视图名解析为`JstlView`形式的JSP文件，从而将JSTL本地化和资源bundle变量暴露给JSTL的格式化（formatting）和信息（message）标签。
- Spring提供了两个JSP标签库，一个用于表单到模型的绑定，另一个提供了通用的工具类特性。

逻辑视图名中包含斜线时，这个斜线也会带到资源的路径名中

```java
    @RequestMapping(value = "/list", method = RequestMethod.GET)
    public String list(Model model) {
        // 设置模型数据
        model.addAttribute(buildStudentList());
        // 返回逻辑视图名，逻辑视图名中包含斜线时，这个斜线也会带到资源的路径名中
        // 查找"WEB-INF/views/student/list.jsp"
        return "student/list";
    }
```

`InternalResourceViewResolver`默认将逻辑视图名解析为`InternalResourceView`实例，这个实例会引用JSP文件。但是这些JSP使用JSTL标签来处理格式化和信息的话，那么需要`InternalResourceViewResolver`将视图解析为`JstlView`实例，我们只需设置它的`viewClass`属性即可

```java
    // 配置JSP视图
    @Bean
    public ViewResolver viewResolver() {
        InternalResourceViewResolver resolver =
                new InternalResourceViewResolver();
        resolver.setPrefix("/WEB-INF/views/");
        ;
        resolver.setSuffix(".jsp");
        resolver.setExposeContextBeansAsAttributes(true);
        // 设置将视图解析为JstlView实例
        resolver.setViewClass(JstlView.class);
        return resolver;
    }
```

为了使用表单绑定库，需要在JSP页面上对其进行声明：

```jsp
<%@ taglib uri="http://www.springframework.org/tags/form" prefix="sf" %>
```

要使用Spring通用的标签库，必须进行以下声明：

```jsp
<%@ taglib uri="http://www.springframework.org/tags" prefix="s" %>
```

## 使用Spring MVC创建REST API

近几年来，以**信息为中心**的**表述性状态转移（Representational Stat Transfer，REST）**已成为替换传统SOAP Web服务的流行方案。SOAP一般关注行为和处理，而REST关注的是要处理的数据。

### REST的基础知识

REST与RPC几乎没有任何关系。RPC是面向服务的，并关注行为和动作；而REST是面向资源的，强调描述应用程序的事务和名词。应该尽量避免使用诸如REST服务、REST Web服务或类似的术语，这些术语会不恰当地强调行为。更应该强调REST面向资源的本质，并讨论RESTful资源。

REST的构成部分：

- 表述性（Representational）：REST资源实际上可以用各种形式来进行表述，包括XML、JSON（JavaScript Object Notation）甚至HTML-最适合资源使用者的任意形式。
- 状态（State）：当使用REST的时候，我们更关注资源的状态而不是对资源采取的行为；
- 转移（Transfer）：REST涉及到转移资源数据，它以某种表述性形式从一个应用转移到另一个应用。

在REST中，资源通过URL进行识别和定位。

REST中会有行为，它们是通过HTTP方法来进行定义。具体来讲，也就是GET、POST、PUT、DELETE、PATCH以及其他的HTTP方法构成了REST中的动作。这些HTTP方法通常会匹配为如下的CRUD动作：

- Create：POS
- Read：GET
- Update：PUT
- Delete：DELETE

有时候，PUT可以用来创建新资源，POST可以用来更新资源。实际上，POST请求非幂等性（non-idempotent）的特点使其成为一个非常灵活的方法，对于无法适应其他HTTP方法语义的操作，它都能够胜任。

### Spring对REST的支持

Spring支持以下方式来创建REST资源：

- 控制器可以处理所有的HTTP方法，包括四个主要的REST方法：GET、PUT、DELETE以及POST；
- 借助`@PathVariable`注解，控制器能够处理参数化URL（将变量输入作为URL的一部分）；
- 借助Spring的视图和视图解析器，资源能够以多种方式进行表述，包括将模型数据渲染为XML、JSON、Atom以及RSS的View实现；
- 可以使用`ContentNegotiatingViewResolver`来选择最适合客户端的表述；
- 借助`@ResponseBody`注解和各种`HttpMethodConverter`实现，能够替换基于视图的渲染方式；
- 类似地，`@RequestBody`注解以及`HttpMethodConverter`实现可以将传入的HTTP参数转化为传入控制器处理方法的Java对象；
- 借助`RestTemplate`，Spring应用能够方便地使用REST资源。

### 创建REST端点（endpoint）

表述是REST中很重要的一个方面。它是关于客户端和服务器端针对某一资源是如何通信的。任何给定的资源都几乎可以用任意的形式来进行表述（JSON、XML、PDF、Excel等）。资源没有变化-只是它的表述形式变化了。

如果客户端时JavaScript的话，JSON会成为优胜者，因为在JavaScript中使用JSON数据根本就不需要编排和解排（marshaling/demarshaling）。

控制器（Controller）本身通常并不关心资源如何表述。控制器以Java对象的方式来处理资源。控制器完成了它的工作之后，资源才会被转换成最适合客户端的形式。

Spring提供了两种方法将资源的Java表述形式转换为发送给客户端的表述形式：

- 内容协商（Content negotiation）：选择一个**视图**，它能够将模型渲染为呈现给客户端的表述形式；
- 消息转换器（Message Conversion）：通过一个**消息转换器**将控制器所返回的对象转换为呈现给客户端的表述形式。

#### 协商资源表述

#### 使用HTTP消息转换器

使用消息转换功能时，`DispatchServlet`不再将模型数据传送到视图中。实际上，这里根本就没有模型，也没有视图，只有控制器产生的数据，以及消息转换器（message converter）转换数据之后产生的资源表述。

Spring提供了多个HTTP消息转换器，用于实现资源表述与各种Java类型之间的互相转换，常用的有：

- `BufferedImageHttpMessageConverter`，`BufferedImages`与图片二进制数据之间互相转换
- `ByteArrayHttpMessageConverter`，读取/写入字节数组。从**所有媒体类型（*/*）**中读取，并以`application/octet-stream`格式写入
- `FormHttpMessageConverter`，将`application/x-www-form-urlencoded`内容读入到`MultiValueMap<String, String>`中，也会将`MultiValueMap<String, String>`写入到`application/x-www-form-urlencoded`中或将`MultiValueMap<String, Object>`写入到`multipart/form-data`中
- `MappingJacksonHttpMessageConverter`，在JSON和类型化的对象或非类型化的HashMap间互相读取和写入。**如果jackson-databind JSON 库在类路径下，将进行注册**。
- `MappingJackson2HttpMessageConverter`，在JSON和类型化的对象或非类型化的HashMap间互相读取和写入。**如果jackson-databind 2 JSON 库在类路径下，将进行注册**。
- `MappingJackson2XmlHttpMessageConverter`，在XML（`text/xml`或`application/xml`）和使用JAXB2或Jacksons注解的对象间互相读取和写入。**如果jackson-dataformat-xml库在类路径下，将进行注册**。
- `Jaxb2RootElementHttpMessageConverter`，在XML（`text/xml`或`application/xml`）和使用JAXB2注解的对象间互相读取和写入。**如果JAXB V2 库在类路径下，将进行注册**。
- `MarshallingHttpMessageConverter`，使用注入的编排器和解排器（marshaller和unmarshaller）来读入和写入XML。支持的编排器和解排器包括Castor、JAXB2、JIBX、XMLBeans以及Xstream。
- `ResourceHttpMessageConverter`，读取或写入Resource。
- `StringHttpMessageConverter`，将**所有媒体类型（*/*）**读取为String。将String写入为`Content-Type: text/plain`。

为了支持消息转换，我们需要对Spring MVC的编程模型进行一些小调整。

一、在响应体中返回资源状态

为控制器方法添加`@ResponseBody`注解，`@ResponseBody`注解会告知Spring，我们要将返回的对象作为资源发送给客户端，并将其转换为客户端可接受的表述形式。更具体地讲，`DispatchServlet`将会考虑到**Accept**头部信息，并查找能够为客户端提供所需表述形式的消息转换器。

Jackson默认会使用反射，但是如果你重构了Java类型，比如添加、移除或重命名属性，那么所产生的JSON也将会发生变化（如果客户端依赖这些属性的话，那客户端有可能会出错）。但是，我们可以在Java类型上使用**Jackson的映射注解（Jackson-Annotations）**，从而改变产生JSON的行为。

```java
    /**
     * `@ResponseBody`注解会告知Spring，
     * 我们要将返回的对象作为资源发送给客户端，
     * 并将其转换为客户端可接受的表述形式
     * 请求头：Accept: application/json
     * 响应头：Content-Type: application/json
     *
     * `@RequestMapping`的`produces`属性表明
     * 这个方法只处理预期输出为JSON的请求，
     * 如果预期输出为XML的请求，不会被该方法处理
     *
     * @return
     */
    @RequestMapping(value = "/list", method = RequestMethod.GET, produces = {MediaType.APPLICATION_JSON_VALUE})
    public @ResponseBody List<Student> list() {
        return studentList;
    }
```

二、在请求体中接收资源状态

为控制器方法参数添加`@RequestBody`注解，告诉Spring查找一个消息转换器，将来自客户端的资源表述转换为Java对象（**该类定义必须保持有默认的构造函数**）。

```java
    /**
     * `@RequestBody`注解，告诉Spring查找一个消息转换器，
     * 将来自客户端的资源表述转换为Java对象
     * （该类定义必须保持有默认的构造函数）。
     *
     * `@RequestBody`的`consumers`属性表明
     * 这个方法只关注请求头部`Content-Type: application/json`
     * 的请求
     *
     * @param student Student类定义必须保持默认的构造函数
     * @return
     */
    @RequestMapping(method = RequestMethod.POST, consumes = {MediaType.APPLICATION_JSON_VALUE})
    public @ResponseBody
    Student saveStudent(@RequestBody Student student) {
        studentList.add(student);
        return studentList.get(studentList.size() - 1);
    }
```

三、为控制器默认设置消息转换

如果在**控制器类**上使用`@RestController`注解来代替`@Controller`注解的话，Spring将会为该控制器的所有处理器方法应用消息转换功能（相当于替换每个方法上的`@ResponseBody`注解）。

```java
/**
 * 如果在**控制器类**上使用`@RestController`
 * 注解来代替`@Controller`注解的话，Spring
 * 将会为该控制器的所有处理器方法应用消息转换功能
 * （相当于替换每个方法上的`@ResponseBody`注解）
 * 
 */
@RestController
@RequestMapping("/api/student/default")
public class StudentDefaultRestController {

    private static List<Student> studentList;

    static {
        studentList = studentList = new ArrayList<Student>();
        studentList.add(new Student(1, "Zhangsan", 3));
        studentList.add(new Student(2, "Lisi", 4));
        studentList.add(new Student(3, "Wangwu", 5));
    }

    /**
     * `@RequestMapping`的`produces`属性表明
     * 这个方法只处理预期输出为JSON的请求（Accept: application/json），
     * 如果预期输出为XML的请求，不会被该方法处理
     *
     * @return
     */
    @RequestMapping(value = "/list", method = RequestMethod.GET, produces = {MediaType.APPLICATION_JSON_VALUE})
    public List<Student> list() {
        return studentList;
    }

    /**
     * `@RequestBody`注解，告诉Spring查找一个消息转换器，
     * 将来自客户端的资源表述转换为Java对象
     * （该类定义必须保持有默认的构造函数）。
     *
     * `@RequestBody`的`consumers`属性表明
     * 这个方法只关注请求头部`Content-Type: application/json`
     * 的请求
     *
     * @param student Student类定义必须保持默认的构造函数
     * @return
     */
    @RequestMapping(method = RequestMethod.POST, consumes = {MediaType.APPLICATION_JSON_VALUE})
    public Student saveStudent(@RequestBody Student student) {
        studentList.add(student);
        return studentList.get(studentList.size() - 1);
    }
}
```

## 参考

- Spring实战-第4版

