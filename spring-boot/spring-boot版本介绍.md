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
Auto-configuration support is now available for Elasticseach, along with a new **spring-boot-starter-data-elasticsearch** starter POM.

##### Apache Solr
Auto-configuration support is now available for Apache Solr, along with a new **spring-boot-starter-data-solr** starter POM.

#### Web Improvements
Auto-configuration classes for **Spring HATEOAS'' and** Spring Data Rest MVC'' have now been added to Spring Boot. Multi-part file upload support has been changed to be enabled by default.

#### Spring Integration
Auto-configuration has been added for Spring Integration. If Spring Integration is on your classpath you will now automatically get **@EnableIntegration** as well as some Spring Integration JMX beans.

#### Jackson
Support for the Jackson JSON marshaling library has been improved. You will now get auto-configured beans for Jackson’s **ObjectMapper** as well as **JodaModule** and **JSR310Module** support when appropriate. You can now also add your own **Module** beans and have them automatically applied.

#### HikariDataSource
Support has been added for the [Hikari Connection Pool](https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-1.1-Release-Notes) library. Simply ensure that the HikariCP library is on your classpath and that the Tomcat DBCP is not.

#### New Conditional Annotations
Two new **@Conditional** annotations have been added to Spring Boot 1.1. **@ConditionalOnProperty** supports conditionally enabling beans based on the presence of an **Environment** property and **@ConditionalOnJava** can be used to enable beans based on the JVM version.

## Spring Boot 1.2 Release Notes

### Upgrading from Spring Boot 1.1
- - -

#### Servlet 3.1, Tomcat 8 and Jetty 9
Spring Boot now uses Tomcat 8 and Jetty 9 as embedded servlet containers. This provides Servlet 3.1 and enhanced WebSocket support out of the box. You can still use Tomcat 7 or Jetty 8 if you prefer to stick to an older version. See the spring-boot-sample-tomcat7-jsp and spring-boot-sample-jetty8 samples for downgrade examples.

#### Logging output
The default logging configuration has been updated with Spring Boot 1.2 to no longer write log files. If you want file output, you can use the **logging.path** or **logging.file** properties. You can still also completely customize logging by adding your own **logback.xml** file.

#### Consistent HTTP URI/body decoding
A **CharacterEncodingFilter** is now registered automatically for consistent URI/body decoding. You can use the **spring.http.encoding.charset** property if you need something other than UTF-8 or set **spring.http.encoding.enabled** to false if you don’t want the **CharacterEncodingFilter** registered at all.

#### IgnoreDefaultModelOnRedirect with Spring MVC
Spring MVC auto-configuration now sets the **ignoreDefaultModelOnRedirect** property of **RequestMappingHandlerAdapter** to **true** by default. If you need model attributes to be part of your redirect URLs you can add the following to your **application.properties**:
> spring.mvc.ignore-default-model-on-redirect=false

#### Jackson defaults
The auto-configured Jackson **ObjectMapper** now has **MapperFeature.DEFAULT_VIEW_INCLUSION** and **DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES** disabled. If you need to revert to the previous settings you can add the following to your **application.properties**:
> spring.jackson.mapper.default-view-inclusion=true
> spring.jackson.deserialization.fail-on-unknown-properties=true

#### Moved health.* properties to management.health.*
Health-related configuration keys have been moved from **health** to **management.health** for consistency with other management-related properties.

#### PersistenceExceptionTranslationPostProcessor
A **PersistenceExceptionTranslationPostProcessor** will now be registered by default. If you don’t want exception translation set the **spring.dao.exceptiontranslation.enabled** property to **false**.

#### Anonymous health access restrictions
The /health actuator endpoint now restricts anonymous access. When accessed anonymously the endpoint hides the health details and simply indicates if the server is UP or DOWN. It also caches the response for the period specified by endpoints.health.time-to-live. These restrictions can be disabled, thereby restoring 1.1.x’s behavior, by setting endpoints.health.sensitive to false.

#### Spring 4.1
Spring Boot 1.2 requires Spring Framework 4.1.5 or later and is not compatible with Spring Framework 4.0.

#### Hikari CP
The version of **com.zaxxer:HikariCP** that’s provide by Boot’s dependency management now requires Java 8. If you’re running on Java 6 or Java 7 you should update your dependency to use **com.zaxxer:HikariCP-java6** instead.

#### Deprecations
- The **org.springframework.boot.actuate.system.ApplicationPidListener** class has been superseded by **ApplicationPidFileWriter**

- The CLI **@EnableRabbitMessaging** annotation has been superseded by **@EnableRabbit** from the **spring-rabbit** project.

- The **http.mappers.** properties are deprecated in favor of their **spring.jackson.serialization.** equivalents.

- **org.springframework.boot.json.SimpleJsonParser** has been deprecated in favor of **BasicJsonParser** to avoid confusion with the **``JSON Simple''** library.

### New and Noteworthy
- - -

