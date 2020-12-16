# quartz

## 属性文件

quartz使用名为`quartz.properties`的配置文件

基础配置

```properties
org.quartz.scheduler.instanceName=MyScheduler
# 线程池中有3个线程，这意味着最多可以同时运行3个job
org.quartz.threadPool.threadCount=3
# quartz的所有数据，包括job和trigger的配置，都会存储在内存中（而不是数据库里）
org.quartz.jobStore.class=org.quartz.simpl.RAMJobStore
```

## 基本实现

需要三个对象Scheduler、JobDetail、Trigger

## 集群和线程池配置文件

项目配置文件`application.yml`内容

```yml
server:
    port: 8115

spring:
    jackson:
        time-zone: GMT+8
    application:
        name: scheduler-job
    quartz:
        #数据库方式
        job-store-type: jdbc
        properties:
            org:
                quartz:
                    scheduler:
                        instanceId: AUTO
                        instanceName: youlin-scheduler-job
                    jobStore:
                        class: org.quartz.impl.jdbcjobstore.JobStoreTX
                        clusterCheckinInterval: 10000
                        driverDelegateClass: org.quartz.impl.jdbcjobstore.StdJDBCDelegate
                        isClustered: true
                        tablePrefix: QRTZ_
                        useProperties: false
                        dataSource: schedulerDS
                    dataSource:
                        schedulerDS:
                            provider: hikaricp
                            driver: com.mysql.cj.jdbc.Driver
                            URL: jdbc:mysql://192.168.1.203:3309/scheduler-job?useUnicode=true&characterEncoding=utf8&zeroDateTimeBehavior=convertToNull&serverTimezone=CTT&useSSL=false
                            user: root
                            password: 123456
                            maxConnections: 15
                    threadPool:
                        class: org.quartz.simpl.SimpleThreadPool
                        threadCount: 15
                        threadPriority: 5
                        threadsInheritContextClassLoaderOfInitializingThread: true
logging:
    config: classpath:config/log4j2-spring.xml
```

quartz配置文件`quartz.properties`内容

```properties
#调度标识名 集群中每一个实例都必须使用相同的名称
org.quartz.scheduler.instanceName=scheduler
#调度器实例编号自动生成，每个实例不能不能相同
org.quartz.scheduler.instanceId=AUTO
#开启分布式部署，集群
org.quartz.jobStore.isClustered=true
#分布式节点有效性检查时间间隔，单位：毫秒,默认值是15000
org.quartz.jobStore.clusterCheckinInterval=15000

#远程管理相关的配置,全部关闭
org.quartz.scheduler.rmi.export=false
org.quartz.scheduler.rmi.proxy=false

org.quartz.scheduler.wrapJobExecutionInUserTransaction=false
#实例化ThreadPool时，使用的线程类为SimpleThreadPool（一般使用SimpleThreadPool即可满足几乎所有用户的需求）
org.quartz.threadPool.class=org.quartz.simpl.SimpleThreadPool
##并发个数,指定线程数，至少为1（无默认值）(一般设置为1-100之间的的整数合适)
org.quartz.threadPool.threadCount=5
##设置线程的优先级（最大为java.lang.Thread.MAX_PRIORITY 10，最小为Thread.MIN_PRIORITY 1，默认为5）
org.quartz.threadPool.threadPriority=5
#org.quartz.threadPool.threadsInheritContextClassLoaderOfInitializingThread=true
#容许的最大作业延长时间,最大能忍受的触发超时时间，如果超过则认为“失误”,不管在内存中还是数据中都要配置，默认值为60000
org.quartz.jobStore.misfireThreshold=60000

#持久化方式配置
# 默认存储在内存中，保存job和Trigger的状态信息到内存中的类
#org.quartz.jobStore.class=org.quartz.simpl.RAMJobStore
#数据库方式
org.quartz.jobStore.class=org.quartz.impl.jdbcjobstore.JobStoreTX
#持久化方式配置数据驱动，MySQL数据库
org.quartz.jobStore.driverDelegateClass=org.quartz.impl.jdbcjobstore.StdJDBCDelegate
#quartz相关数据表前缀名
org.quartz.jobStore.tablePrefix=QRTZ_

#数据库别名 随便取
#org.quartz.jobStore.dataSource=qzDS
#org.quartz.dataSource.qzDS.driver=com.mysql.jdbc.Driver
#org.quartz.dataSource.qzDS.URL=jdbc:mysql://192.168.1.203:3306/quartzdb?useSSL=false&useUnicode=true&characterEncoding=UTF-8
#org.quartz.dataSource.qzDS.user=root
#org.quartz.dataSource.qzDS.password=123456
#org.quartz.dataSource.qzDS.maxConnections=10
#org.quartz.dataSource.qzDS.acquireIncrement=1
```

## 集群改造

