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