#### Version Updates
Spring Boot 1.2 builds on and requires Spring Framework 4.1. Several 3rd party dependencies have been upgraded with this release including Jackson, Joda Time and Hibernate Validator. Tomcat 8 and Jetty 9 are now the default embedded servlet containers (providing Servlet 3.1 support).

#### @SpringBootApplication annotation
A new **@SpringBootApplication** convenience annotation has been added which is equivalent to **@Configuration** + **@EnableAutoConfiguration** + **@ComponentScan**. If you find you use frequently use those three annotations, you might want to consider swapping.

#### JTA Support
Spring Boot 1.2 now supports distributed JTA transactions across multiple XA resources using either an Atomkos or Bitronix embedded transaction manager. JTA transactions are also supported when deploying to a suitable Java EE Application Server.

When a JTA environment is detected, Spring’s **JtaTransactionManager** will be used to manage transactions. Auto-configured JMS, DataSource and JPA beans will be upgraded to support XA transactions. You can use standard Spring idioms such as **@Transactional** to participate in a distributed transaction.

Additionally, general support classes are provided to make Atomkos and Bitronix easier to configure, even if you are not using **@EnableAutoConfiguration**. See the JTA section of the reference manual for details.

#### JNDI Lookups
If you are using a full Java EE Application Server you can now lookup both **DataSource** and JMS **ConnectionFactory** beans from JNDI. Use the **spring.datasource.jndi-name** and **spring.jms.jndi-name** properties in your **application.properties** or **application.yml** file.

#### Jackson customization
It is now possible to [customize the Jackson ObjectMapper](https://docs.spring.io/spring-boot/docs/1.2.x/reference/htmlsingle/#howto-customize-the-jackson-objectmapper) using **spring.jackson** properties. Jackson’s **SerializationFeature**, **DeserializationFeature**, **MapperFeature**, **JsonParser.Feature** and **JsonGenerator.Feature objects** can be customized using **serialization**, **deserialization**, **mapper**, **parser** and **generator** properties. For example to enable **DeserializationFeature.FAIL_ON_UNKNOWN_PROPERTIES** you could add the following to your **application.properties**:
> spring.jackson.deserialization.fail-on-unknown-properties=true

Additionally **spring.jackson.date-format** and **spring.jackson.property-naming-strategy** properties have also been added with Spring Boot 1.2

#### Banner Interface
A new **Banner** interface has been added which can be used in combination with **SpringApplication.setBanner(…​)** to provide customized banner output. The use of **src/main/resources/banner.txt** is still recommended for simple banners, however, the new interface is useful if you need to do something fancy.

#### Banner Properties
You can now use **${application.version}**, **${spring-boot.version}**, **${application.formatted-version}** and **${spring-boot.formatted-version}** variables in your **src/main/resources/banner.txt** file to print your application version and/or the version of Spring Boot. The **formatted-version** is enclosed in brackets and includes a **v** prefix, for example, **(v1.2.0.RELEASE)**.

#### JMS Support
Auto-configuration is now provided for Spring Framework 4.1’s @JmsListener annotation. The @EnableJms annotation is also auto-configured whenever you have spring-jms.jar on your classpath.

#### Spring Cloud Connectors
Auto-configuration has been added for **spring-cloud-connectors** and a new **spring-boot-starter-cloud-connectors** POM has also been included with this release. Auto-configuration provides equivalent functionality to the **@CloudScan** annotation.

#### Conditions
The **@ConditionalOnProperty** annotation has been updated with new **havingValue** and **matchIfMissing** attributes. You can now use the condition to create much more complex property matching conditions. A new **AnyNestedCondition** class has also been added which can be used to compose other **@Conditions**. Finally, the **@ConditionalOnBean** annotation now supports types declared as a String attribute as well as a Class.

#### GSON Support
It is now possible to use GSON instead of Jackson to create JSON output. Jackson remains the default and recommended choice. If you use GSON you should exclude Jackson libraries from your Maven/Gradle build unless your are using Spring Boot’s Actuator which requires Jackson. As of Spring Boot 1.2.2, if both Gson and Jackson are on the classpath, you can configure your application to use Gson by setting **spring.http.converters.preferred-json-mapper** to **gson**.

#### Log4j2
Log4J is now supported as an alternative logging system and **spring-boot-starter-log4j2** start POM is provided. Logback remains the default and recommended logging system.

#### Jersey auto-configuration
Auto-configuration support is now provided for Jersey. Refer to reference documentation for details.

#### Apache-Commons DBCP2 Support
The Apache **commons-dbcp2** database connection pool library is now supported in addition to Tomcat, Hikari and DBCP (v1).

#### Maven Plugin
The **repackage** task of the **spring-boot-maven-plugin** can now be disabled. This can be useful if you want to use **spring-boot:run** but you don’t need ``Fat Jars''.

#### Configuration meta-data
The **spring-boot**, **spring-boot-autoconfigure** and **spring-boot-actuator** jars now include additional meta-data files which can be used by tools developers to provide **`code completion'' support in `application.properties** files. An annotation processor is also provided to allow you to generate your own files automatically from **@ConfigurationProperties** classes.



