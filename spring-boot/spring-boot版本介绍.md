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

## Spring Boot 1.4.0 Release Notes

### Upgrading from Spring Boot 1.3
- - -

#### Deprecations from Spring Boot 1.3

Classes, methods and properties that were deprecated in Spring Boot 1.3 have been removed in this release. Please ensure that you aren’t calling deprecated methods before upgrading.

Log4j 1 support has been removed following [Apache EOL announcement](https://blogs.apache.org/foundation/entry/apache_logging_services_project_announces).

#### Renamed starters
The following starters have been renamed, the old ones will be removed in Spring Boot 1.5

- `spring-boot-starter-ws` → `spring-boot-starter-web-services`

- `spring-boot-starter-redis` → `spring-boot-starter-data-redis`

#### DataSourceProperties get methods
Some `get…​` methods from `DataSourceProperties` have been changed to be more consistent with other `@ConfigurationProperties` classes. If you previously directly called any of the following methods in your code, you will need to migrate to the new `determine…​()` equivalents:

- `getDriverClassName()` → `determineDriverClassName()`

- `getUrl()` → `determineUrl()`

- `getUsername()` → `determineUsername()`

- `getPassword()` → `determineUsername()`

Note: 
> The `get` methods are not deprecated but their behavior has changed, make sure that you manually check for usage when upgrading.

#### DataSource binding
Prior to Spring Boot 1.4, auto-configured datasources were bound to the `spring.datasource` namespace. In 1.4, we only bind the common settings to `spring.datasource` (see DataSourceProperties) and we have defined new specific namespaces for the four connections pools we support (in that order):

- `spring.datasource.tomcat` for `org.apache.tomcat.jdbc.pool.DataSource`

- `spring.datasource.hikari` for `com.zaxxer.hikari.HikariDataSource`

- `spring.datasource.dbcp` for `org.apache.commons.dbcp.BasicDataSource`

- `spring.datasource.dbcp2` for `org.apache.commons.dbcp2.BasicDataSource`

If you were using specific settings of the connection pool implementation that you are using, you will have to move that configuration to the relevant namespace. For instance, if you were using Tomcat’s `testOnBorrow` flag, you’ll have to move it from `spring.datasource.test-on-borrow` to `spring.datasource.tomcat.test-on-borrow`.

If you are using configuration assistance in your IDE, you can now see which settings are available per connection pools rather than having all of them mixed in the `spring.datasource` namespace. This should make your life much easier figuring out what implementation supports what features.

#### Test utilities and classes
Spring Boot 1.4 ships with a new `spring-boot-test` module that contains a completely reorganized `org.springframework.boot.test package`. When upgrading a Spring Boot 1.3 application you should migrate from the deprecated classes in the old package to the equivalent class in the new structure. If you’re using Linux or OSX, you can use the following command to migrate code:
```
    find . -type f -name '*.java' -exec sed -i '' \
    -e s/org.springframework.boot.test.ConfigFileApplicationContextInitializer/org.springframework.boot.test.context.ConfigFileApplicationContextInitializer/g \
    -e s/org.springframework.boot.test.EnvironmentTestUtils/org.springframework.boot.test.util.EnvironmentTestUtils/g \
    -e s/org.springframework.boot.test.OutputCapture/org.springframework.boot.test.rule.OutputCapture/g \
    -e s/org.springframework.boot.test.SpringApplicationContextLoader/org.springframework.boot.test.context.SpringApplicationContextLoader/g \
    -e s/org.springframework.boot.test.SpringBootMockServletContext/org.springframework.boot.test.mock.web.SpringBootMockServletContext/g \
    -e s/org.springframework.boot.test.TestRestTemplate/org.springframework.boot.test.web.client.TestRestTemplate/g \
    {} \;
```

Additionally, Spring Boot 1.4 attempts to rationalize and simplify the various ways that a Spring Boot test can be run. You should migrate the following to use the new `@SpringBootTest` annotation:

- From `@SpringApplicationConfiguration(classes=MyConfig.class)` to `@SpringBootTest(classes=MyConfig.class)`

- From `@ContextConfiguration(classes=MyConfig.class, loader=SpringApplicationContextLoader.class)` to `@SpringBootTest(classes=MyConfig.class)`

- From `@IntegrationTest` to `@SpringBootTest(webEnvironment=WebEnvironment.NONE)`

- From `@IntegrationTest with @WebAppConfiguration` to `@SpringBootTest(webEnvironment=WebEnvironment.DEFINED_PORT) (or RANDOM_PORT)`

- From `@WebIntegrationTest` to `@SpringBootTest(webEnvironment=WebEnvironment.DEFINED_PORT) (or RANDOM_PORT)`

Tip: Whilst migrating tests you may also want to replace any `@RunWith(SpringJUnit4ClassRunner.class)` declarations with Spring 4.3’s more readable `@RunWith(SpringRunner.class)`.

For more details on the `@SpringBootTest` annotation refer to the [updated documentation](https://docs.spring.io/spring-boot/docs/1.4.x/reference/htmlsingle/#boot-features-testing-spring-boot-applications).

#### TestRestTemplate
The `TestRestTemplate` class no longer directly extends `RestTemplate` (although it continues to offer the same methods). This allows `TestRestTemplate` to be configured as a bean without it being accidentally injected. If you need access to the actual underlying `RestTemplate` use the `getRestTemplate()` method.

#### Maven spring-boot.version property
The `spring-boot.version` property has been removed from the `spring-boot-dependencies` pom. See [issue 5104](https://github.com/spring-projects/spring-boot/issues/5014) for details.

#### Dispatch Options Request
The default `spring.mvc.dispatch-options-request` property has changed from `false` to `true` to align with Spring Framework’s preferred default. If you don’t want `OPTIONS` requests to be dispatched to `FrameworkServlet.doService` you should explicitly set `spring.mvc.dispatch-options-request` to `false`.

#### Forced character encoding
Forced character encoding now only applies to requests (and not responses). If you want to force encoding for both requests and responses set `spring.http.encoding.force` to `true`.

#### Multipart support
The multipart properties have moved from the `multipart`. namespace to the `spring.http.multipart.` namespace.

#### Server header
The `Server` HTTP response header is no longer set unless the `server.server-header` property is set.

#### @ConfigurationProperties default bean names
When a `@ConfigurationProperties` bean is registered via `@EnableConfigurationProperties(SomeBean.class)`, we used to generate a bean name of the form `<prefix>.CONFIGURATION_PROPERTIES`. As of Spring Boot 1.4, we have changed that pattern to avoid name clashes if two beans use the same prefix.

The new conventional name is `<prefix>-<fqn>`, where `<prefix>` is the environment key prefix specified in the `@ConfigurationProperties` annotation and `<fqn>` the fully qualified name of the bean. If the annotation does not provide any prefix, only the fully qualified name of the bean is used.

#### Thymeleaf 3
By default, Spring Boot uses Thymeleaf 2.1 but it is now compatible with Thymeleaf 3 as well, check the [updated documentation](https://docs.spring.io/spring-boot/docs/current/reference/htmlsingle/#howto-use-thymeleaf-3) for more details.

#### Executable jar layout
The layout of executable jars has changed. If you are using Spring Boot’s Maven, Gradle, or Ant support to build your application this change will not affect you. If you are building an executable archive yourself, please be aware that an application’s dependencies are now packaged in `BOOT-INF/lib` rather than `lib`, and an application’s own classes are now packaged in `BOOT-INF/classes` rather than the root of the jar.

##### Jersey classpath scanning limitations
The change to the layout of executable jars means that a limitation in Jersey’s classpath scanning now affects executable jar files as well as executable war files. To work around the problem, classes that you wish to be scanned by Jersey should be packaged in a jar and included as a dependency in `BOOT-INF/lib`. The Spring Boot launcher should then be configured to unpack those jars on start up so that Jersey can scan their contents.

#### `@Transactional` default to cglib proxies
When Boot auto-configures the transaction management, `proxyTargetClass` is now set to `true` (meaning that cglib proxies are created rather than requiring your bean to implement an interface). If you want to align that behaviour for other aspects that aren’t auto-configured, you’ll need to explicitly enable the property now:
> @EnableCaching(proxyTargetClass = true)  

Note: If you happen to use `@Transactional` on interfaces, you’ll have to be explicit and add `@EnableTransactionManagement` to your configuration. This will restore the previous behaviour.

### New and Noteworthy
- - - 

Tip: Check [the configuration changelog](https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-1.4-Configuration-Changelog) for a complete overview of the changes in configuration.

#### Spring Framework 4.3
Spring Boot 1.4 builds on and requires Spring Framework 4.3. There are a number of nice refinements in Spring Framework 4.3 including new Spring MVC @RequestMapping annotations. Refer to the [Spring Framework reference documentation](https://docs.spring.io/spring-framework/docs/4.3.x/spring-framework-reference/htmlsingle/#new-in-4.3) for details.

Note that the test framework in Spring Framework 4.3 requires JUnit 4.12. See [SPR-13275](https://jira.spring.io/browse/SPR-13275) for further details.

#### Third-party library upgrades
A number of third party libraries have been upgraded to their latest version. Updates include Jetty 9.3, Tomcat 8.5, Jersey 2.23, Hibernate 5.0, Jackson 2.7, Solr 5.5, Spring Data Hopper, Spring Session 1.2, Hazelcast 3.6, Artemis 1.3, Ehcache 3.1, Elasticsearch 2.3, Spring REST Docs 1.1, Spring AMQP 1.6 & Spring Integration 4.3.

Several Maven plugins were also upgraded.

#### Elasticsearch Jest support
Spring Boot auto-configures a `JestClient` and a dedicated `HealthIndicator` if Jest is on the classpath. This allows you to use `Elasticsearch` even when `spring-data-elasticsearch` isn’t on the classpath.

#### Analysis of startup failures
Spring Boot will now perform analysis of common startup failures and provide useful diagnostic information rather than simply logging an exception and its stack trace.  

If you still want to see the stacktrace of the underlying cause, enable debug logging for `org.springframework.boot.diagnostics.LoggingFailureAnalysisReporter`.

#### Image Banners
You can now use image files to render ASCII art banners. Drop a `banner.gif`, `banner.jpg` or `banner.png` file into `src/main/resources` to have it automatically converted to ASCII. You can use the `banner.image.width` and `banner.image.height` properties to tweak the size, or use `banner.image.invert` to invert the colors.

#### RestTemplate builder
A new `RestTemplateBuilder` can be used to easily create a `RestTemplate` with sensible defaults. By default, the built `RestTemplate` will attempt to use the most suitable `ClientHttpRequestFactory` available on the classpath and will be aware of the `MessageConverter` instances to use (including Jackson). The builder includes a number of useful methods that can be used to quickly configure a `RestTemplate`. For example, to add BASIC auth support you can use:

```java
@Bean
public RestTemplate restTemplate(RestTemplateBuilder builder) {
	return builder.basicAuthorization("user", "secret").build();
}
```

The auto-configured `TestRestTemplate` now uses the `RestTemplateBuilder` as well.


#### JSON Components
A new `@JsonComponent` annotation is now provided for custom Jackson `JsonSerializer` and/or `JsonDeserializer` registration. This can be a useful way to decouple JSON serialization logic:

```java
@JsonComponent
public class Example {

	public static class Serializer extends JsonSerializer<SomeObject> {
		// ...
	}

	public static class Deserializer extends JsonDeserializer<SomeObject> {
		// ...
	}

}
```

Additionally, Spring Boot also now provides `JsonObjectSerializer` and `JsonObjectDeserializer` base classes which provide useful alternatives to the standard Jackson versions when serializing objects. See the [updated documentation](https://docs.spring.io/spring-boot/docs/1.4.x/reference/htmlsingle/#boot-features-json-components) for details.

#### Convention based error pages
Custom error pages for a given status code can now be created by following a convention based approach. Create a static HTML file in `/public/error` or add a template to `/templates/error` using the status code as the filename. For example, to register a custom 404 file you could add `src/main/resource/public/error/404.html`. See [the updated reference documentation](https://docs.spring.io/spring-boot/docs/1.4.x/reference/htmlsingle/#boot-features-error-handling-custom-error-pages) for details.

#### Unified @EntityScan annotation
`org.springframework.boot.autoconfigure.domain.EntityScan` can now be used to specify the packages to use for `JPA`, `Neo4J`, `MongoDB`, `Cassandra` and `Couchbase`. As a result, the JPA-specific `org.springframework.boot.orm.jpa.EntityScan` is now deprecated.

#### ErrorPageRegistry
New `ErrorPageRegistry` and `ErrorPageRegistrar` interfaces allow error pages to be registered in a consistent way regardless of the use of an embedded servlet container. The `ErrorPageFilter` class has been updated to that it is now a `ErrorPageRegistry` and not a fake `ConfigurableEmbeddedServletContainer`.

#### PrincipalExtractor
The `PrincipalExtractor` interface can now be used if you need to extract the OAuth2 `Principal` using custom logic.

#### Test improvements
Spring Boot 1.4 includes a major overhaul of testing support. Test classes and utilities are now provided in dedicated spring-boot-test and spring-boot-test-autoconfigure jars (although most users will continue to pick them up via the spring-boot-starter-test "Starter"). We’ve added AssertJ, JSONassert and JsonPath dependencies to the test starter.

##### @SpringBootTest
With Spring Boot 1.3 there were multiple ways of writing a Spring Boot test. You could use `@SpringApplicationConfiguration`, `@ContextConfiguration` with the `SpringApplicationContextLoader`, `@IntegrationTest` or `@WebIntegrationTest`. With Spring Boot 1.4, a single `@SpringBootTest` annotation replaces all of those.

Use `@SpringBootTest` in combination with `@RunWith(SpringRunner.class)` and set the webEnvironment attribute depending on the type of test you want to write.

A classic integration test, with a mocked servlet environment:
```java
    @RunWith(SpringRunner.class)
    @SpringBootTest
    public class MyTest {

        // ...

    }
```

A web integration test, running a live server listening on a defined port:
```java
    @RunWith(SpringRunner.class)
    @SpringBootTest(webEnvironment=WebEnvionment.DEFINED_PORT)
    public class MyTest {

        // ...

    }
```

A web integration test, running a live server listening on a random port:
```java
    @RunWith(SpringRunner.class)
    @SpringBootTest(webEnvironment=WebEnvionment.RANDOM_PORT)
    public class MyTest {

        @LocalServerPort
        private int actualPort;

        // ...

    }
```
See the [updated reference documentation](https://docs.spring.io/spring-boot/docs/1.4.x/reference/htmlsingle/#boot-features-testing-spring-boot-applications) for details.

##### Auto-detection of test configuration
Test configuration can now be automatically detected for most tests. If you follow the Spring Boot recommended conventions for structuring your code the `@SpringBootApplication` class will be loaded when no explicit configuration is defined. If you need to load a different `@Configuration` class you can either include it as a nested `inner-class` in your test, or use the classes attribute of `@SpringBootTest`.

See [Detecting test configuration](https://docs.spring.io/spring-boot/docs/1.4.x/reference/htmlsingle/#boot-features-testing-spring-boot-applications-detecting-config) for details.

##### Mocking and spying beans
It’s quite common to want to replace a single bean in your ApplicationContext with a mock for testing purposes. With Spring Boot 1.4 this now as easy as annotating a field in your test with `@MockBean`:
```java
    @RunWith(SpringRunner.class)
    @SpringBootTest
    public class MyTest {

        @MockBean
        private RemoteService remoteService;

        @Autowired
        private Reverser reverser;

        @Test
        public void exampleTest() {
            // RemoteService has been injected into the reverser bean
            given(this.remoteService.someCall()).willReturn("mock");
            String reverse = reverser.reverseSomeCall();
            assertThat(reverse).isEqualTo("kcom");
        }

    }
```
You can also use `@SpyBean` if you want to spy on an existing bean rather than using a full mock.

See the [mocking section](https://docs.spring.io/spring-boot/docs/1.4.x/reference/htmlsingle/#boot-features-testing-spring-boot-applications-mocking-beans) of the reference documentation for more details.

##### Auto-configured tests
Full application auto-configuration is sometime overkill for tests, you often only want to auto-configure a specific "slice" of your application. Spring Boot 1.4 introduces a number of specialized test annotations that can be used for testing specific parts of your application:

- `@JsonTest` - For testing JSON marshalling and unmarshalling.

- `@WebMvcTest` - For testing Spring MVC @Controllers using MockMVC.

- `@RestClientTest` - For testing RestTemplate calls.

- `@DataJpaTest` - For testing Spring Data JPA elements

Many of the annotations provide additional auto-configuration that’s specific to testing. For example, if you use `@WebMvcTest` you can `@Autowire` a fully configured `MockMvc` instance.

See the [reference documentation](https://docs.spring.io/spring-boot/docs/1.4.x/reference/htmlsingle/#boot-features-testing-spring-boot-applications-testing-autoconfigured-tests) for details.

##### JSON AssertJ assertions
New `JacksonTester`, `GsonTester` and `BasicJsonTester` classes can be used in combination with AssertJ to test JSON marshalling and unmarshalling. Testers can be used with the `@JsonTest` annotation or directly on a test class:
```java
    @RunWith(SpringRunner.class)
    @JsonTest
    public class MyJsonTests {

        private JacksonTester<VehicleDetails> json;

        @Test
        public void testSerialize() throws Exception {
            VehicleDetails details = new VehicleDetails("Honda", "Civic");
            assertThat(this.json.write(details)).isEqualToJson("expected.json");
            assertThat(this.json.write(details)).hasJsonPathStringValue("@.make");
        }

    }
```
See the [JSON section](https://docs.spring.io/spring-boot/docs/1.4.x/reference/htmlsingle/#boot-features-testing-spring-boot-applications-testing-autoconfigured-json-tests) of the reference documentation or the Javadocs for details.

##### @RestClientTest
The `@RestClientTest` annotation can be used if you want to test REST clients. By default it will auto-configure Jackson and GSON support, configure a `RestTemplateBuilder` and add support for `MockRestServiceServer`.

##### Auto-configuration for Spring REST Docs
Combined with the support for auto-configuring `MockMvc` described above, auto-configuration for Spring REST Docs has been introduced. REST Docs can be enabled using the new `@AutoConfigureRestDocs` annotation. This will result in the `MockMvc` instance being automatically configured to use REST Docs and also removes the need to use REST Docs' JUnit rule. Please see the relevant section of the reference documentation for further details.

##### Test utilities
`spring-boot-starter-test` now brings the [`AssertJ` assertions library](http://joel-costigliola.github.io/assertj/).

Test utilities from the `org.springframework.boot.test` package have been moved to a `spring-boot-test` dedicated artifact.

#### Annotation processing
Apache HttpCore 4.4.5 removed a handful of annotations. This is a binary incompatible change if you are using an annotation processor and are sub-classing a class that uses one of the removed annotations. For example, if the class was using `@Immutable` you will see compile-time annotation processing fail with `[ERROR] diagnostic: error: cannot access org.apache.http.annotation.Immutable`.

The problem can be avoided by downgrading to HttpCore 4.4.4 or, preferably, by structuring your code so that the problematic class is not subject to compile-time annotation processing.

#### Miscellaneous
- `server.jetty.acceptors` and `server.jetty.selectors` properties have been added to configure the number of Jetty acceptors and selectors.

- `server.max-http-header-size` and `server.max-http-post-size` can be used to constrain maximum sizes for HTTP headers and HTTP POSTs. Settings work on Tomcat, Jetty and Undertow.

- The minimum number of spare threads for Tomcat can now be configured using `server.tomcat.min-spare-threads`

- Profile negation in now supported in `application.yml` files. Use the familiar `!` prefix in `spring.profiles` values

- The actuator exposes a new `headdump` endpoint that returns a GZip compressed `hprof` heap dump file

- Spring Mobile is now auto-configured for all supported template engines

- The Spring Boot maven plugin allows to bundle system scoped artifacts using the new `includeSystemScope` attribute

- `spring.mvc.log-resolved-exception` enables the automatic logging of a warning when an exception is resolved by a `HandlerExceptionResolver`

- `spring.data.cassandra.schema-action` you be used to customize the schema action to take on startup

- Spring Boot’s fat jar format should now consume much less memory

- Locale to Charset mapping is now supported via the `spring.http.encoding.mapping.<locale>=<charset>` property

- By default, the locale configured using `spring.mvc.locale` is now overridden by a request’s Accept-Language header. To restore 1.3’s behaviour where the header is ignored, set `spring.mvc.locale-resolver` to fixed.
  
### Deprecations in Spring Boot 1.4
- - -

- Velocity support has been deprecated since support has been deprecated as of Spring Framework 4.3.

- Some constructors in `UndertowEmbeddedServletContainer` have been deprecated (most uses should be unaffected).

- The `locations` and `merge` attributes of the `@ConfigurationProperties` annotation have been deprecated in favor of directly configuring the `Environment`.

- The protected `SpringApplication.printBanner` method should no longer be used to print a custom banner. Use the `Banner` interface instead.

- The protected `InfoEndpoint.getAdditionalInfo` method has been deprecated in favor of the `InfoContributor` interface.

- `org.springframework.boot.autoconfigure.test.ImportAutoConfiguration` has been moved to `org.springframework.boot.autoconfigure`.

- All classes in the `org.springframework.boot.test` package have been deprecated. See the "upgrading" notes above for details.

- `PropertiesConfigurationFactory.setProperties(Properties)` is deprecated in favor of using `PropertySources`.

- Several classes in the `org.springframework.boot.context.embedded` package have been deprecated and relocated to `org.springframework.boot.web.servlet`.

- All classes in the `org.springframework.boot.context.web` package have been deprecated and relocated.

- The `spring-boot-starter-ws` "Starter" has been renamed to `spring-boot-starter-web-services`.

- The `spring-boot-starter-redis` "Starter" has been renamed to `spring-boot-starter-data-redis`.

- The `spring-boot-starter-hornetq` starter and auto-configuration has been deprecated in favour of using `spring-boot-starter-artemis`

- `management.security.role` has been deprecated in favour of `management.security.roles`

- The `@org.springframework.boot.orm.jpa.EntityScan` annotation has been deprecated in favor of `@org.springframework.boot.autoconfigure.domain.EntityScan` or explicit configuration.

- `TomcatEmbeddedServletContainerFactory.getValves()` has been deprecated in favor of `getContextValves()`.

- `org.springframework.boot.actuate.system.EmbeddedServerPortFileWriter` has been deprecated in favor of `org.springframework.boot.system.EmbeddedServerPortFileWriter`

- `org.springframework.boot.actuate.system.ApplicationPidFileWriter` has been deprecated in favor of `org.springframework.boot.system.ApplicationPidFileWriter`

### Property Renames
- - -

- `spring.jackson.serialization-inclusion` should be replaced with `spring.jackson.default-property-inclusion`.

- `spring.activemq.pooled` should be replaced with `spring.activemq.pool.enabled`.

- `spring.jpa.hibernate.naming-strategy` should be replaced with `spring.jpa.hibernate.naming.strategy`.

- `server.tomcat.max-http-header-size` should be replaced with `server.max-http-header-size`.

## Spring Boot 1.5 Release Notes

### Upgrading from Spring Boot 1.4
- - -

#### Deprecations from Spring Boot 1.4
Classes, methods and properties that were deprecated in Spring Boot 1.4 have been removed in this release. Please ensure that you aren’t calling deprecated methods before upgrading. In particular, the HornetQ and Velocity support have been removed.

#### Renamed starters
The following starters that were renamed in Spring Boot 1.4 have now been removed, if you get "unresolved dependency" errors please check that you are pulling in the correctly named starter:

- `spring-boot-starter-ws` → `spring-boot-starter-web-services`

- `spring-boot-starter-redis` → `spring-boot-starter-data-redis`

#### @ConfigurationProperties validation
If you have `@ConfigurationProperties` classes that use JSR-303 constraint annotations, you should now additionally annotate them with `@Validated`. Existing validation will currently continue to work, however, a warning will be logged. In the future, classes without `@Validated` will not be validated at all.

#### spring.jpa.database
The `spring.jpa.database` can now be auto-detected for common databases from the `spring.datasource.url` property. If you’ve manually defined `spring.jpa.database`, and you use a common database, you might want to try removing the property altogether.

Several database have more than one `Dialect` (for instance, Microsoft SQL Server has 3) so we might configure one that doesn’t match the version of the database you are using. If you had a working setup before and would like to rely on Hibernate to auto-detect the `Dialect`, set `spring.jpa.database=default`. Alternatively, you can always set the dialect yourself using the `spring.jpa.database-platform` property.

#### @IntegrationComponentScan
Spring Integration’s `@IntegrationComponentScan` annotation is now auto-configured. If you follow the recommended project structure, you should try removing it.

#### ApplicationStartedEvent
If you currently listen for an `ApplicationStartedEvent` in your code you should refactor to use `ApplicationStartingEvent`. We renamed this class to more accurately reflect what it does.

#### Spring Integration Starter
The `spring-boot-starter-integration` POM no longer includes `spring-integration-jmx`. If you require Spring Integration JMX support you should include a `spring-integration-jmx` dependency yourself.

#### Devtools excluded by default
Both the Maven and Gradle plugins now by default exclude packaging of the `spring-boot-devtools` jar in "fat" jars. If you are using devtools remote support you will now need to explicitly set the excludeDevtools property in your `build.gradle` or `pom.xml` file.

#### OAuth 2 Resource Filter
The default order of the OAuth2 resource filter has changed from `3` to `SecurityProperties.ACCESS_OVERRIDE_ORDER - 1`. This places it after the actuator endpoints but before the basic authentication filter chain. The default can be restored by setting `security.oauth2.resource.filter-order = 3`

#### JSP servlet
The JSP servlet is no longer in development mode by default. Development mode is automatically enabled when using DevTools. It can also be enabled explicitly by setting `server.jsp-servlet.init-parameters.development=true`.

#### Ignored paths and @EnableWebSecurity
In Spring Boot 1.4 and earlier, the Actuator would always configure some ignored paths irrespective of the use of `@EnableWebSecurity`. This has been corrected in 1.5 so that using `@EnableWebSecurity` will switch off all auto-configuration of web security thereby allowing you to take complete control.

### New and Noteworthy
- - -

Tip: Check [the configuration changelog](https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-1.5-Configuration-Changelog) for a complete overview of the changes in configuration.

#### Third-party library upgrades
A number of third party libraries have been upgraded to their latest version. Updates include Spring Data Ingalls, Jetty 9.4, JooQ 3.9, AssertJ 2.6.0, Hikari 2.5 and Neo4J 2.1. Several Maven plugins have also been upgraded.

#### Apache Kafka support
Spring Boot 1.5 include auto-configuration support for Apache Kafka via the [spring-kafka](https://spring.io/projects/spring-kafka) project. To use Kafka simply include the `spring-kafka'depenency and configure the appropriate'spring.kafka.*` application properties.

Recieving messages from Kafka is as simple as annotating a method:

```java
    @Component
    public class MyBean {

        @KafkaListener(topics = "someTopic")
        public void processMessage(String content) {
            // ...
        }

    }
```

#### Transaction manager properties
It’s now possible to configure various aspects of an auto-configured `PlatformTransactionManager` using `spring.transaction.*` properties. Currently the "default-timeout" and `rollback-on-commit-failure` properties are supported.

#### Deprecation level
It is now possible to define a deprecation level for a property in the metadata. The level can either be `warning` (default) or `error`.

Here is an example of manual metadata for a property that got moved in Spring Boot 2:
```json
    {
        "name": "server.context-parameters",
        "type": "java.util.Map<java.lang.String,java.lang.String>",
        "description": "ServletContext parameters.",
        "deprecation": {
            "level": "error",
            "replacement": "server.servlet.context-parameters"
        }
    }
```
So far, when a deprecated property is not bound anymore, we remove the metadata altogether. This new feature allows to flag a property (`server.context-parameters` here) as an error. Newer versions of your favorite IDE should use that to offer assistance.

#### Testing updates
It’s now possible to exclude auto-configuration that would usually be imported by a `@Test…​` annotation. All existing `@Test…​` annotations now include a `excludeAutoConfiguration` attribute. Alternatively, you can add `@ImportAutoConfiguration(exclude=…​)` directly to your tests.

Spring Boot 1.5 also introduces a new `@JdbcTest` annotation that can be used to test direct JDBC interactions.

#### Custom fat jar layouts
The Spring Boot Maven and Gradle plugins now support custom fat jar layouts. This feature allows experimental layouts [such as this one](https://github.com/dsyer/spring-boot-thin-launcher) to be developed outside of Spring Boot. For more details, see [the updated documentation](http://docs.spring.io/spring-boot/docs/1.5.x-SNAPSHOT/reference/htmlsingle/#build-tool-plugins-gradle-configuration-custom-repackager).

#### JmsTemplate customizations
It is now possible to customize the auto-configured `JmsTemplate` using additional keys available in the `spring.jms.template.*` namespace.

#### Miscellaneous
- Mockito 2.x can now be used with `@MockBean` (compatibility with Mockito 1.9 remains)

- The embedded launch script now supports a `force-stop`

- A new health check for Cassandra has been added

- Cassandra user defined types are now resolved (via Spring Data’s `SimpleUserTypeResolver`)

- The `skip` property now works for the Spring Boot Maven Plugin `run`, 'stop' and 'repackage` goals

- If multiple `main` method classes are found, the Maven and Gradle plugins will now automatically use the one annotated with `@SpringBootApplication`

### Deprecations in Spring Boot 1.5
- - -
  
- `TomcatEmbeddedServletContainerFactory.setTldSkip` has been deprecated in favor of `setTldSkipPatterns`

- `ApplicationStartedEvent` has been replaced by `ApplicationStartingEvent`

- Several constants in `LoggingApplicationListener` have been replaced by versions in `LogFile`

- Caching with Guava has been deprecated since Guava support will be dropped in Spring Framework 5. Upgrade to Caffeine

- CRaSH support has been deprecated since it’s no longer actively maintained

- Several protected methods in `EndpointMBeanExporter` have been deprecated following the introduction of `JmxEndpoint`

- `SearchStrategy.PARENTS` has been replaced with `SearchStrategy.ANCESTORS`.

- Apache DBCP support has been deprecated in favor of DBCP 2

- The `server.undertow.buffers-per-region` property has been deprecated because it is not used (see UNDERTOW-587)

- `@AutoConfigureTestDatabase` has been moved from `org.springframework.boot.test.autoconfigure.orm.jpa` to `org.springframework.boot.test.autoconfigure.jdbc`

### Property Renames
- - -
- The `server.max-http-post-size` property has been replaced by technology specific variants (for example `server.tomcat.max-http-post-size`)

- The `spring.data.neo4j.session.scope` property has been removed.