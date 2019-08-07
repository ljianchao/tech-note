# 发布记录
[Release Notes](https://github.com/spring-projects/spring-boot/wiki)

## Spring Boot 1.1 Release Notes

### Upgrading from Spring Boot 1.0
- - -

#### ErrorController
The ErrorController interface and related classes have been moved from spring-boot-actuator to spring-boot-autoconfigure. This change should be transparent for most users, however, some package renames have occurred.

#### Multipart Config
The MultiPartConfigFactory had been renamed to MultipartConfigFactory (note the different case).

#### Reactor
Spring Boot now provides Reactor 1.1.2 by default (1.0.x provided Reactor 1.0.1). In Reactor 1.1 the reactor-tcp module has been renamed reactor-net. If you were using reactor-tcp you should update your dependency to use reactor-net instead.

#### Running your app using Maven
Spring Boot now forks a process to run your app (i.e. using mvn spring-boot:run). This concretely means that command line arguments are no longer available to the application. Check this example for more details.

### New and Noteworthy
- - -

#### Version Updates
Several third-party dependencies have been upgraded to align versions with those defined in the upcoming Spring IO Platform release. It is now also possible to use a custom version meta-data file with the Spring Boot Gradle plugin.

#### Maven Plugin
The Spring Boot Maven plugin now supports excludes which can be used to filter the libraries that bundled as part of your ``fat'' jar. Plugin documentation is now also published as a [maven site](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/maven-plugin/).

#### Custom Banner
You can now add a custom banner by adding a banner.txt file to your classpath or by setting a banner.location property.

#### Elasticsearch
Auto-configuration support is now available for Elasticseach, along with a new `spring-boot-starter-data-elasticsearch` starter POM.

##### Apache Solr
Auto-configuration support is now available for Apache Solr, along with a new `spring-boot-starter-data-solr` starter POM.

#### Web Improvements
Auto-configuration classes for `Spring HATEOAS'' and` Spring Data Rest MVC'' have now been added to Spring Boot. Multi-part file upload support has been changed to be enabled by default.

#### Spring Integration
Auto-configuration has been added for Spring Integration. If Spring Integration is on your classpath you will now automatically get `@EnableIntegration` as well as some Spring Integration JMX beans.

#### Jackson
Support for the Jackson JSON marshaling library has been improved. You will now get auto-configured beans for Jackson’s `ObjectMapper` as well as `JodaModule` and `JSR310Module` support when appropriate. You can now also add your own `Module` beans and have them automatically applied.

#### HikariDataSource
Support has been added for the [Hikari Connection Pool](https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-1.1-Release-Notes) library. Simply ensure that the HikariCP library is on your classpath and that the Tomcat DBCP is not.

#### New Conditional Annotations
Two new `@Conditional` annotations have been added to Spring Boot 1.1. `@ConditionalOnProperty` supports conditionally enabling beans based on the presence of an `Environment` property and `@ConditionalOnJava` can be used to enable beans based on the JVM version.

## Spring Boot 1.2 Release Notes

### Upgrading from Spring Boot 1.1
- - -

#### Servlet 3.1, Tomcat 8 and Jetty 9
Spring Boot now uses Tomcat 8 and Jetty 9 as embedded servlet containers. This provides Servlet 3.1 and enhanced WebSocket support out of the box. You can still use Tomcat 7 or Jetty 8 if you prefer to stick to an older version. See the spring-boot-sample-tomcat7-jsp and spring-boot-sample-jetty8 samples for downgrade examples.

#### Logging output
The default logging configuration has been updated with Spring Boot 1.2 to no longer write log files. If you want file output, you can use the `logging.path` or `logging.file` properties. You can still also completely customize logging by adding your own `logback.xml` file.

#### Consistent HTTP URI/body decoding
A `CharacterEncodingFilter` is now registered automatically for consistent URI/body decoding. You can use the `spring.http.encoding.charset` property if you need something other than UTF-8 or set `spring.http.encoding.enabled` to false if you don’t want the `CharacterEncodingFilter` registered at all.

#### IgnoreDefaultModelOnRedirect with Spring MVC
Spring MVC auto-configuration now sets the `ignoreDefaultModelOnRedirect` property of `RequestMappingHandlerAdapter` to `true` by default. If you need model attributes to be part of your redirect URLs you can add the following to your `application.properties`:
> spring.mvc.ignore-default-model-on-redirect=false

#### Jackson defaults
The auto-configured Jackson `ObjectMapper` now has `MapperFeature.DEFAULT_VIEW_INCLUSION` and `DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES` disabled. If you need to revert to the previous settings you can add the following to your `application.properties`:
> spring.jackson.mapper.default-view-inclusion=true 
> spring.jackson.deserialization.fail-on-unknown-properties=true 

#### Moved health.* properties to management.health.*
Health-related configuration keys have been moved from `health` to `management.health` for consistency with other management-related properties.

#### PersistenceExceptionTranslationPostProcessor
A `PersistenceExceptionTranslationPostProcessor` will now be registered by default. If you don’t want exception translation set the `spring.dao.exceptiontranslation.enabled` property to `false`.

#### Anonymous health access restrictions
The `/health` actuator endpoint now restricts anonymous access. When accessed anonymously the endpoint hides the health details and simply indicates if the server is `UP` or `DOWN`. It also caches the response for the period specified by `endpoints.health.time-to-live`. These restrictions can be disabled, thereby restoring 1.1.x’s behavior, by setting `endpoints.health.sensitive` to `false`.

#### Spring 4.1
Spring Boot 1.2 requires Spring Framework 4.1.5 or later and is not compatible with Spring Framework 4.0.

#### Hikari CP
The version of `com.zaxxer:HikariCP` that’s provide by Boot’s dependency management now requires Java 8. If you’re running on Java 6 or Java 7 you should update your dependency to use `com.zaxxer:HikariCP-java6` instead.

#### Deprecations
- The `org.springframework.boot.actuate.system.ApplicationPidListener` class has been superseded by `ApplicationPidFileWriter`

- The CLI `@EnableRabbitMessaging` annotation has been superseded by `@EnableRabbit` from the `spring-rabbit` project.

- The `http.mappers.` properties are deprecated in favor of their `spring.jackson.serialization.` equivalents.

- `org.springframework.boot.json.SimpleJsonParser` has been deprecated in favor of `BasicJsonParser` to avoid confusion with the ```JSON Simple''` library.

### New and Noteworthy
- - -

#### Version Updates
Spring Boot 1.2 builds on and requires Spring Framework 4.1. Several 3rd party dependencies have been upgraded with this release including Jackson, Joda Time and Hibernate Validator. Tomcat 8 and Jetty 9 are now the default embedded servlet containers (providing Servlet 3.1 support).

#### @SpringBootApplication annotation
A new `@SpringBootApplication` convenience annotation has been added which is equivalent to `@Configuration` + `@EnableAutoConfiguration` + `@ComponentScan`. If you find you use frequently use those three annotations, you might want to consider swapping.

#### JTA Support
Spring Boot 1.2 now supports distributed JTA transactions across multiple XA resources using either an Atomkos or Bitronix embedded transaction manager. JTA transactions are also supported when deploying to a suitable Java EE Application Server.

When a JTA environment is detected, Spring’s `JtaTransactionManager` will be used to manage transactions. Auto-configured JMS, DataSource and JPA beans will be upgraded to support XA transactions. You can use standard Spring idioms such as `@Transactional` to participate in a distributed transaction.

Additionally, general support classes are provided to make Atomkos and Bitronix easier to configure, even if you are not using `@EnableAutoConfiguration`. See the JTA section of the reference manual for details.

#### JNDI Lookups
If you are using a full Java EE Application Server you can now lookup both `DataSource` and JMS `ConnectionFactory` beans from JNDI. Use the `spring.datasource.jndi-name` and `spring.jms.jndi-name` properties in your `application.properties` or `application.yml` file.

#### Jackson customization
It is now possible to [customize the Jackson ObjectMapper](https://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#howto-customize-the-jackson-objectmapper) using `spring.jackson` properties. Jackson’s `SerializationFeature`, `DeserializationFeature`, `MapperFeature`, `JsonParser.Feature` and `JsonGenerator.Feature objects` can be customized using `serialization`, `deserialization`, `mapper`, `parser` and `generator` properties. For example to enable `DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES` you could add the following to your `application.properties`:
> spring.jackson.deserialization.fail-on-unknown-properties=true

Additionally `spring.jackson.date-format` and `spring.jackson.property-naming-strategy` properties have also been added with Spring Boot 1.2

#### Banner Interface
A new `Banner` interface has been added which can be used in combination with `SpringApplication.setBanner(…​)` to provide customized banner output. The use of `src/main/resources/banner.txt` is still recommended for simple banners, however, the new interface is useful if you need to do something fancy.

#### Banner Properties
You can now use `${application.version}`, `${spring-boot.version}`, `${application.formatted-version}` and `${spring-boot.formatted-version}` variables in your `src/main/resources/banner.txt` file to print your application version and/or the version of Spring Boot. The `formatted-version` is enclosed in brackets and includes a `v` prefix, for example, `(v1.2.0.RELEASE)`.

#### JMS Support
Auto-configuration is now provided for Spring Framework 4.1’s @JmsListener annotation. The @EnableJms annotation is also auto-configured whenever you have spring-jms.jar on your classpath.

#### Spring Cloud Connectors
Auto-configuration has been added for `spring-cloud-connectors` and a new `spring-boot-starter-cloud-connectors` POM has also been included with this release. Auto-configuration provides equivalent functionality to the `@CloudScan` annotation.

#### Conditions
The `@ConditionalOnProperty` annotation has been updated with new `havingValue` and `matchIfMissing` attributes. You can now use the condition to create much more complex property matching conditions. A new `AnyNestedCondition` class has also been added which can be used to compose other `@Conditions`. Finally, the `@ConditionalOnBean` annotation now supports types declared as a String attribute as well as a Class.

#### GSON Support
It is now possible to use GSON instead of Jackson to create JSON output. Jackson remains the default and recommended choice. If you use GSON you should exclude Jackson libraries from your Maven/Gradle build unless your are using Spring Boot’s Actuator which requires Jackson. As of Spring Boot 1.2.2, if both Gson and Jackson are on the classpath, you can configure your application to use Gson by setting `spring.http.converters.preferred-json-mapper` to `gson`.

#### Log4j2
Log4J is now supported as an alternative logging system and `spring-boot-starter-log4j2` start POM is provided. Logback remains the default and recommended logging system.

#### Jersey auto-configuration
Auto-configuration support is now provided for Jersey. Refer to reference documentation for details.

#### Apache-Commons DBCP2 Support
The Apache `commons-dbcp2` database connection pool library is now supported in addition to Tomcat, Hikari and DBCP (v1).

#### Maven Plugin
The `repackage` task of the `spring-boot-maven-plugin` can now be disabled. This can be useful if you want to use `spring-boot:run` but you don’t need ``Fat Jars''.

#### Configuration meta-data
The `spring-boot`, `spring-boot-autoconfigure` and `spring-boot-actuator` jars now include additional meta-data files which can be used by tools developers to provide `code completion'' support in `application.properties files. An annotation processor is also provided to allow you to generate your own files automatically from ``@ConfigurationProperties` classes.


## Spring Boot 1.3.0 Release Notes

### Upgrading from Spring Boot 1.2
- - -

#### Deprecations from Spring Boot 1.2
Classes, methods and properties that were deprecated in Spring Boot 1.2 have been removed in this release. Please ensure that you aren’t calling deprecated methods before upgrading.

#### Jackson
Spring Boot 1.2 registered any Jackson Module beans with every `ObjectMapper` in the application context. This made it impossible to take complete control of an `ObjectMapper` bean’s modules. Spring Boot 1.3 will only register Jackson Module beans with `ObjectMappers` that are created or configured with the auto-configured `Jackson2ObjectMapperBuilder`. This brings the behaviour of module configuration into line with Boot’s other Jackson configuration.

#### Logging
##### Spring specific configuration
In order to prevent double initialization Spring specific log configuration files can now be used. It’s recommended (although not required) that you rename any default log configuration files to use a `-spring` suffix. For example `logback.xml` would change to `logback-spring.xml`.

##### Initialization failures
In Spring Boot 1.2, if you specified a custom logging configuration file using `logging.config` and the file did not exist, it would silently fallback to using the default configuration. Spring Boot 1.3 fails due to the missing file. Similarly, if you provided a custom Logback configuration file which was malformed, Spring Boot 1.2 would fall back to its default configuration. Spring Boot 1.3 fails and reports the problems with the configuration to `System.err`.

#### HTTP response compression
Spring Boot 1.2 supported native response compression for Tomcat users, or compression using Jetty’s GZipFilter for users of Jetty, Tomcat, and Undertow. Motivated by the Jetty team’s deprecation of their gzip filter, Spring Boot 1.3 replaces this with support for native response compression in all three embedded containers. As a result the `server.tomcat.compression.` and `spring.http.gzip.` properties are no longer supported. The new `server.compression.*` properties should be used instead.

#### Tomcat session storage
By default tomcat no longer saves session data in `/tmp`. If you want to use persistent sessions with Tomcat set the `server.session.persistent` property to true. The `server.session.store-dir` can be used to save files in a specific location.

#### Jetty JSPs
The `spring-boot-starter-jetty` "Starter POM" no longer includes `org.eclipse.jetty:jetty-jsp`. If you are using Jetty with JSPs you will now need to directly add this dependency yourself.

#### MVC Stacktrace output
Stacktrace information is now never included when Spring MVC renders an error response. If you want Spring Boot 1.2 behavior set `error.include-stacktrace` to `on-trace-param`.

#### Thymeleaf’s Spring Security integration
Due to the upgrade to Spring Security 4, Spring Boot 1.3 has also upgraded the dependency management and auto-configuration of Thymeleaf’s Spring Security support. The coordinates of the new artifact are `org.thymeleaf.extras:thymeleaf-extras-springsecurity4`. Please update your pom.xml or build.gradle accordingly.

#### Missing /templates folder errors
Spring Boot applications no longer fail to start when a `/templates` folder cannot be found. If you are using a supported templating technology, and you forget to add `/templates`, a warning is now logged instead.

#### Auto-configuration order
Spring Boot no longer honours `@Order` to manage ordering between auto-configuration classes, please use `@AutoconfigureOrder` instead. Note also that you can use `@AutoconfigureBefore` and `@AutoconfigureAfter` to specify the order against specific auto-configuration classes.

#### Maven plugin
##### spring-boot:run resources
The Spring Boot Maven plugin no longer adds `src/main/resources` directly to the classpath when using `spring-boot:run`. If you want live, in-place editing we recommend using Devtools. The `addResources` property can be set in your `pom.xml` if you want to restore Spring Boot 1.2. behavior.

#### Maven resources filtering
If you use the `spring-boot-starter-parent`, Maven tokens are only filtered using `@` now. This prevents any Spring placeholders in your configuration (e.g. ${foo}) to be be expanded by the build.

Concretely, if you are still using the standard format (i.e. `${project.version}`) please migrate them (`@project.version@`) or override the `maven-resources-plugin` configuration.


#### Dependencies
##### Spring 4.2
Spring Boot 1.3 requires Spring Framework 4.2 or later and is not compatible with earlier versions.

##### Spring Security 4.0
Spring Boot 1.3 uses Spring Security 4.0. See the Spring Security documentation for information on migrating from 3.2.

### New and Noteworthy
- - -

Tip: Check [the configuration changelog](https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-1.3.0-Configuration-Changelog) for a complete overview of the changes in configuration.


#### Version Updates
Spring Boot 1.3 builds on and requires Spring Framework 4.2. Several 3rd party dependencies have been upgraded with this release. No major upgrades have been made to the Tomcat or Jetty versions with this release.

#### Developer Tools
Spring Boot 1.3 includes a new `spring-boot-devtools` module which aims to improve the development-time experience. The module provides:

- Sensible property defaults (for example disabling template caches) 

- Automatic application restarts 

- LiveReload support 

- Remote development support (including remote updates and remote debug via an HTTP tunnel). 

- Persistent HTTP sessions across restarts 

See the [updated documentation](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/reference/htmlsingle/#using-boot-devtools) for more information.

#### Caching Auto-configuration
Auto-configuration is now provided for the following cache technologies:

- EhCache 

- Hazelcast 

- Infinispan 

- Any compliant JCache (JSR 107) implementation 

- Redis 

- Guava 

In addition, simple Map based in-memory caching is also supported. Caching is automatically configured when your application `@Configuration` is annotated with `@EnableCaching`. Cache statistics are now also exposed as an actuator endpoint (when the underlying technology allows).

For complete details see the [updated documentation](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/reference/htmlsingle/#boot-features-caching).

#### Fully executable JARs and service support
The Spring Boot Maven and Gradle plugins can now generate full executable archives for Linux/Unix operating systems. Furthermore you can now easily install these JARs as `init.d` or `systemd` services. Running a fully executable JAR is as easy as typing:

> $ ./myapp.jar 
and to install it as an init.d service:

> $ sudo link -s /var/myapp/myapp.jar /etc/init.d/myapp 
Additional information is available in the [reference documentation](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/reference/htmlsingle/#deployment-service).

#### OAuth2 Support
You can now use `@EnableAuthorizationServer` and `@EnableResourceServer` to quickly create OAuth2 authorization and resource servers. In addition, `@EnableOAuth2Client` allows your application to act as an OAuth2 client. For details see the overhauled [security section of the reference guide](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/reference/htmlsingle/#boot-features-security-oauth2).

#### Validation "Starter POM"
A new `spring-boot-starter-validation` "starter POM" is now available to provide bean validation (JSR 303) support.

#### Support for @WebServlet, @WebFilter and @WebListener
When using an embedded servlet container, automatic registration of `@WebServlet`, `@WebFilter` and `@WebListener` annotated classes can now be enabled using `@ServletComponentScan`.

#### Spring resource chains
You can now configure basic aspects of Spring’s `ResourceChainRegistration` via application properties. This allows you to create unique resource names so that you can implement cache busting. The `spring.resources.chain.strategy.content.` properties can be used to configure fingerprinting based on the content of the resource; and `spring.resources.chain.strategy.fixed.` properties can be used if you want to use a "fixed version" for your fingerprint.

#### DataSource type
The connection pool used by auto-configuration can now be specified via the `spring.datasource.type` configuration key.

#### Logging
##### Log Patterns
The `logging.pattern.console` and `logging.pattern.file` properties can now be used to specify a logging pattern directly from your `application.properties`. That can be handy If you only want to customize patterns as you no longer need to define your own `log*.xml` file.

##### Jar details in stacktraces
If you are using logback or log4j2, we now include information about the location from which each class in a stack trace was loaded (this can be customized via `logging.exception-conversion-word`).

##### Logback extensions
Spring Boot 1.3 supports some new tags which can be used in your logback configuration file. To use the tags you need to first rename any `logback.xml` configuration to `logback-spring.xml`. Once your configuration file has been renamed, the following tags are available.

See the [Logback extensions](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/reference/htmlsingle/#boot-features-logback-extensions) section of the reference documentation for more details.

#### Configuration properties
If you are using `@ConfigurationProperties` on beans, you no longer need to add `@EnableConfigurationProperties` to your configuration as Spring Boot autoconfigures it now. As before you can ask Spring to create a bean for your `@ConfigurationProperties` class using the value attribute of `@EnableConfigurationProperties` or with a regular `@Bean` definition.

#### Configuration properties conversion
If you need custom type conversion for some configuration keys without a custom `ConversionService` (with bean id conversionService), you now need to qualify the `Converter` beans to use with `@ConfigurationPropertiesBinding` as we no longer lookup all Converter beans.

#### Auto-configuration
The auto-configuration report has now an additional section called "Unconditional classes". It lists any auto-configuration classes that do not have any class-level conditions, i.e. the class will be always be part of the application’s configuration. It also now lists configurations that have been manually excluded via `@SpringBootApplication(exclude=…​)` or `@EnableAutoConfiguration(exclude=…​)`.

It is now possible to also exclude auto-configuration classes via the `spring.autoconfigure.excludes` property. Similarly, a new `@ImportAutoConfiguration` annotation can be used by tests that wish to selectively import certain auto-configuration classes.

#### MVC Error handling
The `error.include-stacktrace` property can now be used to determine when stack trace attributes should be included in MVC error responses. Options are `never`, `always` or `on-trace-param` (with `never` being the default).

#### Disabling health indicators
It is now possible to easily disable all the default health indicators via the `management.health.defaults.enabled` property.

#### TraceWebFilter options
The actuator TraceWebFilter (use to trace HTTP request/response details) can now log more information. Use the `management.trace.include` property to configure the options that you want to include (see the `TraceProperties.Include` enum).

#### Maven Support
##### Maven start/stop support and admin features
The Maven plugin now include `start` and `stop` goals. These enable applications to be started without blocking Maven (allowing other goals to operate on the application). This technique is often used to launch intergration tests from Maven.

A byproduct of this work is that a new `SpringApplicationAdminMXBean` interface has been added which (when enabled) allows a Spring Boot application to be controlled via JMX.

##### Profile activation
The `spring-boot-maven-plugin` now includes a `profiles` property that can be used with `spring-boot:run`. You can configure profiles in your `pom.xml` or use `-Drun.profiles` on the command line. See the [updated plugin documentation](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/maven-plugin/examples/run-profiles.html) for details.

#### Configuration property meta-data updates
The `META-INF/spring-configuration-metadata.json` file format has been updated to support new deprecation and hints attributes. Hints can be used by IDE developers to provided better content assist support. Deprecation allow for the deprecation and a replacement key, if any. Such information can be provided by adding `@DeprecatedConfigurationProperty` on the getter of the property. See the [updated appendix](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/reference/htmlsingle/##configuration-metadata-property-attributes) for details.

We’ve also improved the detection of default value: if a property is initialized via a method call having a single argument, we consider said argument to be the default value (i.e. `Charset.forName("UTF-8")` would detect `UTF-8` as the default value).

A new `spring-boot-configuration-metadata` module is now available for any tool developers wishing to use the configuration meta-data in their own tools and apps; it offers an API to read the meta-data and build a repository out of it.

#### Annotation processing
Apache HttpCore 4.4.5 removed a handful of annotations. This is a binary incompatible change if you are using an annotation processor and are sub-classing a class that uses one of the removed annotations. For example, if the class was using `@Immutable` you will see compile-time annotation processing fail with `[ERROR] diagnostic: error: cannot access org.apache.http.annotation.Immutable`.

The problem can be avoided by downgrading to HttpCore 4.4.4 or, preferably, by structuring your code so that the problematic class is not subject to compile-time annotation processing.

#### Deprecations in Spring Boot 1.3.0
- The `Application.showBanner` and `ApplicationBuilder.showBanner` methods have been deprecated in favor of `setBannerMode`.

- `@ConditionalOnMissingClass` now expects the class name to be provided using the value attribute rather than name.

- `Log4JLoggingSystem` is now deprecated following Apache’s [EOL declaration for log4j 1.x](https://blogs.apache.org/foundation/entry/apache_logging_services_project_announces).

- The `ConfigurableEmbeddedServletContainer` `setJspServletClassName` and `setRegisterJspServlet` methods have been replaced by setJspServlet.

- The `EndpointMBean` class (and subclasses) now expect an `ObjectMapper` to be provided to the constructor.

- The `DropwizardMetricWriter` had been replaced by `DropwizardMetricService`.

- The protected `SpringApplication.afterRefresh` method that takes a `String[]` has been deprecated in favor of a version that takes `ApplicationArguments`.

- `VcapEnvironmentPostProcessor` has been deprecated in favor of `CloudFoundryVcapEnvironmentPostProcessor`.

- The `LoggingSystem` `initialize` method has been deprecated in favor of a version that accepts `LoggingInitializationContext`.

- The `ServerPortInfoApplicationContextInitializer` has been deprecated to move it to a new package

- `org.springframework.boot.autoconfigure.orm.jpa.EntityManagerFactoryBuilder` has been deprecated to move it to a new package. `org.springframework.boot.orm.jpa.EntityManagerFactoryBuilder` should be used instead. A bean of the old type is no longer auto-configured. If your application uses this bean it should be updated to use the `org.springframework.boot.orm.jpa.EntityManagerFactoryBuilder` bean instead.