### 创建数据库

使用官方提供的脚本`tables_mysql_innodb.sql`创建`quartz`组件需要的数据库

### 修改youlin-scheduler-job依赖

pom文件中引入数据库线程池依赖`druid-spring-boot-starter`和quartz依赖`spring-boot-starter-quartz`

```xml
    <dependency>
        <groupId>com.alibaba</groupId>
        <artifactId>druid-spring-boot-starter</artifactId>
        <version>1.1.10</version>
    </dependency>
    <dependency>
        <groupId>org.springframework.boot</groupId>
        <artifactId>spring-boot-starter-quartz</artifactId>
    </dependency>
```

### 修改配置文件

配置文件中增加druid线程池和quartz的配置

```yml
spring:
    application:
        name: youlin-youlin-scheduler-job
    datasource:
        driver-class-name: com.mysql.cj.jdbc.Driver
        password: 123456
        platform: mysql
        type: com.alibaba.druid.pool.DruidDataSource
        url: jdbc:mysql://192.168.43.131:3306/youlin-scheduler-job?useUnicode=true&characterEncoding=utf8&zeroDateTimeBehavior=convertToNull&serverTimezone=CTT
        username: root
        druid:
            initial-size: 5  # 初始化大小
            min-idle: 5  # 最小
            max-active: 100  # 最大
            max-wait: 60000  # 配置获取连接等待超时的时间
            time-between-eviction-runs-millis: 60000  # 配置间隔多久才进行一次检测，检测需要关闭的空闲连接，单位是毫秒
            min-evictable-idle-time-millis: 300000  # 指定一个空闲连接最少空闲多久后可被清除，单位是毫秒
            validationQuery: select 'x'
            test-while-idle: true  # 当连接空闲时，是否执行连接测试
            test-on-borrow: false  # 当从连接池借用连接时，是否测试该连接
            test-on-return: false  # 在连接归还到连接池时是否测试该连接
            filters: config,wall,stat  # 配置监控统计拦截的filters，去掉后监控界面sql无法统计，'wall'用于防火墙
            poolPreparedStatements: true # 打开PSCache，并且指定每个连接上PSCache的大小
            maxPoolPreparedStatementPerConnectionSize: 20
            maxOpenPreparedStatements: 20
            # 通过connectProperties属性来打开mergeSql功能；慢SQL记录
            connectionProperties: druid.stat.slowSqlMillis=200;druid.stat.logSlowSql=true;config.decrypt=false
    quartz:
        #数据库方式
        job-store-type: jdbc
        #初始化表结构
        #        jdbc:
        #            initialize-schema: never
        properties:
            org:
                quartz:
                    scheduler:
                        instanceId: AUTO
                        instanceName: youlin-scheduler
                    jobStore:
                        class: org.quartz.impl.jdbcjobstore.JobStoreTX
                        clusterCheckinInterval: 10000
                        driverDelegateClass: org.quartz.impl.jdbcjobstore.StdJDBCDelegate
                        isClustered: true
                        tablePrefix: QRTZ_
                        useProperties: false
                    threadPool:
                        class: org.quartz.simpl.SimpleThreadPool
                        threadCount: 10
                        threadPriority: 5
                        threadsInheritContextClassLoaderOfInitializingThread: true
```

### 代码修改

- 注释掉类`SchedulerCenter`中的`SchedulerFactoryBean`的bean定义内容：

```java
//    @Autowired
//    private SpringJobFactory springJobFactory;

//    @Bean(name = "schedulerFactoryBean")
//    public SchedulerFactoryBean schedulerFactoryBean() {
//        SchedulerFactoryBean schedulerFactoryBean = new SchedulerFactoryBean();
//        schedulerFactoryBean.setJobFactory(springJobFactory);
//        return schedulerFactoryBean;
//    }
```

- 修改新增Job任务的代码，先判断JobKey是否存在，存在则删除的逻辑：

```java
public void userReportJob(Scheduler scheduler) throws SchedulerException{
    // 定义JobKey
    JobKey jobKey = new JobKey("job001", "group001");
    if (scheduler.checkExists(jobKey)) {
        scheduler.deleteJob(jobKey);
    }
    //创建任务明细，名称job1,组group1
    JobDetail jobDetail = JobBuilder.newJob(UserReportLogJob.class) .withIdentity(jobKey).build();
    //每周一零点1分执行一次,创建调度
    CronScheduleBuilder scheduleBuilder = CronScheduleBuilder.cronSchedule("1 0 0 ? * 2"); //每周周一
    //创建触发器，名称trigger1,组group1
    CronTrigger cronTrigger = TriggerBuilder.newTrigger().withIdentity("trigger001", "group001") .withSchedule(scheduleBuilder).build();
    scheduler.scheduleJob(jobDetail,cronTrigger);
}
```
