This document is meant to help you migrate your application to Spring Boot 2.0 by providing thematic sections that mirror the developer guide.

# Before You Start

### Upgrade to the latest 1.5.x version

Before you start the upgrade, make sure to upgrade to the latest 1.5.x available version. This will make sure that you are building against the most recent dependencies of that line.

### Review dependencies

The move to Spring Boot 2 will upgrade a number of dependencies and might require work on your end. You can review [dependency management for 1.5.x](https://docs.spring.io/spring-boot/docs/1.5.x/reference/html/appendix-dependency-versions.html) with [dependency management for 2.0.x](https://docs.spring.io/spring-boot/docs/2.0.x/reference/html/appendix-dependency-versions.html) to asses how your project is affected.

You may also use dependencies that are not managed by Spring Boot (e.g. Spring Cloud). As your project defines an explicit version for those, you need first to identify the compatible version before upgrading.

### Review custom configuration
Any user-configuration that your project defines might need to be reviewed on upgrade. If this can be replaced by use of standard auto-configuration, do it so before upgrading.

### Review system requirements
**Spring Boot 2.0 requires Java 8 or later**. Java 6 and 7 are no longer supported. It also requires Spring Framework 5.0.

## Upgrade to Spring Boot 2

Once you have reviewed the state of your project and its dependencies, upgrade to the latest maintenance release of Spring Boot 2.0. In particular, do not upgrade to Spring Boot `2.0.0.RELEASE` as a number of issues have been reported and fixed.

We also recommend to upgrade in phases and not in one jump to the latest GA: first upgrade to `2.0`, then `2.1`, etc.

### Configuration properties migration

With Spring Boot 2.0, many configuration properties were renamed/removed and developers need to update their `application.properties/application.yml` accordingly. To help you with that, Spring Boot ships a new `spring-boot-properties-migrator` module. Once added as a dependency to your project, this will not only analyze your application’s environment and print diagnostics at startup, but also temporarily migrate properties at runtime for you. This is a must have during your application migration:

```xml
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-properties-migrator</artifactId>
        <scope>runtime</scope>
    </dependency>
```

```
    runtime("org.springframework.boot:spring-boot-properties-migrator")
```

Note: Once you’re done with the migration, please make sure to remove this module from your project’s dependencies.

### Next steps

If you wish to look into specifics, here’s a curated list of resources - otherwise, proceed to the next sections:

- [Spring Boot 2.0.0 Release Notes](https://github.com/spring-projects/spring-boot/wiki/Spring-Boot-2.0-Release-Notes)

- [Running Spring Boot on Java 9](https://github.com/spring-projects/spring-boot/wiki)

- [Upgrading to Spring Framework 5.0](https://github.com/spring-projects/spring-framework/wiki/Upgrading-to-Spring-Framework-5.x#upgrading-to-version-50)


## Spring Boot Features

### Default Proxying Strategy

Spring Boot now uses CGLIB proxying by default, including for the AOP support. If you need interface-based proxy, you’ll need to set the `spring.aop.proxy-target-class` to `false`.

### SpringApplication

#### Web Environment
Spring Boot applications can now operate in more modes so `spring.main.web-environment` property is now deprecated in favor of `spring.main.web-application-type` that provides more control.

If you want to make sure an application doesn’t start a web server you’d have to change the property to:

> spring.main.web-application-type=none

Tip: there is also a `setWebApplicationType` on `SpringApplication` if you want to do that programmatically.

#### Spring Boot Application Events Changes
We’ve added a new event, `ApplicationStartedEvent`. `ApplicationStartedEvent` is sent after the context has been refreshed but before any application and command-line runners have been called. `ApplicationReadyEvent` is sent after any application and command-line runners have been called. It indicates that the application is ready to service requests.

See the [updated reference documentation](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/reference/html/spring-boot-features.html#boot-features-application-events-and-listeners).

#### Banner
In our effort to limit the number of root namespaces that Spring Boot uses, banner-related properties have been relocated to `spring.banner`.

### Externalized Configuration

#### Relaxed Binding
The rules related to relaxed binding [have been tightened](https://docs.spring.io/spring-boot/docs/current-SNAPSHOT/reference/html/spring-boot-features.html#boot-features-external-config-relaxed-binding)

This new relaxed bindings as several advantages:

- There is no need to worry about the structure of the key in `@ConditionalOnProperty`: you must now use the canonical format (`acme.my-property` and not `acme.myProperty`), the supported relaxed variants will work transparently. If you were using the `prefix` attribute you can now simply put the full key using the `name` or `value` attributes.

- `RelaxedPropertyResolver` is no longer available as the Environment takes care of that automatically: `env.getProperty("com.foo.my-bar")` will find a `com.foo.myBar` property.

The `org.springframework.boot.bind` package is no longer available and is replaced by the [new relaxed binding infrastructure](https://github.com/spring-projects/spring-boot/wiki/Relaxed-Binding-2.0). In particular, `RelaxedDataBinder` and friends have been replaced with a new `Binder` API. The following samples binds MyProperties from the `app.acme` prefix.
```java
MyProperties target = Binder.get(environment)
		.bind("app.acme", MyProperties.class)
		.orElse(null);
```
As relaxed binding is now built-in, you can request any property without having to care about the case as long as it’s using one of the supported formats:
```java
FlagType flagType = Binder.get(environment)
		.bind("acme.app.my-flag", FlagType.class)
		.orElse(FlagType.DEFAULT);
```

#### Binding on static methods
While binding on static properties (using a `static` getter and setter pair) works in Spring Boot `1.x`, we never really intended to provide such feature and it is no longer possible as of Spring Boot 2.

#### `@ConfigurationProperties` Validation
It is now mandatory that your `@ConfigurationProperties` object is annotated with @Validated if you want to turn on validation.

#### Configuration Location
The behavior of the `spring.config.location` configuration has been fixed; it previously added a location to the list of default ones, now it replaces the default locations. If you were relying on the way it was handled previously, you should now use `spring.config.additional-location` instead.