## Creating an executable jar

[Spring Boot Maven Plugin](https://docs.spring.io/spring-boot/docs/1.5.22.RELEASE/maven-plugin/index.html)

```xml
<build>
  ...
  <plugins>
    ...
    <plugin>
      <groupId>org.springframework.boot</groupId>
      <artifactId>spring-boot-maven-plugin</artifactId>
      <version>1.5.22.RELEASE</version>
      <executions>
        <execution>
          <goals>
            <goal>repackage</goal>
          </goals>
        </execution>
      </executions>
    </plugin>
    ...
  </plugins>
  ...
</build>
```

[Adding classpath to SpringBoot command line start when using maven-spring-boot-plugin](https://stackoverflow.com/questions/45710051/adding-classpath-to-springboot-command-line-start-when-using-maven-spring-boot-p)

```xml
<plugin>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-maven-plugin</artifactId>
    <configuration>
        <folders>
            <folder>
                /this/that/theother
            </folder>
        </folders>
    </configuration>
</plugin>
```

启动项目

```shell
mvn spring-boot:run
```

使用操作系统环境变量
```shell
export MAVEN_OPTS=-Xmx1024m -XX:MaxPermSize=128M
```

## Developing web applications

### Spring MVC auto-configuration

Spring Boot provides auto-configuration for Spring MVC that works well with most applications. The auto-configuration adds the following features on top of Spring’s defaults:

- Inclusion of `ContentNegotiatingViewResolver` and `BeanNameViewResolver` beans.

- Support for serving static resources, including support for WebJars (see below).

- Automatic registration of `Converter`, `GenericConverter`, `Formatter beans`.

- Support for `HttpMessageConverters` (see below).

- Automatic registration of `MessageCodesResolver` (see below).

- Static `index.html` support.

- Custom `Favicon` support (see below).

- Automatic use of a `ConfigurableWebBindingInitializer` bean (see below).

If you want to keep Spring Boot MVC features, and you just want to add additional MVC configuration (`interceptors`, `formatters`, `view controllers` etc.) you can add your own `@Configuration` class of type `WebMvcConfigurerAdapter`, but without `@EnableWebMvc`. If you wish to provide custom instances of `RequestMappingHandlerMapping`, `RequestMappingHandlerAdapter` or `ExceptionHandlerExceptionResolver` you can declare a `WebMvcRegistrationsAdapter` instance providing such components.

If you want to take complete control of Spring MVC, you can add your own `@Configuration` annotated with `@EnableWebMvc`.