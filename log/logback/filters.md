# [Filters](https://logback.qos.ch/manual/filters.html)

# In logback-classic

Logback-classic offers two types of filters, regular filters and turbo filters.

## Regular filters

### LevelFilter

`LevelFilter`过滤器基于精确（exact）的`level`匹配。如果`event's level`等于配置的`level`，过滤器基于配置文件中的`onMatch`和`onMissmatch`属性接受（accepts）或者拒绝（denies）该事件。示例配置如下：

```xml
<configuration>
    <appender name="CONSOLE" class="ch.qos.logback.core.ConsoleAppender">
        <filter class="ch.qos.logback.classic.filter.LevelFilter">
        <level>INFO</level>
        <onMatch>ACCEPT</onMatch>
        <onMismatch>DENY</onMismatch>
        </filter>
        <encoder>
        <pattern>
            %-4relative [%thread] %-5level %logger{30} - %msg%n
        </pattern>
        </encoder>
    </appender>
    <root level="DEBUG">
        <appender-ref ref="CONSOLE" />
    </root>
</configuration>
```

### ThresholdFilter

`ThresholdFilter`过滤低于指定阈值的事件（events）。对于级别等于或高于阈值的事件，`ThresholdFilter`将在调用其`decision()`方法时响应`NEUTRAL`。但是，将拒绝级别低于阈值的事件。示例配置如下：

```xml
<configuration>
  <appender name="CONSOLE"
    class="ch.qos.logback.core.ConsoleAppender">
    <!-- deny all events with a level below INFO, that is TRACE and DEBUG -->
    <filter class="ch.qos.logback.classic.filter.ThresholdFilter">
      <level>INFO</level>
    </filter>
    <encoder>
      <pattern>
        %-4relative [%thread] %-5level %logger{30} - %msg%n
      </pattern>
    </encoder>
  </appender>
  <root level="DEBUG">
    <appender-ref ref="CONSOLE" />
  </root>
</configuration>
```

### EvaluatorFilter

`EvaluatorFilter`是封装（encapsulating）`EventEvaluator`的通用过滤器。顾名思义，`EventEvaluator`会评估给定事件是否满足给定条件。在匹配和不匹配时，托管(hosting)的`EventEvaluator`将分别返回`onMatch`和`omMissmatch`属性指定的值。

请注意，`EvaluatorFilter`是一个抽象类。你可以通过对`EventEvaluator`进行子类化来实现自己的事件评估逻辑。

#### GEventEvaluator

#### JaninoEventEvaluator

`Logback-classic`附带另一个名为`JaninoEventEvaluator`的具体`EventEvaluator`实现，它使用任意`java`语言块(block)返回一个布尔值作为评估标准。我们将这种Java语言布尔表达式称为"evaluation expressions"。评估表达式使事件过滤具有很大的灵活性。

Note：`JaninoEventEvaluator`需要`Janino`类库，maven的`pom`需要显示添加`Janino`类库的依赖，否则会无法启动，也没有任何启动失败的提示。

与`JaninoEventEvaluator`相比，`GEventEvaluator`凭借`Groovy`语言的优点使用起来非常方便，但是对于等效表达式（equivalent expressions），`JaninoEventEvaluator`通常会运行的更快。

示例配置如下：

```xml
<configuration>

  <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <filter class="ch.qos.logback.core.filter.EvaluatorFilter">      
      <evaluator> <!-- defaults to type ch.qos.logback.classic.boolex.JaninoEventEvaluator -->
        <expression>return message.contains("billing");</expression>
      </evaluator>
      <OnMismatch>NEUTRAL</OnMismatch>
      <OnMatch>DENY</OnMatch>
    </filter>
    <encoder>
      <pattern>
        %-4relative [%thread] %-5level %logger - %msg%n
      </pattern>
    </encoder>
  </appender>

  <root level="INFO">
    <appender-ref ref="STDOUT" />
  </root>
</configuration>
```

#### Matchers

虽然可以通过调用Spring类中的`matches()`方法来进行模式匹配，但是每次调用过滤器时都会产生编译全新`Pattern`对象的成本。要消除此开销，可以预定义一个或多个`Matcher`对象。一旦定义了匹配器，就可以在评估器表达式中按名称重复引用它。

```xml
<configuration debug="true">

  <appender name="STDOUT" class="ch.qos.logback.core.ConsoleAppender">
    <filter class="ch.qos.logback.core.filter.EvaluatorFilter">
      <evaluator>        
        <matcher>
          <Name>odd</Name>
          <!-- filter out odd numbered statements -->
          <regex>statement [13579]</regex>
        </matcher>
        
        <expression>odd.matches(formattedMessage)</expression>
      </evaluator>
      <OnMismatch>NEUTRAL</OnMismatch>
      <OnMatch>DENY</OnMatch>
    </filter>
    <encoder>
      <pattern>%-4relative [%thread] %-5level %logger - %msg%n</pattern>
    </encoder>
  </appender>

  <root level="DEBUG">
    <appender-ref ref="STDOUT" />
  </root>
</configuration>
```



## Turbo filters